#pragma once

#include "Vertex.h"
#include "Point.h"
#include "Triangle.h"
#include "Normal.h"
#include "Face.h"

/**
	三维模型中的一些相关操作
*/

bool SameSide(const Vertex& A, const Vertex& B, const Vertex& C, const MyPoint& p) {
	bool sameside;
	Vertex AB = B - A;
	Vertex AC = C - A;
	Vertex AP = Vertex(p.X() - A.X(), p.Y() - A.Y(), p.Z() - A.Z());

	Vertex v1 = AB.Cross(AC);
	Vertex v2 = AB.Cross(AP);
	return v1.Dot(v2) >= 0 ? true : false;
}

bool IsInTriangle(const Vertex& A, const Vertex& B, const Vertex& C,const MyPoint& point) {
	return SameSide(A, B, C, point) && SameSide(B, C, A, point) && SameSide(C, A, B, point);
}


bool IsNeighborTri(const Triangle& triangle1, const Triangle triangle2) {
	const int* vindices1 = triangle1.Vindices();
	const int* vindices2 = triangle2.Vindices();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (vindices1[i] == vindices2[j]) {
				return true;
			}
		}
	}
	return false;
}

double Distance(const Vertex& v, const MyPoint& p) {
	return sqrt(
		(v.X() - p.X())*(v.X() - p.X()) +
		(v.Y() - p.Y())*(v.Y() - p.Y()) +
		(v.Z() - p.Z())*(v.Z() - p.Z())
	);
}

int IsVertexUpperPlane(const Vertex& vertexA, double* abcd, const MyPoint& center_point) {
	Vertex plane2Vertex(vertexA.X() - center_point.X(),
		vertexA.Y() - center_point.Y(),
		vertexA.Z() - center_point.Z());
	Vertex planeNormal(abcd[0], abcd[1], abcd[2]);
	const double EPILON = 1.0e-5;

	double dotProduct = plane2Vertex.Dot(planeNormal);
	if (abs(dotProduct) > EPILON) {
		if (dotProduct > 0) {
			return 1;
		}
		else if (dotProduct < 0) {
			return -1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int IsVertexUpperPlane(const Vertex& vertexA, const double yValue) {
	double z = vertexA.Z();
	double EPILON = 1.0e-5;
	if (z > yValue && abs(z - yValue) > EPILON) {
		return 1;
	}
	else if (z < yValue && abs(z - yValue) >EPILON) {
		return -1;
	}
	else {
		return 0;
	}
}


int IsFaceUpperPlane(const Face& face, double* abcd, const MyPoint& center_point) {
	Vertex vertexA = face.VertexA();
	Vertex vertexB = face.VertexB();
	Vertex vertexC = face.VertexC();
	Normal faceNormal(vertexA, vertexB, vertexC);

	double dis[3];
	dis[0] = abcd[0] * vertexA.X() + abcd[1] * vertexA.Y() + abcd[2] * vertexA.Z() + abcd[3];
	dis[1] = abcd[0] * vertexB.X() + abcd[1] * vertexB.Y() + abcd[2] * vertexB.Z() + abcd[3];
	dis[2] = abcd[0] * vertexC.X() + abcd[1] * vertexC.Y() + abcd[2] * vertexC.Z() + abcd[3];

	int upper[3];
	upper[0] = IsVertexUpperPlane(vertexA, abcd, center_point);
	upper[1] = IsVertexUpperPlane(vertexB, abcd, center_point);
	upper[2] = IsVertexUpperPlane(vertexC, abcd, center_point);

	if ((upper[0] > 0 && upper[1] > 0 && upper[2] > 0) ||
		(upper[0] > 0 && upper[1] > 0 && fabs(dis[2]) < 1.0e-8) || (upper[0] > 0 && fabs(dis[1]) < 1.0e-8 && upper[2] > 0) || (fabs(dis[0]) < 1.0e-8 &&upper[1] > 0 && upper[2] > 0) ||
		(upper[0] > 0 && fabs(dis[1]) < 1.0e-8 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 && upper[1] > 0 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 &&fabs(dis[1]) < 1.0e-8&& upper[2] > 0)) {
		return 1;
	}
	//三角面片位于平面下方
	//三个顶点都位于平面下方||两个顶点位于平面下方,一个顶点位于平面上||一个顶点位于平面下方，两个顶点位于平面上
	else if ((upper[0] < 0 && upper[1] < 0 && upper[2] < 0) ||
		(upper[0] < 0 && upper[1] < 0 && fabs(dis[2]) < 1.0e-8) || (upper[0] < 0 && fabs(dis[1]) < 1.0e-8 && upper[2] < 0) || (fabs(dis[0]) < 1.0e-8 &&upper[1] < 0 && upper[2] < 0) ||
		(upper[0] < 0 && fabs(dis[1]) < 1.0e-8 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 && upper[1] < 0 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 &&fabs(dis[1]) < 1.0e-8&& upper[2] < 0)) {
		return -1;
	}
	return 1;
}

int IsFaceUpperPlaneX(const Face& face, const double xValue) {
	double EPSILON = 1.0e-5;

	///A,B,C三个顶点是否位于平面上
	bool freeA = abs(face.VertexA().X() - xValue) > EPSILON ? true : false;
	bool freeB = abs(face.VertexB().X() - xValue) > EPSILON ? true : false;
	bool freeC = abs(face.VertexC().X() - xValue) > EPSILON ? true : false;

	bool upA = freeA && (face.VertexA().X() > xValue ? true : false);
	bool downA = freeA && (face.VertexA().X() < xValue ? true : false);
	bool onA = !freeA;

	bool upB = freeB && (face.VertexB().X() > xValue ? true : false);
	bool downB = freeB && (face.VertexB().X() < xValue ? true : false);
	bool onB = !freeB;


	bool upC = freeC && (face.VertexC().X() > xValue ? true : false);
	bool downC = freeC && (face.VertexC().X() < xValue ? true : false);
	bool onC = !freeC;


	if ((upA&&upB&&upC) ||
		(onA&&onB&&upC) || (onA & onC&& upB) || (onB && onC&&upA) ||
		(onA && upB && upC) || (onB && upA && upC) || (onC && upA && upB)) {
		return 1;
	}
	else if ((downA&&downB&&downC) ||
		(onA&&onB&&downC) || (onA & onC&& downB) || (onB && onC&&downA) ||
		(onA && downB && downC) || (onB && downA && downC) || (onC && downA && downB)) {
		return -1;
	}
	else {
		return 0;
	}
}
int IsVertexUpperYOZ(const Vertex& vertex) {
	if (vertex.X() > 0) {
		return 1;
	}
	else if (vertex.X() < 0) {
		return -1;
	}
	else {
		return 0;
	}
}
int IsFaceUpperYOZ(const Face& face) {
	double dis[3] = {0.0,0.0,0.0};
	int upper[3] = {0,0,0};

	dis[0] = face.VertexA().X();
	dis[1] = face.VertexB().X();
	dis[2] = face.VertexC().X();

	upper[0] = IsVertexUpperYOZ(face.VertexA());
	upper[1] = IsVertexUpperYOZ(face.VertexB());
	upper[2] = IsVertexUpperYOZ(face.VertexC());

	//三角面片位于平面上方
	//三个顶点都位于平面上方||两个顶点位于平面上方,一个顶点位于平面上||一个顶点位于平面上方，两个顶点位于平面上
	if ((upper[0] > 0 && upper[1] > 0 && upper[2] > 0) ||
		(upper[0] > 0 && upper[1] > 0 && fabs(dis[2]) < 1.0e-8) || (upper[0] > 0 && fabs(dis[1]) < 1.0e-8 && upper[2] > 0) || (fabs(dis[0]) < 1.0e-8 &&upper[1] > 0 && upper[2] > 0) ||
		(upper[0] > 0 && fabs(dis[1]) < 1.0e-8 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 && upper[1] > 0 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 &&fabs(dis[1]) < 1.0e-8&& upper[2] > 0)) {
		return 1;
	}
	//三角面片位于平面下方
	//三个顶点都位于平面下方||两个顶点位于平面下方,一个顶点位于平面上||一个顶点位于平面下方，两个顶点位于平面上
	else if ((upper[0] < 0 && upper[1] < 0 && upper[2] < 0) ||
		(upper[0] < 0 && upper[1] < 0 && fabs(dis[2]) < 1.0e-8) || (upper[0] < 0 && fabs(dis[1]) < 1.0e-8 && upper[2] < 0) || (fabs(dis[0]) < 1.0e-8 &&upper[1] < 0 && upper[2] < 0) ||
		(upper[0] < 0 && fabs(dis[1]) < 1.0e-8 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 && upper[1] < 0 && fabs(dis[2]) < 1.0e-8) || (fabs(dis[0]) < 1.0e-8 &&fabs(dis[1]) < 1.0e-8&& upper[2] < 0)) {
		return -1;
	}
	else {
		return 1;
	}
	
}


///判断三角面片是否位于平行于xoz的平面右侧
int IsFaceUpperPlaneY(const Face& face, const double yValue) {
	double EPSILON = 1.0e-8;

	///A,B,C三个顶点是否位于平面上
	bool freeA = abs(face.VertexA().Y() - yValue) > EPSILON ? true : false;
	bool freeB = abs(face.VertexB().Y() - yValue) > EPSILON ? true : false;
	bool freeC = abs(face.VertexC().Y() - yValue) > EPSILON ? true : false;

	bool upA = freeA && (face.VertexA().Y() > yValue);
	bool downA = freeA && (face.VertexA().Y() < yValue);
	bool onA = !freeA;

	bool upB = freeB && (face.VertexB().Y() > yValue);
	bool downB = freeB && (face.VertexB().Y() < yValue);
	bool onB = !freeB;


	bool upC = freeC && (face.VertexC().Y() > yValue);
	bool downC = freeC && (face.VertexC().Y() < yValue);
	bool onC = !freeC;


	if ((upA&&upB&&upC) ||
		(onA&&onB&&upC) || (onA & onC&& upB) || (onB && onC&&upA) ||
		(onA && upB && upC) || (onB && upA && upC) || (onC && upA && upB)) {
		return 1;
	}
	else if ((downA&&downB&&downC) ||
		(onA&&onB&&downC) || (onA & onC&& downB) || (onB && onC&&downA) ||
		(onA && downB && downC) || (onB && downA && downC) || (onC && downA && downB)) {
		return -1;
	}
	else {
		return 0;
	}
}

