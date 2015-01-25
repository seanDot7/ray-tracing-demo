/**
*
* @author sean.7
* @date 1/25/2015
* @about class Sphere for representing sphere.
*
*/

#ifndef SPHERE_H
#define SPHERE_H

#include <sstream>
#include "Point.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"
#include <math.h>

using namespace std;


class Sphere : public Object {
public:
	Point o;
	double r;

	//double a;	// opacity
	//double u;	// refractive index
	Sphere() {
		r = 0.0;
	}

	Sphere(const Point& O, const double R, const Color &C, const Color &sC = Color(1, 1, 1)) {
		o = O;
		r = R;
		color = C;
		specularColor = sC;
	}

	Sphere(const Sphere& s) {
		o = s.o;
		r = s.r;
		color = s.color;
		specularColor = s.specularColor;
	}

	~Sphere(){}

	string str() {
		stringstream buffer;
		buffer << "Sphere[" << o.str() << ", " << r << ", " << color.str() << "]";
		return buffer.str();
	}

	bool isIntersected(const Ray &ray) {
		double A = ray.direction.dot(ray.direction);
		Vector vectorSphereToView(ray.origin - o);
		double B = 2.0 * vectorSphereToView.dot(ray.direction);
		double C = vectorSphereToView.dot(vectorSphereToView) - r * r;
		double delta = B * B - 4.0 * A * C;
		if (delta < 0) {
			return false;
		} else {
			double t1 = (-B + sqrt(delta)) / (2.0 * A);
			double t2 = (-B - sqrt(delta)) / (2.0 * A);
			if (t1 < 0 && t2 < 0)
				return false;
			else
				return true;
		}
	}

	Point getClosestIntersectedPoint(const Ray &ray) {
		double A = ray.direction.dot(ray.direction);
		Vector vectorSphereToView(ray.origin - o);
		double B = 2.0 * vectorSphereToView.dot(ray.direction);
		double C = vectorSphereToView.dot(vectorSphereToView) - r * r;
		double delta = B * B - 4.0 * A * C;

		if (delta < 0) {
			return Point(0, 0, 0);
		}
		else {
			double t1 = (-B + sqrt(delta)) / (2.0 * A);
			double t2 = (-B - sqrt(delta)) / (2.0 * A);

			if (t1 < 0 && t2 < 0)
				return Point(0, 0, 0);
			else {
				// intersected with the ray
				if (t1 >= 0 && t2 >= 0) {
					if (t1 < t2)
						return ray.origin + ray.direction * t1;
					else
						return ray.origin + ray.direction * t2;
				} else if (t1 >= 0) {
					return ray.origin + ray.direction * t1;
				} else if (t2 >= 0) {
					return ray.origin + ray.direction * t2;
				}
			}
	
		}
	}

	Vector getNormal(const Ray &ray) {
		Point intersectedPoint = getClosestIntersectedPoint(ray);
		Vector normal(intersectedPoint - o);
		normal.normalize();
		return normal;
	}

};

#endif