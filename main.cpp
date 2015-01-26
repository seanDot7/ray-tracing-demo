#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Ray.h"
#include "PointLight.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"

using namespace std;

const int RESOLUTION_WIDTH = 400;
const int RESOLUTION_HEIGHT = 300;

// 点光源
vector<PointLight> pointLights;
// 视点
Point viewPoint(5, 8, 10);
Vector viewVector(0, -1, -1);
double screenResolutionRatio = 0.01;
double screenWdith = RESOLUTION_WIDTH * screenResolutionRatio;
double screenHeight = RESOLUTION_HEIGHT * screenResolutionRatio;
// 环境及phong模型参数
Color backgroundColor(0, 0, 0);
double k_a = 0.2;
Color I_a(1.0, 1.0, 1.0);
double k_d = 0.7;
double k_s = 0.5;
int n_s = 10;
double f_att_c1 = 0.01;
double f_att_c2 = 0.15;
double f_att_c3 = 0.01;
// frame buffer
Color frameBuffer[RESOLUTION_WIDTH][RESOLUTION_HEIGHT];
// objects
vector<Object*> objects;
Sphere sphere(Point(3.0, 3.0, 7.0), 1.0, Color(0.97, 0.26, 0.31), Color(1.0, 1.0, 1.0));
Plane plane(Point(0, 0, 0), Point(10, 0, 0), Point(10, 0, 10), Color(0.60, 0.66, 0.94), Color(0.0, 0.0, 0.0));

Point tempP1(6, 2, 6), tempP2(6, 2, 7), tempP3(7, 2, 7), tempP4(7, 2, 6),
	  tempP5(6, 4, 6), tempP6(6, 4, 7), tempP7(7, 4, 7), tempP8(7, 4, 6);
Color tempCubeColor = Color(0.84, 0.84, 0.84);
Plane tempPlanes[6] = { Plane(tempP1, tempP2, tempP3, tempCubeColor), 
						Plane(tempP5, tempP8, tempP7, tempCubeColor),
						Plane(tempP6, tempP7, tempP3, tempCubeColor),
						Plane(tempP7, tempP8, tempP4, tempCubeColor),
						Plane(tempP8, tempP5, tempP1, tempCubeColor),
						Plane(tempP5, tempP6, tempP2, tempCubeColor),
					  };
Cube cube(tempPlanes, tempCubeColor);
Cylinder cylinder(Point(5, 2, 3), Point(5, 4, 3), 1, Color(0.87, 0.70, 0.97));


void init() {
	pointLights.push_back(PointLight(Point(7, 20, 7), Color(1.0, 1.0, 1.0)));
	pointLights.push_back(PointLight(Point(0, 20, 0), Color(1.0, 1.0, 1.0)));
	//for (auto point : pointLights) {
	//	cout << point.str() << endl;
	//}
	// 初始化 objects
	objects.push_back(&sphere);
	objects.push_back(&plane);
	objects.push_back(&cube);
	objects.push_back(&cylinder);
	//cout << sphere.getNormal(Ray(Point(1, 1, 50), Vector(0, 0, -1), backgroundColor)).str() << endl;

} 
 
double min(const double &a, const double &b) {
	if (a < b)
		return a;
	else
		return b;
}

Color rayTrace(Ray ray, int level) {
	// 获得最近的相交的物体
	bool flagIntersected = false;
	Point closestPoint;
	Object *ptrClosestObject;
	for (auto ptrObject : objects) {
		if (ptrObject->isIntersected(ray)) {
			if (!flagIntersected) {
				closestPoint = ptrObject->getClosestIntersectedPoint(ray);
				ptrClosestObject = ptrObject;
				flagIntersected = true;
			} else {
				Point tempPoint = ptrObject->getClosestIntersectedPoint(ray);
				if ((closestPoint - ray.origin).length() > (tempPoint - ray.origin).length()) {
					closestPoint = tempPoint;
					ptrClosestObject = ptrObject;
				}
			}
		}
	}

	// shading
	if (!flagIntersected)
		return backgroundColor;
	else {
		Color O_d = ptrClosestObject->color;
		Color O_s = ptrClosestObject->specularColor;
		Color returnColor = I_a * k_a * O_d; 
		
		for (auto pointLight : pointLights) {
			Vector N = ptrClosestObject->getNormal(ray);
			// TODO shadow
			double S = 1.0;
			Vector vectorObjectLight = pointLight.point - closestPoint;
			if (vectorObjectLight.dot(N) >= 0) {
				Ray tempRay(closestPoint, vectorObjectLight, Color(0, 0, 0));
				bool flagIntersected = false;
				for (auto ptrObject : objects) {
					if (ptrObject != ptrClosestObject && ptrObject->isIntersected(tempRay)) {
						flagIntersected = true;
						break;
					}
				}
				if (flagIntersected)
					S = 0.0;
			}

			double dist = (ray.origin - closestPoint).length();
			double f_att = min(1.0 / (f_att_c1 + f_att_c2 * dist + f_att_c3 * dist * dist), 1.0);
			//cout << f_att << endl;

			Color I_p = pointLight.color;

			Vector L = pointLight.point - closestPoint;
			L.normalize();
			double dotProductOfNandL = N.dot(L);
			double dotProductOfRandV = 0.0;
			if (dotProductOfNandL < 0)
				dotProductOfNandL = 0;

			else {
				// specular light
				Vector V = -ray.direction;
				V.normalize();
				Vector R = N * 2.0 * dotProductOfNandL - L;
				dotProductOfRandV = R.dot(V);
				if (dotProductOfRandV < 0)
					dotProductOfRandV = 0;
			}
			
			returnColor = returnColor + I_p * (O_d * dotProductOfNandL * k_d + O_s * k_s * pow(dotProductOfRandV, n_s)) * S * f_att;
			

		}


		// TODO reflection
		// TODO refraction

		return returnColor;
	}
}

void render() {
	Vector vectorW(-viewVector.z, 0, viewVector.x), vectorH;
	vectorW.normalize();
	//cout << vectorW.str() << endl;
	vectorH = vectorW.cross(viewVector);
	vectorH.normalize();
	//cout << vectorH.str() << endl;

	for (int resX = -RESOLUTION_WIDTH / 2; resX < RESOLUTION_WIDTH / 2; ++resX) {
		for (int resY = -RESOLUTION_HEIGHT / 2; resY < RESOLUTION_HEIGHT / 2; ++resY) {
			double screenX = resX * screenResolutionRatio;
			double screenY = resY * screenResolutionRatio;
			//cout << screenX << ", " << screenY << endl;
			frameBuffer[resX + RESOLUTION_WIDTH / 2][resY + RESOLUTION_HEIGHT / 2] = backgroundColor;
			Vector screenVector(vectorW * screenX + vectorH * screenY);
			Point screenPoint(viewPoint + viewVector + screenVector);
			Vector rayVector(screenPoint - viewPoint);
			// perpective tracing
			Ray ray(viewPoint, rayVector, backgroundColor);
			// parallel tracing
			//Ray ray(screenPoint, viewVector, backgroundColor);
			//cout << screenPoint.str() << endl;
			//cout << ray.str() << endl;
			// ray tracing
			frameBuffer[resX + RESOLUTION_WIDTH / 2][resY + RESOLUTION_HEIGHT / 2] = rayTrace(ray, 1);

		}

	}

}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glPointSize(2.0f); 
	glBegin(GL_POINTS);
	
	for (int i = -RESOLUTION_WIDTH / 2; i < RESOLUTION_WIDTH / 2; i++)
	{	
		for (int j = -RESOLUTION_HEIGHT / 2; j < RESOLUTION_HEIGHT / 2; j++)
		{
			Color tempColor = frameBuffer[i + RESOLUTION_WIDTH / 2][j + RESOLUTION_HEIGHT / 2];
			glColor3f(tempColor.r, tempColor.g, tempColor.b);
			glVertex2f(1.0 * i * 2 / RESOLUTION_WIDTH, 1.0 * j * 2 / RESOLUTION_HEIGHT);
		}
	}
	glEnd();
	glFlush();
}
int main(int argc, char* argv[])
{
	init();
	render();
	// display the rendered 3D picture
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
	glutCreateWindow("Ray Tracing");
	glutDisplayFunc(&Display);
	glutMainLoop();
	return 0;
}