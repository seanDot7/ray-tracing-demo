/**
*
* @author sean.7
* @date 1/25/2015
* @about class Cylinder for representing sphere.
*
*/

#ifndef CYLINDER_H
#define CYLINDER_H

#include <sstream>
#include "Point.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"
#include <math.h>
#include <iostream>

using namespace std;


class Cylinder : public Object {
public:
	Point center1, center2;
	double r;

	//double a;	// opacity
	//double u;	// refractive index
	Cylinder() {
	}

	Cylinder(const Point &c1, const Point &c2, const double &r, const Color &C, const Color &sC = Color(1, 1, 1)) {
		center1 = c1;
		center2 = c2;
		this->r = r;
		color = C;
		specularColor = sC;
	}

	Cylinder(const Cylinder& cylinder) {
		center1 = cylinder.center1;
		center2 = cylinder.center2;
		r = cylinder.r;
		color = cylinder.color;
		specularColor = cylinder.specularColor;
	}

	~Cylinder(){}

	string str() {
		stringstream buffer;
		buffer << "Cylinder[" << center1.str() << ", " << center2.str() << ", " << r << ", " << color.str() << "]";
		return buffer.str();
	}

	bool isIntersected(const Ray &ray) {
		// check top & bottom plane
		Vector normal = center2 - center1;
		normal.normalize();

		bool flagIntersectTopOrBottom = false;
		if (ray.direction.dot(normal) != 0) {
			Vector c1v = ray.origin - center1;
			double t = -c1v.dot(normal) / ray.direction.dot(normal);
			if (t >= 0) {
				Point intersectedPoint =ray.origin + ray.direction * t;
				if ((intersectedPoint - center1).length() <= r)
					flagIntersectTopOrBottom = true;
			}
			if (!flagIntersectTopOrBottom) {
				Vector c2v = ray.origin - center2;
				double t = -c2v.dot(normal) / ray.direction.dot(normal);
				if (t >= 0) {
					Point intersectedPoint = ray.origin + ray.direction * t;
					if ((intersectedPoint - center2).length() <= r)
						flagIntersectTopOrBottom = true;
				}
			}
		}

		if (flagIntersectTopOrBottom)
			return true;

		// check curve surface
		bool flagIntersectCurveSurface = false;
		Vector c1ov = ray.origin - center1;
		Vector d = ray.direction;
		Vector n = center2 - center1;
		n.normalize();

		double A = d.dot(d) - 2.0 * d.dot(n) * d.dot(n) + n.dot(n) * d.dot(n) * d.dot(n);
		double B = 2.0 * (d.dot(c1ov) + d.dot(n) * c1ov.dot(n) * n.dot(n) - 2.0 * d.dot(n) * n.dot(c1ov));
		double C = (c1ov.dot(c1ov) + c1ov.dot(n) * c1ov.dot(n) * n.dot(n) - 2.0 * c1ov.dot(n) * c1ov.dot(n)) - r * r;
		double delta = B * B - 4.0 * A * C;

		if (delta >= 0) {
			double t1 = (-B + sqrt(delta)) / (2.0 * A);
			double t2 = (-B - sqrt(delta)) / (2.0 * A);
			if (t1 >= 0 || t2 >= 0) {
				if (t1 >= 0) {
					Point tempPoint = ray.origin + ray.direction * t1;
					Vector tempVector = n * (tempPoint - center1).dot(n);
					double ratio = tempVector.getRatio(center2 - center1);
					if (ratio >= 0 && ratio <= 1) {
						flagIntersectCurveSurface = true;
					}
				}
				if (t2 >= 0) {
					Point tempPoint = ray.origin + ray.direction * t2;
					Vector tempVector = n * (tempPoint - center1).dot(n);
					double ratio = tempVector.getRatio(center2 - center1);
					if (ratio >= 0 && ratio <= 1) {
						flagIntersectCurveSurface = true;
					}
				}
			}
		}

		if (flagIntersectCurveSurface || flagIntersectTopOrBottom)
			return true;
		else
			return false;
	}

	Point getClosestIntersectedPoint(const Ray &ray) {
		// check top & bottom plane
		Vector normal = center2 - center1;
		normal.normalize();

		double minT = 99999999;

		bool flagIntersectTopOrBottom = false;
		if (ray.direction.dot(normal) != 0) {
			Vector c1v = ray.origin - center1;
			double t = -c1v.dot(normal) / ray.direction.dot(normal);
			if (t >= 0) {
				Point intersectedPoint = ray.origin + ray.direction * t;
				if ((intersectedPoint - center1).length() <= r) {
					flagIntersectTopOrBottom = true;
					if (minT > t)
						minT = t;
				}
			}
			if (!flagIntersectTopOrBottom) {
				Vector c2v = ray.origin - center2;
				double t = -c2v.dot(normal) / ray.direction.dot(normal);
				if (t >= 0) {
					Point intersectedPoint = ray.origin + ray.direction * t;
					if ((intersectedPoint - center2).length() <= r) {
						flagIntersectTopOrBottom = true;
						if (minT > t)
							minT = t;
					}
				}
			}
		}

		// check curve surface
		bool flagIntersectCurveSurface = false;
		Vector c1ov = ray.origin - center1;
		Vector d = ray.direction;
		Vector n = center2 - center1;
		n.normalize();

		double A = d.dot(d) - 2.0 * d.dot(n) * d.dot(n) + n.dot(n) * d.dot(n) * d.dot(n);
		double B = 2.0 * (d.dot(c1ov) + d.dot(n) * c1ov.dot(n) * n.dot(n) - 2.0 * d.dot(n) * n.dot(c1ov));
		double C = (c1ov.dot(c1ov) + c1ov.dot(n) * c1ov.dot(n) * n.dot(n) - 2.0 * c1ov.dot(n) * c1ov.dot(n)) - r * r;
		double delta = B * B - 4.0 * A * C;
		if (delta >= 0) {
			double t1 = (-B + sqrt(delta)) / (2.0 * A);
			double t2 = (-B - sqrt(delta)) / (2.0 * A);
			if (t1 >= 0 || t2 >= 0) {
				
				if (t1 >= 0) {
					Point tempPoint = ray.origin + ray.direction * t1;
					Vector tempVector = n * (tempPoint - center1).dot(n);
					double ratio = tempVector.getRatio(center2 - center1);
					if (ratio >= 0 && ratio <= 1 && minT > t1) {
						minT = t1;
						flagIntersectCurveSurface = true;
					}
				}
				if (t2 >= 0 && minT > t2) {
					Point tempPoint = ray.origin + ray.direction * t2;
					Vector tempVector = n * (tempPoint - center1).dot(n);
					double ratio = tempVector.getRatio(center2 - center1);
					if (ratio >= 0 && ratio <= 1 && minT > t2) {
						minT = t2;
						flagIntersectCurveSurface = true;
					}
				}
			}
		}

		if (flagIntersectCurveSurface || flagIntersectTopOrBottom)
			return ray.origin + ray.direction * minT;
		else
			return Point();
	}

	Vector getNormal(const Ray &ray) {
		// check top & bottom plane
		Vector returnNormal;
		Vector normal = center2 - center1;
		normal.normalize();

		double minT = 99999999;

		bool flagIntersectTopOrBottom = false;
		if (ray.direction.dot(normal) != 0) {
			Vector c1v = ray.origin - center1;
			double t = -c1v.dot(normal) / ray.direction.dot(normal);
			if (t >= 0) {
				Point intersectedPoint = ray.origin + ray.direction * t;
				if ((intersectedPoint - center1).length() <= r) {
					flagIntersectTopOrBottom = true;
					if (minT > t) {
						minT = t;
						returnNormal = -normal;
					}
				}
			}
			if (!flagIntersectTopOrBottom) {
				Vector c2v = ray.origin - center2;
				double t = -c2v.dot(normal) / ray.direction.dot(normal);
				if (t >= 0) {
					Point intersectedPoint = ray.origin + ray.direction * t;
					if ((intersectedPoint - center2).length() <= r) {
						flagIntersectTopOrBottom = true;
						if (minT > t) {
							minT = t;
							returnNormal = normal;
						}
					}
				}
			}
		}

		// check curve surface
		bool flagIntersectCurveSurface = false;
		Vector c1ov = ray.origin - center1;
		Vector d = ray.direction;
		Vector n = center2 - center1;
		n.normalize();

		double A = d.dot(d) - 2.0 * d.dot(n) * d.dot(n) + n.dot(n) * d.dot(n) * d.dot(n);
		double B = 2.0 * (d.dot(c1ov) + d.dot(n) * c1ov.dot(n) * n.dot(n) - 2.0 * d.dot(n) * n.dot(c1ov));
		double C = (c1ov.dot(c1ov) + c1ov.dot(n) * c1ov.dot(n) * n.dot(n) - 2.0 * c1ov.dot(n) * c1ov.dot(n)) - r * r;
		double delta = B * B - 4.0 * A * C;
		if (delta >= 0) {
			double t1 = (-B + sqrt(delta)) / (2.0 * A);
			double t2 = (-B - sqrt(delta)) / (2.0 * A);
			if (t1 >= 0 || t2 >= 0) {
				if (t1 >= 0) {
					Point tempPoint = ray.origin + ray.direction * t1;
					Vector tempVector = n * (tempPoint - center1).dot(n);
					double ratio = tempVector.getRatio(center2 - center1);
					if (ratio >= 0 && ratio <= 1 && minT > t1) {
						minT = t1;
						flagIntersectCurveSurface = true;
						Point p = ray.origin + ray.direction * t1;
						Vector c1p = p - center1;
						returnNormal = c1p - n * c1p.dot(n);
					}
				}


				if (t2 >= 0) {
					Point tempPoint = ray.origin + ray.direction * t2;
					Vector tempVector = n * (tempPoint - center1).dot(n);
					double ratio = tempVector.getRatio(center2 - center1);
					if (ratio >= 0 && ratio <= 1 && minT > t2) {
						minT = t2;
						flagIntersectCurveSurface = true;
						Point p = ray.origin + ray.direction * t2;
						Vector c1p = p - center1;
						returnNormal = c1p - n * c1p.dot(n);
					}
				}
			}
		}


		if (flagIntersectCurveSurface || flagIntersectTopOrBottom) {
			returnNormal.normalize();
			return returnNormal;
		} else
			return Vector();
	}

};

#endif