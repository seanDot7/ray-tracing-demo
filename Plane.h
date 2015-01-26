/**
*
* @author sean.7
* @date 1/25/2015
* @about class Sphere for representing sphere.
*
*/

#ifndef PLANE_H
#define PLANE_H

#include <sstream>
#include "Point.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"
#include <math.h>


using namespace std;


class Plane : public Object {
public:
	Point p1, p2, p3, p4;

	//double a;	// opacity
	//double u;	// refractive index
	Plane() {
	}

	Plane(const Point &p1, const Point &p2, const Point &p3, const Point &p4, const Color &C, const Color &sC = Color(1, 1, 1)) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		this->p4 = p4;
		color = C;
		specularColor = sC;
	}

	Plane(const Point &p1, const Point &p2, const Point &p3, const Color &C, const Color &sC = Color(1, 1, 1)) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		Vector p2p3 = p3 - p2;
		this->p4 = p1 + p2p3;
		color = C;
		specularColor = sC;
	}

	Plane(const Plane& p) {
		p1 = p.p1;
		p2 = p.p2;
		p3 = p.p3;
		p4 = p.p4;
		color = p.color;
		specularColor = p.specularColor;
	}

	~Plane(){}

	string str() {
		stringstream buffer;
		buffer << "Plane[" << p1.str() << ", " << p2.str() << ", " << p3.str() << ", " << p4.str() << ", " << color.str() << "]";
		return buffer.str();
	}

	bool isIntersected(const Ray &ray) {
		Vector p1p2 = p2 - p1;
		Vector p1p4 = p4 - p1;
		Vector normal = p1p2.cross(p1p4);
		normal.normalize();

		if (ray.direction.dot(normal) == 0)
			return false;
		else {

			Vector p1v = ray.origin - p1;
			double t = - p1v.dot(normal) / ray.direction.dot(normal);
			if (t < 0)
				return false;
			else {
				Point p = ray.origin + ray.direction * t;
				if (p == p1 || p == p2 || p == p3 || p == p4)
					return true;

				Vector p1p = p - p1;
				Vector p2p = p - p2;
				Vector p3p = p - p3;
				Vector p4p = p - p4;
				Vector p2p3 = p3 - p2;
				Vector p3p4 = p4 - p3;
				Vector p4p1 = p1 - p4;

				if (p1p2.cross(p1p).hasSameDirectionWith(p2p3.cross(p2p))
					&& p2p3.cross(p2p).hasSameDirectionWith(p3p4.cross(p3p))
					&& p3p4.cross(p3p).hasSameDirectionWith(p4p1.cross(p4p))
					)
					return true;
				else
					return false;

			}
				
		}
	}

	Point getClosestIntersectedPoint(const Ray &ray) {
		if (isIntersected(ray)) {
			Vector p1p2 = p2 - p1;
			Vector p1p4 = p4 - p1;
			Vector normal = p1p2.cross(p1p4);
			normal.normalize();

			Vector p1v = ray.origin - p1;
			double t = -p1v.dot(normal) / ray.direction.dot(normal);
			Point p = ray.origin + ray.direction * t;
			return p;
		} else
			return Point();
	}

	Vector getNormal(const Ray &ray) {
		Vector p1p2 = p2 - p1;
		Vector p1p4 = p4 - p1;
		Vector normal = p1p4.cross(p1p2);
		normal.normalize();
		return normal;
	}

};

#endif