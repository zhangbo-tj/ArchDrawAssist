#pragma once

#include "Face.h"
#include "Vertex.h"
#include "PlaneFace.h"
#include "EnrolledSlice.h"
#include "vertexHull.h"

#include <vector>

using namespace std;

/**
	三维模型被切分平面分割后的Slice，对每一个Slice展开后得到平面上的Slice，
	成员为三角面片集合，以及相关的辅助展开的参数
*/

class Slice
{
public:
	vector<Face> faces_;	//Slice内的三角面片集合
	double lower_point_, upper_point_;	//Y值上限和下限
	double lower_radius_, higher_radius_;	//下截面的半径和上截面的半径
	double height_, H_;	//拟合为圆台后的相关参数
	double min_radius_, max_radius_;
	bool is_positive_;	//拟合得到的圆台是正立的还是倒立的

public:
	Slice(const double minvalue,const double maxvalue,const double minradius,const double highradius);
	~Slice();

	double MaxRadius() { return max_radius_; }
	const double MaxRadius() const { return max_radius_; }
	double MinRadius() { return min_radius_; }
	const double MinRadius()const { return min_radius_; }


	bool ContainVertex(const Vertex& vertex)const ;	//判断顶点是否位于Slice内
	bool ContainFace(const Face& face)const;	//判断三角面片是否位于Slice内
	int FacesSize() const;	//获取三角面片的数量

	void AddFace(const Face& face);	//添加三角面片

	bool IsRight() const;	//获取拟合后的圆台是否为正立的

	vector<Face> Faces();	//获取三角面片集合

	/**
		按照圆台方式展开三角面片
	*/
	EnrolledSlice EnrollSliceAsConial();	//展开Slice
	void ConialFrustumFitting();	//拟合为圆台
	//按照圆台方式展开三角面片
	PlaneFace ExpandFaceAsConial(const Face& face, const  double height, const double R,
		const  double H, const  bool flag);


	/**
		按照圆柱方式展开三角面片
	*/
	EnrolledSlice EnrollSliceAsCylinder();
	double CylinderFitting();	//拟合为圆柱，返回圆柱体的半径
	PlaneFace ExpandFaceAsCylinder(const Face& face,const double radius);	//按照圆柱方式展开三角面片

};

