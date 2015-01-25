/**
*
* @author sean.7
* @date 1/25/2015
* @about class Object.
*
*/

#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Color.h"

class Object {
public:
	Color color, specularColor;

	virtual bool isIntersected(const Ray &ray) = 0;
	virtual Point getClosestIntersectedPoint(const Ray &ray) = 0;
	virtual Vector getNormal(const Ray &ray) = 0;
};

#endif