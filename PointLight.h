/**
*
* @author sean.7
* @date 1/25/2015
* @about class PointLight defines point light source.
*
*/

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <string>
#include <sstream>
#include "Point.h"
#include "Color.h"

using namespace std;

class PointLight{
public:
	Point point;
	Color color;
	// Defining Constructors for the class
	PointLight(){}

	PointLight(const Point &point, const Color &color) {
		this->point = point;
		this->color = color;
	}

	PointLight(const PointLight& pointLight) {
		point = pointLight.point;
		color = pointLight.color;
	}

	~PointLight(){}

	//Assignment operator
	PointLight& operator=(const PointLight &pointLight) {
		if (this == &pointLight)
			return *this;
		else {
			point = pointLight.point;
			color = pointLight.color;
			return *this;
		}
	}

	//Output Format of the point
	string str() {
		stringstream buffer;
		buffer << "PointLight[" << point.str() << ", " << color.str() << "]";
		return buffer.str();
	}
};


#endif