#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Ray.h"
#include "PointLight.h"
#include "Sphere.h"

using namespace std;

const int RESOLUTION_WIDTH = 400;
const int RESOLUTION_HEIGHT = 300;

// 点光源
vector<PointLight> pointLights;
// 视点
Point viewPoint(1, 1, 5);
Vector viewVector(0, 0, -2);
double screenResolutionRatio = 0.01;
double screenWdith = RESOLUTION_WIDTH * screenResolutionRatio;
double screenHeight = RESOLUTION_HEIGHT * screenResolutionRatio;
// 环境及phong模型参数
Color backgroundColor(0, 0, 0);
double k_a = 0.2;
Color I_a(1.0, 1.0, 1.0);
double k_d = 0.7;
double k_s = 0.5;
int n_s = 6;
// frame buffer
Color frameBuffer[RESOLUTION_WIDTH][RESOLUTION_HEIGHT];
// objects
vector<Object*> objects;
Sphere sphere(Point(1.0, 1.0, 1.0), 1.0, Color(1.0, 0.0, 0.0), Color(1.0, 1.0, 1.0));

void init() {
	pointLights.push_back(PointLight(Point(5, 20, 5), Color(1.0, 1.0, 1.0)));
	//pointLights.push_back(PointLight(Point(20, 20, 20), Color(1.0, 1.0, 1.0)));
	//for (auto point : pointLights) {
	//	cout << point.str() << endl;
	//}
	// 初始化 objects
	objects.push_back(&sphere);
	//cout << sphere.getNormal(Ray(Point(1, 1, 50), Vector(0, 0, -1), backgroundColor)).str() << endl;

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
			// TODO shadow
			double S = 1.0;
			// TODO f_att
			double f_att = 1.0;
			Color I_p = pointLight.color;
			Vector N = ptrClosestObject->getNormal(ray);
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
			Ray ray(viewPoint, rayVector, backgroundColor);

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