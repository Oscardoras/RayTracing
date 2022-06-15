#ifndef OLDWORLD_H_
#define OLDWORLD_H_

#include <limits>
#include <memory>
#include <vector>
#include <cmath>

#include "algebra/Light.hpp"
#include "algebra/Ray.hpp"
#include "objects/Object.hpp"
#include "Box.hpp"


class World {

protected:

	virtual std::shared_ptr<Box> sort(std::vector<std::shared_ptr<Box>> boxes, bool const& recursive = true) {
		std::shared_ptr<Box> box = std::make_shared<Box>();
		for (std::shared_ptr<Box> const& b : boxes) {
			if (b->m.x < box->m.x) box->m.x = b->m.x;
			if (b->M.x > box->M.x) box->M.x = b->M.x;
			if (b->m.y < box->m.y) box->m.y = b->m.y;
			if (b->M.y > box->M.y) box->M.y = b->M.y;
			if (b->m.z < box->m.z) box->m.z = b->m.z;
			if (b->M.z > box->M.z) box->M.z = b->M.z;
		}
		int size = boxes.size();
		if (!recursive || size <= 2) {
			for (std::shared_ptr<Box> const& b : boxes)
				box->primitives.push_back(b->primitives[0]);
		} else {
			int axis;
			float intersection = Infinite;
			for (int ax = 1; ax <= 4; ax++) {
				bool finish = (ax == 4);
				if (finish) ax = axis;
				auto comparator = ax == 1 ? xComparator : (ax == 2 ? yComparator : zComparator);
				std::sort(boxes.begin(), boxes.end(), comparator);
				int middle = size / 2;

				std::vector<std::shared_ptr<Box>> f = std::vector<std::shared_ptr<Box>>();
				for (int i = 0; i < middle; i++) f.push_back(boxes[i]);
				std::shared_ptr<Box> first = sort(f, finish);

				std::vector<std::shared_ptr<Box>> s = std::vector<std::shared_ptr<Box>>();
				for (int i = middle; i < size; i++) s.push_back(boxes[i]);
				std::shared_ptr<Box> second = sort(s, finish);

				if (finish) {
					if (f.size() == 1) box->primitives.push_back(first->primitives[0]);
					else box->boxes.push_back(first);
					if (s.size() == 1) box->primitives.push_back(second->primitives[0]);
					else box->boxes.push_back(second);
					break;
				} else {
					float a = first->M.x - second->m.x;
					if (a < 0) a = first->m.x - second->M.x;
					if (a < 0) a = 0;
					float b = first->M.y - second->m.y;
					if (b < 0) b = first->m.y - second->M.y;
					if (b < 0) b = 0;
					float c = first->M.z - second->m.z;
					if (c < 0) c = first->m.z - second->M.z;
					if (c < 0) c = 0;
					float inter = a*b*c;
					if (inter < intersection) {
						axis = ax;
						intersection = inter;
					}
				}
				
			}
		}
		return box;
	}

public:

	void sort(bool s = true) {
		std::vector<std::shared_ptr<Box>> boxes;
		for (std::shared_ptr<Primitive> const& primitive : primitives) {
			std::shared_ptr<Box> box = std::make_shared<Box>(primitive->getBox());
			box->primitives.push_back(primitive);
			boxes.push_back(box);
		}
		tree = sort(boxes, s);
	}

	Hit hit(Ray const& ray, bool inside) const {
		nbr++;
		return tree->hit(ray, 0.001, Infinity, inside);
	}

	Light trace(Ray const& ray, bool inside, int samples, int depth) const {
		if (depth > 0) {
			Hit hit = this->hit(ray, inside);
			if (!std::isnan(hit.t)) return hit.primitive->color(Ray(ray.at(hit.t), ray.v), *this, samples, depth-1);
			else return Light(infiniteSpectrum(ray));
		} else return Light(maxDepthSpectrum(ray));
	}

};


#endif
