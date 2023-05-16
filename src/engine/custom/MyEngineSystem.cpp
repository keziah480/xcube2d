#include "MyEngineSystem.h"
#include <fstream>

float MyEngineSystem::GetArea(float t1x, float t1y, float t2x, float t2y, float t3x, float t3y) {
	return abs((t1x * (t2y - t3y) + t2x * (t3y - t1y) + t3x * (t1y - t2y)) / 2.0);
}

bool MyEngineSystem::IsPointInTriangle(float px, float py, float t1x, float t1y, float t2x, float t2y, float t3x, float t3y) {
	
	/* Calculate area of triangle ABC */
	float A = GetArea(t1x, t1y, t2x, t2y, t3x, t3y);

	/* Calculate area of triangle PBC */
	float A1 = GetArea(px, py, t2x, t2y, t3x, t3y);

	/* Calculate area of triangle PAC */
	float A2 = GetArea(t1x, t1y, px, py, t3x, t3y);

	/* Calculate area of triangle PAB */
	float A3 = GetArea(t1x, t1y, t2x, t2y, px, py);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A >= A1 + A2 + A3);
}

Vertex::Vertex() {
	x = 0;
	y = 0;
	z = 0;

	xO = x;
	yO = y;
	zO = z;
}

Vertex::Vertex(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;

	xO = x;
	yO = y;
	zO = z;
}

Face::Face() {
	verticies[0] = 0;
	verticies[1] = 0;
	verticies[2] = 0;
	
}

Face::Face(int vertex1Index, int vertex2Index, int vertex3Index, int colorR, int colorG, int colorB)
{
	verticies[0] = vertex1Index;
	verticies[1] = vertex2Index;
	verticies[2] = vertex3Index;
	
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
}

void Face::RenderFace(Pixel**& _screen, std::vector<Vertex> cuboidVerticiesRef, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY, float centerX, float centerY, float centerZ) {
	
	int tDPoints[3][2];
	float averageDepth = (cuboidVerticiesRef[verticies[0]].z + cuboidVerticiesRef[verticies[1]].z + cuboidVerticiesRef[verticies[2]].z + centerZ*3) / 3;

	int faceColor[3] = { color[0], color[1], color[2] };

	float lighting;
	float magnitude;
	float normalLength;
	float lightLength;

	float surfaceNormal[3];
	float vectorU[3];
	float vectorV[3];
	float lightVector[3];
	float dotProduct;

	vectorU[0] = (cuboidVerticiesRef[verticies[1]].x + centerX) - (cuboidVerticiesRef[verticies[0]].x + centerX);
	vectorU[1] = (cuboidVerticiesRef[verticies[1]].y + centerY) - (cuboidVerticiesRef[verticies[0]].y + centerY);
	vectorU[2] = (cuboidVerticiesRef[verticies[1]].z + centerZ) - (cuboidVerticiesRef[verticies[0]].z + centerZ);

	vectorV[0] = (cuboidVerticiesRef[verticies[2]].x + centerX) - (cuboidVerticiesRef[verticies[0]].x + centerX);
	vectorV[1] = (cuboidVerticiesRef[verticies[2]].y + centerY) - (cuboidVerticiesRef[verticies[0]].y + centerY);
	vectorV[2] = (cuboidVerticiesRef[verticies[2]].z + centerZ) - (cuboidVerticiesRef[verticies[0]].z + centerZ);

	surfaceNormal[0] = (vectorU[1] * vectorV[2]) - (vectorU[2] * vectorV[1]);
	surfaceNormal[1] = (vectorU[2] * vectorV[0]) - (vectorU[0] * vectorV[2]);
	surfaceNormal[2] = (vectorU[0] * vectorV[1]) - (vectorU[1] * vectorV[0]);

	dotProduct = surfaceNormal[0] * lightSource[0] + surfaceNormal[1] * lightSource[1] + surfaceNormal[2] * lightSource[2];

	normalLength = sqrt(surfaceNormal[0] * surfaceNormal[0] + surfaceNormal[1] * surfaceNormal[1] + surfaceNormal[2] * surfaceNormal[2]);

	lightLength = sqrt(lightSource[0] * lightSource[0] + lightSource[1] * lightSource[1] + lightSource[2] * lightSource[2]);

	lighting = dotProduct / (normalLength * lightLength);
	
	if (lighting < 0) lighting = 0;

	float lightingBonus = (1 - lighting) * 0.2f;

	lighting += lightingBonus;
	
	faceColor[0] *= lighting;
	faceColor[1] *= lighting;
	faceColor[2] *= lighting;

	float x2 = _eye.x;
	float y2 = _eye.y;
	float z2 = _eye.z;

	int minX = 800;
	int maxX = 0;
	int minY = 600;
	int maxY = 0;

	for (int i = 0; i < 3; i++) {

		float x1 = cuboidVerticiesRef[verticies[i]].x + centerX;
		float y1 = cuboidVerticiesRef[verticies[i]].y + centerY;
		float z1 = cuboidVerticiesRef[verticies[i]].z + centerZ;

		float l = (x2 - x1);
		float m = (y2 - y1);
		float n = (z2 - z1);

		float z = _eye.z + _fov;

		tDPoints[i][0] = ((((z - z1) / n) * l) + x1) * 200 + _offsetX;
		tDPoints[i][1] = ((((z - z1) / n) * m) + y1) * 200 + _offsetY;

		if (tDPoints[i][0] < minX) minX = tDPoints[i][0];
		if (tDPoints[i][0] > maxX) maxX = tDPoints[i][0];
		if (tDPoints[i][1] < minY) minY = tDPoints[i][1];
		if (tDPoints[i][1] > maxY) maxY = tDPoints[i][1];

	}

	// Backface culling
	if ((tDPoints[0][0] * tDPoints[1][1] - tDPoints[0][1] * tDPoints[1][0]) + (tDPoints[1][0] * tDPoints[2][1] - tDPoints[1][1] * tDPoints[2][0]) + (tDPoints[2][0] * tDPoints[0][1] - tDPoints[2][1] * tDPoints[0][0]) > 0) return;

	bool is0to1Straight = false;
	bool is1to2Straight = false;
	bool is2to0Straight = false;

	float line0to1M, line0to1C;
	float line1to2M, line1to2C;
	float line2to0M, line2to0C;

	if (tDPoints[0][0] - tDPoints[1][0] == 0) is0to1Straight = true;
	else {
		line0to1M = float(tDPoints[0][1] - tDPoints[1][1]) / (tDPoints[0][0] - tDPoints[1][0]);
		line0to1C = float(tDPoints[0][1]) - line0to1M * float(tDPoints[0][0]);
	}

	if (tDPoints[1][0] - tDPoints[2][0] == 0) is1to2Straight = true;
	else {
		line1to2M = float(tDPoints[1][1] - tDPoints[2][1]) / (tDPoints[1][0] - tDPoints[2][0]);
		line1to2C = float(tDPoints[1][1]) - line1to2M * float(tDPoints[1][0]);
	}

	if (tDPoints[2][0] - tDPoints[0][0] == 0) is2to0Straight = true;
	else {
		line2to0M = float(tDPoints[2][1] - tDPoints[0][1]) / (tDPoints[2][0] - tDPoints[0][0]);
		line2to0C = float(tDPoints[2][1]) - line2to0M * float(tDPoints[2][0]);
	}
	
	for (int row = 0; row < 600; row++) {

		if (row < minY) row = minY;
		if (row > maxY) break;

		float line0to1I;
		float line1to2I;
		float line2to0I;

		minX = 800;
		maxX = 0;

		if (is0to1Straight) {
			line0to1I = tDPoints[0][0];
			if ((tDPoints[0][1] <= row && row <= tDPoints[1][1]) || (tDPoints[0][1] >= row && row >= tDPoints[1][1])) { maxX = line0to1I; minX = line0to1I; }
		}
		else { 
			line0to1I = roundf(float(row - line0to1C) / line0to1M);
			//line0to1I = (float(row - line0to1C) / line0to1M);

			if ((tDPoints[0][0] <=  line0to1I && tDPoints[1][0] >= line0to1I) || (tDPoints[0][0] >= line0to1I && tDPoints[1][0] <= line0to1I)) { maxX = line0to1I; minX = line0to1I; }
		}

		if (is1to2Straight) {
			line1to2I = tDPoints[1][0];
			if ((tDPoints[1][1] <= row && tDPoints[2][1] >= row) || (tDPoints[1][1] >= row && tDPoints[2][1] <= row)) { 
				if (line1to2I <= minX) minX = line1to2I; 
				if (line1to2I >= maxX) maxX = line1to2I; 
			}
		}

		else { 
			line1to2I = roundf(float(row - line1to2C) / line1to2M);
			//line1to2I = (float(row - line1to2C) / line1to2M);
			if ((tDPoints[1][0] <= line1to2I && tDPoints[2][0] >= line1to2I) || (tDPoints[1][0] >= line1to2I && tDPoints[2][0] <= line1to2I)) { 
				if (line1to2I <= minX) minX = line1to2I; 
				if (line1to2I >= maxX) maxX = line1to2I; 
			}
		}

		if (is2to0Straight) {
			line2to0I = tDPoints[2][0];
			if ((tDPoints[2][1] <= row && tDPoints[0][1] >= row) || (tDPoints[2][1] >= row && tDPoints[0][1] <= row)) { 
				if (line2to0I <= minX) minX = line2to0I; 
				if (line2to0I >= maxX) maxX = line2to0I; 
			}
		}

		else { 
			line2to0I = roundf(float(row - line2to0C) / line2to0M);
			//line2to0I = (float(row - line2to0C) / line2to0M);
			if ((tDPoints[2][0] <= line2to0I && tDPoints[0][0] >= line2to0I) || (tDPoints[2][0] >= line2to0I && tDPoints[0][0] <= line2to0I)) { 
				if (line2to0I <= minX) minX = line2to0I; 
				if (line2to0I >= maxX) maxX = line2to0I; 
			}
		}

		if (minX < 0) minX = 0;
		if (maxX > 800) maxX = 800;

		for (int col = minX; col < maxX; col++) {

			if (averageDepth < _screen[col][row].depth) {

				_screen[col][row].r = faceColor[0];
				_screen[col][row].g = faceColor[1];
				_screen[col][row].b = faceColor[2];

				_screen[col][row].depth = averageDepth;
				
			}
		}
	}
}

Cuboid::Cuboid() {

}

Cuboid::Cuboid(float _centerX, float _centerY, float _centerZ, float length, float width, float depth, int redValue, int greenValue, int blueValue) {

	centerPoint.x = _centerX;
	centerPoint.y = _centerY;
	centerPoint.z = _centerZ;

	// Innitiallise all the verticies 
	cuboidVerticies.push_back(Vertex(-1 * length, -1 * depth, -1 * width));
	cuboidVerticies.push_back(Vertex(1 * length, -1 * depth, -1 * width));
	cuboidVerticies.push_back(Vertex(-1 * length, -1 * depth, 1 * width));
	cuboidVerticies.push_back(Vertex(1 * length, -1 * depth, 1 * width));
	cuboidVerticies.push_back(Vertex(-1 * length, 1 * depth, -1 * width));
	cuboidVerticies.push_back(Vertex(1 * length, 1 * depth, -1 * width));
	cuboidVerticies.push_back(Vertex(-1 * length, 1 * depth, 1 * width));
	cuboidVerticies.push_back(Vertex(1 * length, 1 * depth, 1 * width));

	//// Initialise all the faces 
	faces.push_back(Face(0, 4, 5, redValue, greenValue, blueValue));
	faces.push_back(Face(0, 5, 1, redValue, greenValue, blueValue));
	faces.push_back(Face(1, 5, 7, redValue, greenValue, blueValue));
	faces.push_back(Face(1, 7, 3, redValue, greenValue, blueValue));
	faces.push_back(Face(3, 7, 6, redValue, greenValue, blueValue));
	faces.push_back(Face(3, 6, 2, redValue, greenValue, blueValue));
	faces.push_back(Face(2, 6, 4, redValue, greenValue, blueValue));
	faces.push_back(Face(2, 4, 0, redValue, greenValue, blueValue));
	faces.push_back(Face(0, 3, 2, redValue, greenValue, blueValue));
	faces.push_back(Face(0, 1, 3, redValue, greenValue, blueValue));
	faces.push_back(Face(4, 6, 7, redValue, greenValue, blueValue));
	faces.push_back(Face(4, 7, 5, redValue, greenValue, blueValue));

}

Sphere::Sphere() {

}
Sphere::Sphere(float _centerX, float _centerY, float _centerZ, float radius, int redValue, int greenValue, int blueValue) {

	rad = radius;
	centerPoint.x = _centerX;
	centerPoint.y = _centerY;
	centerPoint.z = _centerZ;

}

Mesh::Mesh() {

}

Mesh::Mesh(float centerX, float centerY, float centerZ, std::string meshName, int redValue, int greenValue, int blueValue) {
	centerPoint = Vertex(centerX, centerY, centerZ);
	
	std::string fileName = "H:/Rendering_Project/xcube/build/Debug/";
	fileName = fileName+meshName;

	std::ifstream file(fileName);

	if (!file.is_open()) {
		std::cerr << "Open Error." << std::endl;
	}

	std::string line;
	while (std::getline(file, line)) {
		// If the line is for a new vertex
		if (line[0] == 'v' && line[1] == ' ') { 
			std::string vertexNum1 = "";
			std::string vertexNum2 = "";
			std::string vertexNum3 = "";

			int curChar = 2;
			
			// Append current char
			while (curChar < line.length() && line[curChar] != ' ') {
				vertexNum1 += line[curChar];
				curChar++;
			}
			curChar++;

			while (curChar < line.length() && line[curChar] != ' ') {
				vertexNum2 += line[curChar];
				curChar++;
			}
			curChar++;

			while (curChar < line.length() && line[curChar] != ' ') {
				vertexNum3 += line[curChar];
				curChar++;
			}
			curChar++;

			meshVerticies.push_back(Vertex(std::stof(vertexNum1), std::stof(vertexNum2), std::stof(vertexNum3)));

		}
		if (line[0] == 'f' && line[1] == ' ') { 
			std::string vertexIndex1 = "";
			std::string vertexIndex2 = "";
			std::string vertexIndex3 = "";

			int curChar = 2;

			while (curChar < line.length() && line[curChar] != ' ' && line[curChar] != '/') {
				vertexIndex1 += line[curChar];
				curChar++;
			}
			while (curChar < line.length() && line[curChar] != ' ') {
				curChar++;
			}

			curChar++;

			while (curChar < line.length() && line[curChar] != ' ' && line[curChar] != '/') {
				vertexIndex2 += line[curChar];
				curChar++;
			}
			while (curChar < line.length() && line[curChar] != ' ') {
				curChar++;
			}
			curChar++;

			while (curChar < line.length() && line[curChar] != ' ' && line[curChar] != '/') {
				vertexIndex3 += line[curChar];
				curChar++;
			}

			if (vertexIndex1 != ""&& vertexIndex2 != ""&& vertexIndex3 != "") meshFaces.push_back(Face(stoi(vertexIndex1)-1, stoi(vertexIndex2) - 1, stoi(vertexIndex3) - 1, redValue, greenValue, blueValue));

		}
	}
}

void Cuboid::RenderMesh(Pixel **&_screen, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY) {
	if (centerPoint.z < -9) return;
	for (int i = 0; i < 12; i++) {
		faces[i].RenderFace(_screen, cuboidVerticies, lightSource, _eye, _fov, _offsetX, _offsetY, centerPoint.x, centerPoint.y, centerPoint.z);
	}
}

void Sphere::RenderMesh(Pixel**& _screen, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY) {
	if (centerPoint.z < -9) return;
	for (int i = 0; i < 200; i++) {
		faces[i].RenderFace(_screen, sphereVerticies, lightSource, _eye, _fov, _offsetX, _offsetY, centerPoint.x, centerPoint.y, centerPoint.z);
	}
}

void Mesh::RenderMesh(Pixel**& _screen, float lightSource[3], Vertex _eye, float _fov, float _offsetX, float _offsetY) {
	for (int i = 0; i < meshFaces.size(); i++) {
		meshFaces[i].RenderFace(_screen, meshVerticies, lightSource, _eye, _fov, _offsetX, _offsetY, centerPoint.x, centerPoint.y, centerPoint.z);
	}
}

void Sphere::UpdatePosition() {
	yVel += 0.06;
	if (centerPoint.y + rad > 1) { if (abs(yVel) < 0.13) yVel = 0; yVel = -abs(yVel) * 0.7; }
	TranslateSphere(xVel, yVel, zVel);
}

void Cuboid::TranslateCuboid(float xVel, float yVel) {
	centerPoint.x += xVel;
	centerPoint.y += yVel;
}

void Sphere::TranslateSphere(float xVel, float yVel, float zVel) {
	centerPoint.x += xVel;
	centerPoint.y += yVel;
	centerPoint.z += zVel;
}
void Mesh::TranslateMesh(float xVel, float yVel, float zVel) {
	centerPoint.x += xVel;
	centerPoint.y += yVel;
	centerPoint.z += zVel;
}

void Cuboid::RotateCuboid(int axis, float theta) {
	for (int i = 0; i < 8; i++) {
		switch (axis) {
		case(0):
			cuboidVerticies[i].xRot += theta;
			break;
		case(1):
			cuboidVerticies[i].yRot += theta;
			break;
		case(2):
			break;
		}

		cuboidVerticies[i].x = cuboidVerticies[i].xO * cos(cuboidVerticies[i].yRot) + cuboidVerticies[i].yO * sin(cuboidVerticies[i].xRot) * sin(cuboidVerticies[i].yRot) + cuboidVerticies[i].zO * cos(cuboidVerticies[i].xRot) * sin(cuboidVerticies[i].yRot);
		cuboidVerticies[i].y = cuboidVerticies[i].yO * cos(cuboidVerticies[i].xRot) + cuboidVerticies[i].zO * -sin(cuboidVerticies[i].xRot);
		cuboidVerticies[i].z = cuboidVerticies[i].xO * -sin(cuboidVerticies[i].yRot) + cuboidVerticies[i].yO * cos(cuboidVerticies[i].yRot) * sin(cuboidVerticies[i].xRot) + cuboidVerticies[i].zO * cos(cuboidVerticies[i].yRot) * cos(cuboidVerticies[i].xRot);
	}

}

void Sphere::RotateSphere(int axis, float theta) {
	for (int i = 0; i < 121; i++) {
		switch (axis) {
		case(0):
			sphereVerticies[i].xRot += theta;
			break;
		case(1):
			sphereVerticies[i].yRot += theta;
			break;
		case(2):
			break;
		}

		sphereVerticies[i].x = sphereVerticies[i].xO * cos(sphereVerticies[i].yRot) + sphereVerticies[i].yO * sin(sphereVerticies[i].xRot) * sin(sphereVerticies[i].yRot) + sphereVerticies[i].zO * cos(sphereVerticies[i].xRot) * sin(sphereVerticies[i].yRot);
		sphereVerticies[i].y = sphereVerticies[i].yO * cos(sphereVerticies[i].xRot) + sphereVerticies[i].zO * -sin(sphereVerticies[i].xRot);
		sphereVerticies[i].z = sphereVerticies[i].xO * -sin(sphereVerticies[i].yRot) + sphereVerticies[i].yO * cos(sphereVerticies[i].yRot) * sin(sphereVerticies[i].xRot) + sphereVerticies[i].zO * cos(sphereVerticies[i].yRot) * cos(sphereVerticies[i].xRot);
	}
}

void Mesh::RotateMesh(int axis, float theta) {
	for (int i = 0; i < meshVerticies.size(); i++) {
		switch (axis) {
		case(0):
			meshVerticies[i].xRot += theta;
			break;
		case(1):
			meshVerticies[i].yRot += theta;
			break;
		case(2):
			break;
		}

		meshVerticies[i].x = meshVerticies[i].xO * cos(meshVerticies[i].yRot) + meshVerticies[i].yO * sin(meshVerticies[i].xRot) * sin(meshVerticies[i].yRot) + meshVerticies[i].zO * cos(meshVerticies[i].xRot) * sin(meshVerticies[i].yRot);
		meshVerticies[i].y = meshVerticies[i].yO * cos(meshVerticies[i].xRot) + meshVerticies[i].zO * -sin(meshVerticies[i].xRot);
		meshVerticies[i].z = meshVerticies[i].xO * -sin(meshVerticies[i].yRot) + meshVerticies[i].yO * cos(meshVerticies[i].yRot) * sin(meshVerticies[i].xRot) + meshVerticies[i].zO * cos(meshVerticies[i].yRot) * cos(meshVerticies[i].xRot);
	}

}

void Cuboid::SetColor(int redValue, int greenValue, int blueValue) {
	for (int i = 0; i < 12; i++) {
		faces[i].color[0] = redValue;
		faces[i].color[1] = greenValue;
		faces[i].color[2] = blueValue;
	}

}