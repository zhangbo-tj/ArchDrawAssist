#pragma once

#include "PlaneFace.h"
#include "PlanePoint.h"
#include "gl/GLAUX.H"
#include <vector>
#include <QDebug>

using namespace std;



/**
	����Ϊ����άģ��չ����õ��Ķ�άƬ�Σ���������Ҫ�ĳ�ԱΪ��άƽ���ϵ�������Ƭ����
	��Ҫ����չʾչ�����Ƭ�Ρ�
	���������Ŀ�ľ���Ϊ���Ժ��ܹ����õص���չ����Ƭ��֮������λ�ã�ʹ����Ӻ���
	Ŀǰ���õ��Ǽ���ÿһ��չ��Ƭ�ε�bounding box,���Ժ�������ڼ���λ�ã���������
	���û��ֶ�����λ��
*/
class EnrolledSlice
{
public:

	/**
		���캯��
		����Ϊ��ά��Ƭ����
	*/
	EnrolledSlice(const vector<PlaneFace>&faces) 
		:faces_(faces),boundary_(PlanePoint(0.0,0.0)),center_(PlanePoint(0.0,0.0)),
		min_radius_(0.0),max_radius_(0.0),upright_(true){
		UpdateBoundary();
	}


	/**
		���캯��
		����Ϊ��ά��Ƭ�����Լ���߽硢���ĵ�Ȳ���
		boundary:��άSlice�ı߽��
		center:��ʾ�ö�άSlice�����ĵ�
		minradius:��Сչ���뾶
		maxradius:���չ���뾶
		up����Sliceչ����ķ��������ϻ�������

	*/
	EnrolledSlice(const vector<PlaneFace>& faces, const PlanePoint& boundary,
		const PlanePoint& center,const double minradius,const double maxradius, const bool up) 
		:faces_(faces),boundary_(boundary),center_(center),
		min_radius_(minradius),max_radius_(maxradius),upright_(up){
		UpdateBoundary();
	}
	

	/**
		��������
	*/	~EnrolledSlice() {
	}

	
	/**
		�����Slice�ı߽磨boundary box��
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
		���Ƹ�Slice
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
		�Ըö�άSlice����ƽ��

		offsetX,offsetY�ֱ�Ϊ��X�ᣬY�᷽���ƶ��ľ���
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
		��ȡ��Slice����ز���
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
	vector<PlaneFace> faces_;	//��ά������Ƭ����
	double minx_, miny_;		//��άSlice���±߽�
	double maxx_, maxy_;		//Slice���ϱ߽�
	PlanePoint boundary_;		//Slice�ı߽��
	PlanePoint center_;			//Slice�����ĵ�
	double min_radius_;			//Slice����С�뾶
	double max_radius_;			//Slice�����뾶
	bool upright_;				//Slice�ķ���
};

