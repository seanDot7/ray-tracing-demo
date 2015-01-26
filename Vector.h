/**
*
* @author sean.7
* @date 1/24/2015
* @about class Vector for representing a vector in free space and functions for mathematical operations in a vector.
*
*/
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <string>
#include <sstream>


#define ACCURACY 0.000001
using namespace std;

class Vector {
public:
	double x, y, z;

	// defining constructors for the class
	Vector() {
		x = 0.0; y = 0.0; z = 0.0;
	}

	Vector(const double &X, const double &Y, const double &Z) {
		x = X; y = Y; z = Z;
	}

	Vector(const Vector& v){
		x = v.x; y = v.y; z = v.z;
	}

	~Vector(){

	}

	// return the private information
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }


	Vector& operator=(const Vector &v) {
		if (this == &v)
			return *this;
		else {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
	}

	string str() {
		stringstream buffer;
		buffer << "Vector(" << x << ", " << y << ", " << z << ")";
		return buffer.str();
	}

	bool operator==(const Vector &v) {

		if (this == &v)
			return true;
		else
			return (fabs(x - v.x) < ACCURACY && fabs(y - v.y) < ACCURACY && fabs(z - v.z) < ACCURACY);
	}

	bool operator!=(const Vector &v) const {
		if (this == &v)
			return false;
		else
			return !(fabs(x - v.x) < ACCURACY && fabs(y - v.y) < ACCURACY && fabs(z - v.z) < ACCURACY);
	}

	Vector operator+(const Vector &v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector operator-(const Vector &v) const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector operator-() const {
		return Vector(-x, -y, -z);
	}

	Vector operator*(const double &c) const {
		return Vector(x * c, y * c, z * c);
	}

	Vector operator/(const double &c) const {
		return Vector(x / c, y / c, z / c);
	}

	bool hasSameDirectionWith(const Vector &v) const {
		double k = 1.0;
		if (x != 0 && v.x != 0)
			k = v.x / x;
		else if (y != 0 && v.y != 0)
			k = v.y / y;
		else if (z != 0 && v.z != 0)
			k = v.z / z;
		if (k < 0)
			return false;
		else
			return ((*this) * k == v);
	}

	double getRatio(const Vector &v) {
		double k = 0.0;
		if (x != 0 && v.x != 0)
			k = x / v.x;
		else if (y != 0 && v.y != 0)
			k = y / v.y;
		else if (z != 0 && v.z != 0)
			k = z / v.z;

		if ((*this) == v * k)
			return k;
		else
			return 0.0;
	}

	double length() {
		return sqrt(x * x + y * y + z * z);
	}

	void normalize() {
		double l = length();
		// TODO when l is 0
		x = x / l;
		y = y / l;
		z = z / l;
	}

	double dot(const Vector& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vector cross(const Vector& v) const {
		Vector vector;
		double x1 = (y * v.z - z * v.y);
		double y1 = (z * v.x - x * v.z);
		double z1 = (x * v.y - y * v.x);
		vector.x = x1; vector.y = y1; vector.z = z1;
		return vector;
	}

};

#endif