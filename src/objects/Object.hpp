#ifndef OBJECTS_OBJECT_H_
#define OBJECTS_OBJECT_H_

#include <vector>

class Primitive;


class Object {

public:

	virtual std::vector<std::shared_ptr<Primitive>> getPrimitives() const {
		return std::vector<std::shared_ptr<Primitive>>();
	}

};


#endif
