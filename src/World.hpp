#ifndef WORLD_H_
#define WORLD_H_

#include <limits>
#include <memory>
#include <vector>
#include <cmath>

#include "algebra/Light.hpp"
#include "algebra/Ray.hpp"
#include "objects/Object.hpp"
#include "Box.hpp"
#include "Priority.hpp"


class World {

public:

	std::vector<std::shared_ptr<Object>> objects;
	std::shared_ptr<Box> tree;
	std::vector<std::shared_ptr<Priority>> priorities;
	float priority = 0.5;

	virtual Spectrum infiniteColor(Ray const& r) const = 0;

	void isEmpty() {
		objects.empty();
	}

	void add(std::shared_ptr<Object> object) {
		objects.push_back(object);
	}

	void clear() {
		objects.clear();
	}

protected:

	virtual std::shared_ptr<Box> sort(std::vector<std::shared_ptr<Box>> const& boxes) {
		std::shared_ptr<Box> box = std::make_shared<Box>();
		for (const std::shared_ptr<Box> b : boxes) {
			if (b->m.x < box->m.x) box->m.x = b->m.x;
			if (b->M.x > box->M.x) box->M.x = b->M.x;
			if (b->m.y < box->m.y) box->m.y = b->m.y;
			if (b->M.y > box->M.y) box->M.y = b->M.y;
			if (b->m.z < box->m.z) box->m.z = b->m.z;
			if (b->M.z > box->M.z) box->M.z = b->M.z;
		}
		int size = boxes.size();
		if (size <= 2) {
			box->objects.push_back(boxes[0]->objects[0]);
			box->objects.push_back(boxes[1]->objects[0]);
		} else if (size == 3) {
			box->objects.push_back(boxes[0]->objects[0]);
			std::vector<std::shared_ptr<Box>> second = std::vector<std::shared_ptr<Box>>();
			second.push_back(boxes[1]);
			second.push_back(boxes[2]);
			box->boxes.push_back(sort(second));
		} else {
			int axis = random_int(1, 3);
			/*auto comparator = axis == 1 ? xComparator : (axis == 2 ? yComparator : zComparator);
			std::sort(boxes.begin(), boxes.end(), comparator);*/
			int middle = size / 2;
			std::vector<std::shared_ptr<Box>> first = std::vector<std::shared_ptr<Box>>();
			for (int i = 0; i < middle; i++) first.push_back(boxes[i]);
			box->boxes.push_back(sort(first));
			std::vector<std::shared_ptr<Box>> second = std::vector<std::shared_ptr<Box>>();
			for (int i = middle; i < size; i++) second.push_back(boxes[i]);
			box->boxes.push_back(sort(second));
		}
		return box;
	}

public:

	virtual void sort(bool s = true) {
		std::vector<std::shared_ptr<Box>> boxes;
		for (const std::shared_ptr<Object> object : objects) {
			boxes.push_back(std::make_shared<Box>(object->getBox()));
		}
		if (!s || objects.size() <= 1000) {
			tree = std::make_shared<Box>();
			for (const std::shared_ptr<Box> b : boxes) {
				if (b->m.x < tree->m.x) tree->m.x = b->m.x;
				if (b->M.x > tree->M.x) tree->M.x = b->M.x;
				if (b->m.y < tree->m.y) tree->m.y = b->m.y;
				if (b->M.y > tree->M.y) tree->M.y = b->M.y;
				if (b->m.z < tree->m.z) tree->m.z = b->m.z;
				if (b->M.z > tree->M.z) tree->M.z = b->M.z;
			}
			tree->objects = objects;
		} else {
			tree = sort(boxes);
		}
	}

	/*Hit hit(Ray const& ray, bool const& in) const {
		nbr++;
		Hit hit;
		hit.t = Infinite;
		for (const std::shared_ptr<Object> object : objects) {
			float t = object->hit(ray, 0.001, hit.t, in);
			if (!std::isnan(t) && t < hit.t) {
				hit.t = t;
				hit.object = object;
			}
		}
		return hit;
	}*/
	Hit hit(Ray const& ray, bool const& in) const {
		nbr++;
		return tree->hit(ray, 0.001, Infinite, in);
	}

	Light trace(Ray const& ray, bool const& in, int const& remainingRays, int const& maxDepth) const {
		if (maxDepth > 0) {
			Hit hit = this->hit(ray, in);
			if (std::isfinite(hit.t)) return hit.object->color(Ray(ray.at(hit.t), ray.direction, ray.ior), *this, remainingRays, maxDepth-1);
			else return Light(infiniteColor(ray), Light(), Light(), Spectrum(), Spectrum());
		} else return Light();
	}

};


#endif
