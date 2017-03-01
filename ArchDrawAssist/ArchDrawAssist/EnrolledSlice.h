#pragma once

#include "PlaneFace.h"
#include "PlanePoint.h"
#include "gl/GLAUX.H"
#include <vector>
#include <QDebug>

using namespace std;



/**
	该类为对三维模型展开后得到的二维片段，其中最重要的成员为二维平面上的三角面片集合
	主要用于展示展开后的片段。
	构建该类的目的就是为了以后能够更好地调整展开后片段之间的相对位置，使其更加合理，
	目前采用的是计算每一个展开片段的bounding box,但以后可以用于计算位置，甚至可以
	由用户手动调整位置
*/
class EnrolledSlice
{
public:

	/**
		构造函数
		参数为二维面片集合
	*/
	EnrolledSlice(const vector<PlaneFace>&faces) 
		:faces_(faces),boundary_(PlanePoint(0.0,0.0)),center_(PlanePoint(0.0,0.0)),
		min_radius_(0.0),max_radius_(0.0),upright_(true){
		UpdateBoundary();
	}


	/**
		构造函数
		参数为二维面片集合以及其边界、中心点等参数
		boundary:二维Slice的边界点
		center:表示该二维Slice的中心点
		minradius:最小展开半径
		maxradius:最大展开半径
		up：该Slice展开后的方向是向上还是向下

	*/
	EnrolledSlice(const vector<PlaneFace>& faces, const PlanePoint& boundary,
		const PlanePoint& center,const double minradius,const double maxradius, const bool up) 
		:faces_(faces),boundary_(boundary),center_(center),
		min_radius_(minradius),max_radius_(maxradius),upright_(up){
		UpdateBoundary();
	}
	

	/**
		析构函数
	*/	~EnrolledSlice() {
	}

	
	/**
		计算该Slice的边界（boundary box）
	*/
	void UpdateBoundary() {
		double maxX =- 100.0;
		double maxY = -100.0;
		double minX = 100.0;
		double minY = 100.0;
		for (auto face : faces_) {
			minX = minX < face.vertexA_.X() ? minX : face.vertexA_.X();
			minX = minX < face.vertexB_.X() ? minX : face.vertexB_.X();
			minX = minX < face.vertexC_.X() ? minX : face.vertexC_.X();

			minY = minY < face.vertexA_.Y() ? minY : face.vertexA_.Y();
			minY = minY < face.vertexB_.Y() ? minY : face.vertexB_.Y();
			minY = minY < face.vertexC_.Y() ? minY : face.vertexC_.Y();

			maxX = maxX > face.vertexA_.X() ? maxX : face.vertexA_.X();
			maxX = maxX > face.vertexB_.X() ? maxX : face.vertexB_.X();
			maxX = maxX > face.vertexC_.X() ? maxX : face.vertexC_.X();

			maxY = maxY > face.vertexA_.Y() ? maxY : face.vertexA_.Y();
			maxY = maxY > face.vertexB_.Y() ? maxY : face.vertexB_.Y();
			maxY = maxY > face.vertexC_.Y() ? maxY : face.vertexC_.Y();
		}
		minx_ = minX;
		miny_ = minY;
		maxx_ = maxX;
		maxy_ = maxY;
	}

	/**
		绘制该Slice
	*/
	void DrawSlice() const{
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		//glDisable(GL_LINE_STIPPLE);
		glBegin(GL_TRIANGLES);
		for (auto& face : faces_) {
			glTexCoord2d(face.TextureA().X(), face.TextureA().Y());
			glVertex2d(face.VertexA().X(), face.VertexA().Y());
			glTexCoord2d(face.TextureB().X(), face.TextureB().Y());
			glVertex2d(face.VertexB().X(), face.VertexB().Y());
			glTexCoord2d(face.TextureC().X(), face.TextureC().Y());
			glVertex2d(face.VertexC().X(), face.VertexC().Y());
		}
		glEnd();

	}

	/**
		对该二维Slice进行平移

		offsetX,offsetY分别为在X轴，Y轴方向移动的距离
	*/
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

	/**
		获取该Slice的相关参数
	*/
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
	vector<PlaneFace> faces_;	//二维三角面片集合
	double minx_, miny_;		//二维Slice的下边界
	double maxx_, maxy_;		//Slice的上边界
	PlanePoint boundary_;		//Slice的边界点
	PlanePoint center_;			//Slice的中心点
	double min_radius_;			//Slice的最小半径
	double max_radius_;			//Slice的最大半径
	bool upright_;				//Slice的方向
};

