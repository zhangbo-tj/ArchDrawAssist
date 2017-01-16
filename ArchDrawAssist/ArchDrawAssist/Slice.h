#pragma once

#include "Face.h"
#include "Vertex.h"
#include "PlaneFace.h"
#include "EnrolledSlice.h"
#include "vertexHull.h"

#include <vector>

using namespace std;

/**
	��άģ�ͱ��з�ƽ��ָ���Slice
*/

class Slice
{
public:
	vector<Face> mFaces;	//Slice�ڵ�������Ƭ����
	double mLowerPoint, mUpperPoint;	//Yֵ���޺�����
	double mLowerRadius, mHigherRadius;	//�½���İ뾶���Ͻ���İ뾶
	double mHeight, H;	//���ΪԲ̨�����ز���
	double mMinRadius, mMaxRadius;
	bool mFlag;	//��ϵõ���Բ̨�������Ļ��ǵ�����

public:
	Slice(const double minvalue,const double maxvalue,const double minradius,const double highradius);
	~Slice();

	double MaxRadius() { return mMaxRadius; }
	const double MaxRadius() const { return mMaxRadius; }
	double MinRadius() { return mMinRadius; }
	const double MinRadius()const { return mMinRadius; }


	bool ContainVertex(const Vertex& vertex)const ;	//�ж϶����Ƿ�λ��Slice��
	bool ContainFace(const Face& face)const;	//�ж�������Ƭ�Ƿ�λ��Slice��
	int FacesSize() const;	//��ȡ������Ƭ������

	void AddFace(const Face& face);	//���������Ƭ

	bool IsRight() const;	//��ȡ��Ϻ��Բ̨�Ƿ�Ϊ������

	vector<Face> Faces();	//��ȡ������Ƭ����
	EnrolledSlice EnrollSlice();	//չ��Slice
	void ConialFrustumFitting();	//���ΪԲ̨
	//����Բ̨��ʽչ��������Ƭ
	PlaneFace ExpandAsConial(const Face& face, const  double height, const double R, const  double H, const  bool flag);
};

