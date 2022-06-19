#ifndef __WORLD_TREEWORLD_HPP__
#define __WORLD_TREEWORLD_HPP__

#include "Box.hpp"
#include "SimpleWorld.hpp"


class TreeWorld: public SimpleWorld {

protected:

	Box const * tree;

public:

	inline TreeWorld(): SimpleWorld() {}

	inline ~TreeWorld() {
		if (tree != nullptr)
			delete tree;
	}

	void sort();

	virtual Hit hit(Ray const& ray, bool const inside) const override;
	virtual Light trace(Ray const& ray, bool const inside, int const samples, int const depth) const override;

protected:

	virtual Box* sort(std::vector<Box*> boxes, bool const recursive = true);

};


#endif
