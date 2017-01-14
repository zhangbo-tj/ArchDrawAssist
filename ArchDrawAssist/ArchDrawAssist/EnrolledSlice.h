#pragma once

#include "PlaneFace.h"
#include "gl/GLAUX.H"
#include <vector>
#include <QDebug>

using namespace std;
class EnrolledSlice
{
public:
	EnrolledSlice(const vector<PlaneFace>&faces) 
		:faces_(faces){
		UpdateBoundary();
	}
	~EnrolledSlice() {
	}
	void UpdateBoundary() {
		double maxX =- 100;
		double maxY = -100;
		double minX = 100;
		double minY = 100;
		for (auto i = 0; i < faces_.size(); i++) {
			PlaneFace tempPlaneFace = faces_[i];
			minX = minX < tempPlaneFace.vertexA_.X() ? minX : tempPlaneFace.vertexA_.X();
			minX = minX < tempPlaneFace.vertexB_.X() ? minX : tempPlaneFace.vertexB_.X();
			minX = minX < tempPlaneFace.vertexC_.X() ? minX : tempPlaneFace.vertexC_.X();

			minY = minY < tempPlaneFace.vertexA_.Y() ? minY : tempPlaneFace.vertexA_.Y();
			minY = minY < tempPlaneFace.vertexB_.Y() ? minY : tempPlaneFace.vertexB_.Y();
			minY = minY < tempPlaneFace.vertexC_.Y() ? minY : tempPlaneFace.vertexC_.Y();

			maxX = maxX > tempPlaneFace.vertexA_.X() ? maxX : tempPlaneFace.vertexA_.X();
			maxX = maxX > tempPlaneFace.vertexB_.X() ? maxX : tempPlaneFace.vertexB_.X();
			maxX = maxX > tempPlaneFace.vertexC_.X() ? maxX : tempPlaneFace.vertexC_.X();

			maxY = maxY > tempPlaneFace.vertexA_.Y() ? maxY : tempPlaneFace.vertexA_.Y();
			maxY = maxY > tempPlaneFace.vertexB_.Y() ? maxY : tempPlaneFace.vertexB_.Y();
			maxY = maxY > tempPlaneFace.vertexC_.Y() ? maxY : tempPlaneFace.vertexC_.Y();
		}
		minx_ = minX;
		miny_ = minY;
		maxx_ = maxX;
		maxy_ = maxY;
	}
	void DrawSlice() {
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		//glDisable(GL_LINE_STIPPLE);
		glBegin(GL_TRIANGLES);
		//glColor3f(1, 0, 0);
		for (int i = 0; i < faces_.size(); i++) {
			PlaneFace tri = faces_[i];
			glTexCoord2d(tri.textureA_.X(),tri.textureA_.Y());
			glVertex2d(tri.vertexA_.X(),tri.vertexA_.Y());
			glTexCoord2d(tri.textureB_.X(), tri.textureB_.Y());
			glVertex2d(tri.vertexB_.X(), tri.vertexB_.Y());
			glTexCoord2d(tri.textureC_.X(), tri.textureC_.Y());
			//qDebug() << tri.textureA_.X() << " " << tri.textureB_.Y();
			glVertex2d(tri.vertexC_.X(), tri.vertexC_.Y());
		}
		glEnd();

	}

	void Translate(double offsetX, double offsetY) {
		for (int i = 0; i < faces_.size(); i++) {
			faces_[i].Translate(offsetX, offsetY);
		}
		minx_ += offsetX;
		miny_ += offsetY;
		maxx_ += offsetX;
		maxy_ += offsetY;
	}
	double LowerBound() {
		return miny_;
	}
	double UpperBound() {
		return maxy_;
	}
public:
	vector<PlaneFace> faces_;
	double minx_, miny_;
	double maxx_, maxy_;
};

