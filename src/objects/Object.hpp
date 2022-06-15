#ifndef __OBJECTS_OBJECT_H__
#define __OBJECTS_OBJECT_H__

#include <vector>

struct Primitive;


struct Object {

	virtual std::vector<Primitive*> getPrimitives() = 0;

};


#endif
