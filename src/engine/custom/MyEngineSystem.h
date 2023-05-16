#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"
#include <vector>

class MyEngineSystem {
	friend class XCube2Engine;
	private:

	public:
		static float GetArea(float t1x, float t1y, float t2x, float t2y, float t3x, float t3y);
		static bool IsPointInTriangle(float px, float py, float t1x, float t1y, float t2x, float t2y, float t3x, float t3y);
};

class Pixel
{
public:
	float depth = 99999;
	int r = 0, g = 0, b = 0;
};


class Vertex {
public:
	float x = 0, y = 0, z = 0;
	float xO = 0, yO = 0, zO = 0;
	float xRot = 0, yRot = 0, zRot = 0;
	Vertex();
	Vertex(float _x, float _y, float _z);
};

class Face
{
public:
	int verticies[3];
	int color[3] = { 0, 0, 0 };
	
	Face();
	Face(int vertex1Index, int vertex2Index, int vertex3Index, int colorR, int colorG, int colorB);
	void RenderFace(Pixel **& _screen, std::vector<Vertex> cuboidVerticiesRef, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY, float centerX, float centerY, float centerZ);
};


class Cuboid {
public:
	Vertex centerPoint;
	std::vector<Vertex> cuboidVerticies;
	std::vector<Face> faces;


	Cuboid();
	Cuboid(float centerX, float centerY, float centerZ, float length, float width, float depth, int redValue, int greenValue, int blueValue);
	void RenderMesh(Pixel **& _screen, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY);
	void RotateCuboid(int axis, float theta);
	void TranslateCuboid(float xVel, float yVel);
	void SetColor(int redValue, int greenValue, int blueValue);
};

class Sphere {
public:
	Vertex centerPoint;
	std::vector<Vertex> sphereVerticies;
	std::vector<Face> faces;

	float rad;
	float xVel, yVel, zVel;

	Sphere();
	Sphere(float centerX, float centerY, float centerZ, float radius, int redValue, int greenValue, int blueValue);
	void RenderMesh(Pixel **& _screen, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY);
	void UpdatePosition();
	void RotateSphere(int axis, float theta);
	void TranslateSphere(float xVel, float yVel, float zVel);
	void SetColor(int redValue, int greenValue, int blueValue);
};

class Mesh {
public:
	Vertex centerPoint;

	std::vector<Vertex> meshVerticies;
	std::vector<Face> meshFaces;

	Mesh();
	Mesh(float centerX, float centerY, float centerZ, std::string meshName, int redValue, int greenValue, int blueValue);

	void RenderMesh(Pixel**& _screen, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY);
	void UpdatePosition();
	void RotateMesh(int axis, float theta);
	void TranslateMesh(float xVel, float yVel, float zVel);
	void SetColor(int redValue, int greenValue, int blueValue);
};

#endif