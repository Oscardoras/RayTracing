#ifndef OBJECTS_PARALLEPIPED_H_
#define OBJECTS_PARALLEPIPED_H_

#include <memory>
#include <algorithm>

#include "Rectangle.hpp"


class Parallelepiped: public Object {

public:

	std::shared_ptr<Rectangle> R1;
	std::shared_ptr<Rectangle> R2;
	std::shared_ptr<Rectangle> R3;
	std::shared_ptr<Rectangle> R4;
	std::shared_ptr<Rectangle> R5;
	std::shared_ptr<Rectangle> R6;

	Parallelepiped(Point A, Point B, Point F, Point G, std::shared_ptr<Material> material) {
		Point C = B + (G-F);
		Point D = A + (G-F);
		Point E = A + (F-B);
		Point H = D + (F-B);
		R1 = std::make_shared<Rectangle>(A, B, C, material);
		R2 = std::make_shared<Rectangle>(A, B, F, material);
		R3 = std::make_shared<Rectangle>(B, C, G, material);
		R4 = std::make_shared<Rectangle>(D, C, G, material);
		R5 = std::make_shared<Rectangle>(A, D, H, material);
		R6 = std::make_shared<Rectangle>(E, F, G, material);
	}

	virtual std::vector<std::shared_ptr<Primitive>> getPrimitives() const override {
		std::vector<std::shared_ptr<Primitive>> primitives;
		primitives.push_back(R1->T1);
		primitives.push_back(R1->T2);
		primitives.push_back(R2->T1);
		primitives.push_back(R2->T2);
		primitives.push_back(R3->T1);
		primitives.push_back(R3->T2);
		primitives.push_back(R4->T1);
		primitives.push_back(R4->T2);
		primitives.push_back(R5->T1);
		primitives.push_back(R5->T2);
		primitives.push_back(R6->T1);
		primitives.push_back(R6->T2);
		return primitives;
	}

};


#endif
