#ifndef __OBJECTS_OBJECT_H__
#define __OBJECTS_OBJECT_H__

#include <vector>

struct Primitive;
struct Box;


struct Object {

	virtual std::vector<Primitive*> getPrimitives() = 0;

	virtual Box* getBox() const = 0;

};


#endif
