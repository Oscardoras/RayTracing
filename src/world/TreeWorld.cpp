#include <algorithm>

#include "TreeWorld.hpp"


void TreeWorld::sort() {
	std::vector<Box*> boxes;
	for (Primitive const * const primitive : primitives)
		boxes.push_back(primitive->getBox());
	tree = sort(boxes, true);
}

Hit TreeWorld::hit(Ray const& ray, bool const inside) const {
	(*nbr)++;
	return tree->hit(ray, 0.001, Infinity, inside);
}

Light TreeWorld::trace(Ray const& ray, bool const inside, int const samples, int const depth) const {
	if (depth > 0) {
		Hit hit = this->hit(ray, inside);
		if (!std::isnan(hit.t))
			return hit.primitive->color(*this, Ray(ray.at(hit.t), ray.v.unit()), samples, depth-1);
		else
			return Light(infiniteLight(ray));
	} else return Light(infiniteLight(ray));
}

Box* TreeWorld::sort(std::vector<Box*> boxes, bool const recursive) {
	Box* box = new Box();

	for (Box* const b : boxes) {
		if (b->m.x < box->m.x) box->m.x = b->m.x;
		if (b->M.x > box->M.x) box->M.x = b->M.x;
		if (b->m.y < box->m.y) box->m.y = b->m.y;
		if (b->M.y > box->M.y) box->M.y = b->M.y;
		if (b->m.z < box->m.z) box->m.z = b->m.z;
		if (b->M.z > box->M.z) box->M.z = b->M.z;
	}

	int size = boxes.size();
	if (!recursive || size <= 2) {
		for (Box* const b : boxes)
			box->primitives.push_back(b->primitives[0]);
	} else {
		int axis;
		float intersection = Infinity;
		for (int ax = 1; ax <= 4; ax++) {
			bool finish = (ax == 4);
			if (finish) ax = axis;
			auto comparator = ax == 1 ? xComparator : (ax == 2 ? yComparator : zComparator);
			std::sort(boxes.begin(), boxes.end(), comparator);
			int middle = size / 2;

			std::vector<Box*> f = std::vector<Box*>();
			for (int i = 0; i < middle; i++)
				f.push_back(boxes[i]);
			Box* first = sort(f, finish);

			std::vector<Box*> s = std::vector<Box*>();
			for (int i = middle; i < size; i++)
				s.push_back(boxes[i]);
			Box* second = sort(s, finish);

			if (finish) {
				if (f.size() == 1)box->primitives.push_back(first->primitives[0]);
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