/**
*
* @author sean.7
* @date 1/24/2015
* @about class Point to define any point in 3 dimensional space.
*
*/
#ifndef POINT_H
#define POINT_H

#include <string>
#include <sstream>
#include "Vector.h"

using namespace std;

class Point{
public:
	double x, y, z;
	// Defining Constructors for the class
	Point(){
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	Point(const double &X, const double &Y, const double &Z){
		x = X;
		y = Y;
		z = Z;
	}
	Point(const Point& p){
		x = p.x;
		y = p.y;
		z = p.z;
	}
	~Point(){}


	//Assignment operator
	Point& operator=(const Point& p){
		if (this == &p)
			return *this;
		else {
			x = p.x;
			y = p.y;
			z = p.z;
			return *this;
		}
	}

	//Output Format of the point
	string str() {
		stringstream buffer;
		buffer << "Point(" << x << ", " << y << ", " << z << ")";
		return buffer.str();
	}

	//Equality operators
	bool operator==(Point& p) {
		if (this == &p)
			return true;
		else
			return (x == p.x & y == p.y & z == p.z);
	}

	bool operator!=(Point& p) {
		if (this == &p)
			return false;
		else
			return !(x == p.x & y == p.y & z == p.z);
	}

	Point operator+(Vector &v) const {
		Point point;
		point.x = x + v.x;
		point.y = y + v.y;
		point.z = z + v.z;
		return point;
	}

	Vector operator-(const Point& p) const {
		Vector v;
		v.x = x - p.x;
		v.y = y - p.y;
		v.z = z - p.z;
		return v;
	}
};


#endif