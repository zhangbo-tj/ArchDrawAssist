#pragma once

#include "Face.h"
#include "Vertex.h"
#include "PlaneFace.h"
#include "EnrolledSlice.h"
#include "vertexHull.h"

#include <vector>

using namespace std;

/**
	��άģ�ͱ��з�ƽ��ָ���Slice����ÿһ��Sliceչ����õ�ƽ���ϵ�Slice��
	��ԱΪ������Ƭ���ϣ��Լ���صĸ���չ���Ĳ���
*/

class Slice
{
public:
	vector<Face> faces_;	//Slice�ڵ�������Ƭ����
	double lower_point_, upper_point_;	//Yֵ���޺�����
	double lower_radius_, higher_radius_;	//�½���İ뾶���Ͻ���İ뾶
	double height_, H_;	//���ΪԲ̨�����ز���
	double min_radius_, max_radius_;
	bool is_positive_;	//��ϵõ���Բ̨�������Ļ��ǵ�����

public:
	Slice(const double minvalue,const double maxvalue,const double minradius,const double highradius);
	~Slice();

	double MaxRadius() { return max_radius_; }
	const double MaxRadius() const { return max_radius_; }
	double MinRadius() { return min_radius_; }
	const double MinRadius()const { return min_radius_; }


	bool ContainVertex(const Vertex& vertex)const ;	//�ж϶����Ƿ�λ��Slice��
	bool ContainFace(const Face& face)const;	//�ж�������Ƭ�Ƿ�λ��Slice��
	int FacesSize() const;	//��ȡ������Ƭ������

	void AddFace(const Face& face);	//���������Ƭ

	bool IsRight() const;	//��ȡ��Ϻ��Բ̨�Ƿ�Ϊ������

	vector<Face> Faces();	//��ȡ������Ƭ����

	/**
		����Բ̨��ʽչ��������Ƭ
	*/
	EnrolledSlice EnrollSliceAsConial();	//չ��Slice
	void ConialFrustumFitting();	//���ΪԲ̨
	//����Բ̨��ʽչ��������Ƭ
	PlaneFace ExpandFaceAsConial(const Face& face, const  double height, const double R,
		const  double H, const  bool flag);


	/**
		����Բ����ʽչ��������Ƭ
	*/
	EnrolledSlice EnrollSliceAsCylinder();
	double CylinderFitting();	//���ΪԲ��������Բ����İ뾶
	PlaneFace ExpandFaceAsCylinder(const Face& face,const double radius);	//����Բ����ʽչ��������Ƭ

};

