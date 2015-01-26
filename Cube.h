/**
*
* @author sean.7
* @date 1/25/2015
* @about class Cube for representing cube.
*
*/

#ifndef CUBE_H
#define CUBE_H

#include <sstream>
#include "Point.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"
#include "Plane.h"
#include <math.h>


using namespace std;


class Cube : public Object {
public:
	Plane plane[6];

	//double a;	// opacity
	//double u;	// refractive index
	Cube() {
	}

	Cube(const Plane planeArr[6], const Color &C, const Color &sC = Color(1, 1, 1)) {
		for (int i = 0; i < 6; ++i) {
			plane[i] = planeArr[i];
		}
		color = C;
		specularColor = sC;
	}

	Cube(const Cube &c) {
		for (int i = 0; i < 6; ++i) {
			plane[i] = c.plane[i];
		}
		color = c.color;
		specularColor = c.specularColor;
	}

	~Cube(){}

	string str() {
		stringstream buffer;
		buffer << "Cube[" 
			<< plane[0].str() << ", " 
			<< plane[1].str() << ", "
			<< plane[2].str() << ", "
			<< plane[3].str() << ", "
			<< plane[4].str() << ", "
			<< plane[5].str() << ", "
			<< color.str() << "]";
		return buffer.str();
	}

	bool isIntersected(const Ray &ray) {
		for (int i = 0; i < 6; ++i) {
			if (plane[i].isIntersected(ray))
				return true;
		}
		return false;
	}

	Point getClosestIntersectedPoint(const Ray &ray) {
		Point closestPoint;
		double minDist = 999999999;
		if (isIntersected(ray)) {
			for (int i = 0; i < 6; ++i) {
				if (plane[i].isIntersected(ray)) {
					Point tempPoint = plane[i].getClosestIntersectedPoint(ray);
					if (minDist >(tempPoint - ray.origin).length()) {
						minDist = (tempPoint - ray.origin).length();
						closestPoint = tempPoint;
					}
				}
			}
			return closestPoint;
		} else
			return Point();
	}

	Vector getNormal(const Ray &ray) {
		Point closestPoint;
		double minDist = 999999999;
		Vector normal;
		if (isIntersected(ray)) {
			for (int i = 0; i < 6; ++i) {
				if (plane[i].isIntersected(ray)) {
					Point tempPoint = plane[i].getClosestIntersectedPoint(ray);
					if (minDist >(tempPoint - ray.origin).length()) {
						minDist = (tempPoint - ray.origin).length();
						closestPoint = tempPoint;
						normal = plane[i].getNormal(ray);
					}
				}
			}
			return normal;
		} else
			return Vector();
	}

};

#endif