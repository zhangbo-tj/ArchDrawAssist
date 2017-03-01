#pragma once
#include "Normal.h"

/**
	��ά�ռ��ڵĵ�,��Ϊ��VCG���ڵ�Point���ͻ�����Ը���ΪMyPoint
*/
class MyPoint {
private:
	double x_, y_, z_;	//�������ֵ
	Normal normal_;		//��ķ�����
	Normal principle1_, principle2_;	//���������
	int index_;	//����
	int trinum_;	//����������Ƭ


public:

	///���캯��
	MyPoint()
		:x_(0.0),y_(0.0),z_(0.0),index_(0),trinum_(0) {
	}
	MyPoint(double x, double y, double z, int num = 0)
		:x_(x),y_(y),z_(z),index_(0),trinum_(num){
	}
	double* ToDouble() {
		static double n[3] = { x_,y_,z_};
		return n;
	}

	///��Ա��getter��setter����
	double X() { return x_; }
	const double X()const { return x_; }
	double Y() { return y_; }
	const double Y()const { return y_; }
	double Z() { return z_; }
	const double Z() const { return z_; }
	int Trinum() { return trinum_; }
	const int Trinum()const { return trinum_; }
	int Index() { return index_; }
	const int Index() const { return index_; }
	Normal GetNormal() { return normal_; }
	const Normal GetNormal()const { return normal_; }
	Normal Principle1() { return principle1_; }
	const Normal Principle1()const { return principle1_; }
	Normal Principle2() { return principle2_; }
	const Normal Principle2()const { return principle2_; }


	void SetX(double x) { x_ = x; }
	void SetY(double y) { y_ = y; }
	void SetZ(double z) { z_ = z; }
	void SetNormal(const Normal& normal) { normal_ = normal; }
	void SetTrinum(int num) { trinum_ = num; }
	void SetPrinciple1(const Normal& normal) { principle1_ = normal; }
	void SetPrinciple2(const Normal& normal) { principle2_ = normal; }
	void SetIndex(int index) { index_ = index; }


	///���������
	MyPoint operator+(const MyPoint& p) const{
		return MyPoint(x_ + p.X(), y_ + p.Y(), z_ + p.Z());
	}
	MyPoint operator-(const MyPoint& p)const {
		return MyPoint(x_ - p.X(), y_ - p.Y(), z_ - p.Z());
	}
	MyPoint operator*(double s)const {
		return MyPoint(x_*s, y_*s, z_*s);
	}
	MyPoint operator/(double s)const {
		return MyPoint(x_ / s, y_ / s, z_ / s);
	}
	double operator*(const MyPoint& p)const {
		return x_*p.X() + y_*p.Y() + z_*p.Z();
	}
	MyPoint operator^(const MyPoint& p)const {
		return MyPoint(y_*p.Z() - z_*p.Y(),
			z_*p.X() - x_*p.Z(),
			x_*p.Y() - y_*p.X());
	}



	///���ظ��������
	MyPoint& operator+=(const MyPoint& p) {
		x_ += p.X();
		y_ += p.Y();
		z_ += p.Z();
		return *this;
	}
	MyPoint& operator-=(const MyPoint& p) {
		x_ -= p.X();
		y_ -= p.Y();
		z_ -= p.Z();
		return *this;
	}
	MyPoint& operator *=(const double  s) {
		x_ *= s;
		y_ *= s;
		z_ *= s;
		return *this;
	}
	MyPoint& operator /=(const double s) {
		x_ /= s;
		y_ /= s;
		z_ /= s;
		return *this;
	}


	///��ˡ���ˡ���λ���Ȳ���
	double Dot(const MyPoint& p) const{
		return x_*p.X() + y_*p.Y() + z_*p.Z();
	}

	MyPoint Cross(const MyPoint p)const {
		return MyPoint(y_*p.Z() - z_*p.Y(),
			z_*p.X() - x_*p.Z(),
			x_*p.Y() - y_*p.X());
	}

	double Norm() const{
		return sqrt(x_*x_ + y_*y_ + z_*z_);
	}
	double SquaredNorm()const {
		return x_*x_ + y_*y_ + z_*z_;
	}

	void Normalize() {
		double norm = Norm();
		x_ /= norm;
		y_ /= norm;
		z_ /= norm;
	}

};


/**
	����������֮��ľ���
*/
double Distance(const MyPoint& v1, const MyPoint& v2) {
	return (v1 - v2).Norm();
}

/**
	����������֮��ľ���ƽ��
*/
double SquaredDistance(const MyPoint& v1, const MyPoint& v2) {
	return (v1 - v2).SquaredNorm();
}

/**
	����������֮��ľ���
*/
double Norm(const MyPoint& v1) {
	return v1.Norm();
}


/**
	����������֮��ľ���ƽ��ֵ
*/
double SquaredNorm(const MyPoint& v1) {
	return v1.SquaredNorm();
}


/**
	��λ��
*/
MyPoint Normalize(MyPoint& v) {
	v.Normalize();
	return v;
}


/**
	��˲���
*/
double Dot(const MyPoint& v1, const MyPoint& v2) {
	return v1.Dot(v2);
}


/**
	��˲���
*/
MyPoint Cross(const MyPoint& v1, const MyPoint& v2) {
	return v1.Cross(v2);
}


/**
	����н�
*/
double Angle(const MyPoint& v1, const MyPoint& v2) {
	double w = v1.Norm()* v2.Norm();
	if (w == 0) {
		return -1;
	}
	double t = (v1 * v2) / w;
	if (t > 1) t = 1;
	else if (t < -1) t = -1;
	return acos(t);
}