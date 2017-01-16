#pragma once

#include "Face.h"
#include "Vertex.h"
#include "PlaneFace.h"
#include "EnrolledSlice.h"
#include "vertexHull.h"

#include <vector>

using namespace std;

/**
	三维模型被切分平面分割后的Slice
*/

class Slice
{
public:
	vector<Face> mFaces;	//Slice内的三角面片集合
	double mLowerPoint, mUpperPoint;	//Y值上限和下限
	double mLowerRadius, mHigherRadius;	//下截面的半径和上截面的半径
	double mHeight, H;	//拟合为圆台后的相关参数
	double mMinRadius, mMaxRadius;
	bool mFlag;	//拟合得到的圆台是正立的还是倒立的

public:
	Slice(const double minvalue,const double maxvalue,const double minradius,const double highradius);
	~Slice();

	double MaxRadius() { return mMaxRadius; }
	const double MaxRadius() const { return mMaxRadius; }
	double MinRadius() { return mMinRadius; }
	const double MinRadius()const { return mMinRadius; }


	bool ContainVertex(const Vertex& vertex)const ;	//判断顶点是否位于Slice内
	bool ContainFace(const Face& face)const;	//判断三角面片是否位于Slice内
	int FacesSize() const;	//获取三角面片的数量

	void AddFace(const Face& face);	//添加三角面片

	bool IsRight() const;	//获取拟合后的圆台是否为正立的

	vector<Face> Faces();	//获取三角面片集合
	EnrolledSlice EnrollSlice();	//展开Slice
	void ConialFrustumFitting();	//拟合为圆台
	//按照圆台方式展开三角面片
	PlaneFace ExpandAsConial(const Face& face, const  double height, const double R, const  double H, const  bool flag);
};

