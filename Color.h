/**
* 
* @author sean.7
* @date 1/24/2015
* @about class Color for representing color attribute for any class. RGB method of representation is followed.
*
*/
#ifndef COLOR_H
#define COLOR_H

#include<string>
#include<sstream>
#include<math.h>

using namespace std;
class Color{
public:
	double r, g, b;
	// define the constructors
	Color(){
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
	Color(const double &R, const double &G, const double &B){
		r = R;
		if (r<0.0) 
			r = 0.0;
		else if (r>1.0) 
			r = 1.0;

		g = G;
		if (g<0.0) 
			g = 0.0;
		else if (g>1.0) 
			g = 1.0;

		b = B;
		if (b<0.0) 
			b = 0.0;
		else if (b>1.0) 
			b = 1.0;
	}
	Color(const Color& c){
		r = c.r;
		b = c.b;
		g = c.g;
	}
	~Color(){}

	// assignment operators

	Color& operator=(const Color& c) {
		if (this == &c) 
			return *this;
		else{
			r = c.r;
			g = c.g;
			b = c.b;
			return *this;
		}
	}
	//Color& operator+ (const Color& c);
	// outputting format
	string str() {
		stringstream buffer;
		buffer << "Color(" << r << ", " << g << ", " << b << ")";
		return buffer.str();
	}

	Color operator*(const double &k) const {
		return Color(k*r, k*g, k*b);
	}

	Color operator*(const Color &c) const {
		return Color(c.r*r, c.g*g, c.b*b);
	}

	Color operator+(const Color &c) const {
		return Color(r+c.r, g+c.g, b+c.b);
	}

	bool operator==(Color& c) {
		if (this == &c)
			return true;
		else
			return (r == c.r && g == c.g && b == c.b);
	}
	bool operator!=(Color& c) {
		if (this == &c)
			return false;
		else
			return !(r == c.r && g == c.g && b == c.b);
	}
	//void alter(double f);
};
#endif