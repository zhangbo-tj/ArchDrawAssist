#pragma once

#include "PlaneFace.h"
#include "PlanePoint.h"
#include "gl/GLAUX.H"
#include <vector>
#include <QDebug>

using namespace std;
class EnrolledSlice
{
public:
	EnrolledSlice(const vector<PlaneFace>&faces) 
		:faces_(faces),boundary_(PlanePoint(0.0,0.0)),center_(PlanePoint(0.0,0.0)),
		min_radius_(0.0),max_radius_(0.0),upright_(true){
		UpdateBoundary();
	}
	EnrolledSlice(const vector<PlaneFace>& faces, const PlanePoint& boundary,
		const PlanePoint& center,const double minradius,const double maxradius, const bool up) 
		:faces_(faces),boundary_(boundary),center_(center),
		min_radius_(minradius),max_radius_(maxradius),upright_(up){
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
	void DrawSlice() const{
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
		boundary_.Translate(offsetX, offsetY);
		center_.Translate(offsetX, offsetY);
		minx_ += offsetX;
		miny_ += offsetY;
		maxx_ += offsetX;
		maxy_ += offsetY;
	}
	double LowerBound() const{
		return miny_;
	}
	double UpperBound() const{
		return maxy_;
	}
	PlanePoint Boundary() const{
		return boundary_;
	}
	PlanePoint Center() const {
		return center_;
	}
	bool IsRight()const {
		return upright_;
	}
	double MinRadius()const{
		return min_radius_;
	}
	double MaxRadius()const {
		return max_radius_;
	}

public:
	vector<PlaneFace> faces_;
	double minx_, miny_;
	double maxx_, maxy_;
	PlanePoint boundary_;
	PlanePoint center_;
	double min_radius_;
	double max_radius_;
	bool upright_;
};

