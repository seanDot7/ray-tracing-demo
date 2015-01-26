/**
*
* @author sean.7
* @date 1/25/2015
* @about class Ray for representing ray by integrating Point class and Vector class.
*
*/

#ifndef RAY_H
#define RAY_H
#include "Color.h"
#include <sstream>
#include "Point.h"
#include "Vector.h"

class Ray {
public:
	Point origin;
	Vector direction;
	Color color;

	Ray(){}
	Ray(const Point &point, const Vector &vector, const Color &color = Color(0, 0, 0)) {
		this->origin = point;
		this->direction = vector;
		this->color = color;
	}
	Ray(const Ray &ray) {
		origin = ray.origin;
		direction = ray.direction;
		color = ray.color;
	}
	~Ray(){}

	string str() {
		stringstream buffer;
		buffer << "Ray[" << origin.str() << ", " << direction.str() << ", " << color.str() << "]";
		return buffer.str();
	}
};
#endif