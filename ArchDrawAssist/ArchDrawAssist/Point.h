#pragma once
#include "Normal.h"

/**
	三维空间内的点,因为与VCG库内的Point类冲突，所以改名为MyPoint
*/
class MyPoint {
private:
	double x_, y_, z_;	//点的坐标值
	Normal normal_;		//点的法向量
	Normal principle1_, principle2_;	//点的主方向
	int index_;	//索引
	int trinum_;	//所在三角面片


public:

	///构造函数
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

	///成员的getter和setter方法
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


	///重载运算符
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



	///重载复合运算符
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


	///点乘、叉乘、单位化等操作
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
	计算两个点之间的距离
*/
double Distance(const MyPoint& v1, const MyPoint& v2) {
	return (v1 - v2).Norm();
}

/**
	计算两个点之间的距离平方
*/
double SquaredDistance(const MyPoint& v1, const MyPoint& v2) {
	return (v1 - v2).SquaredNorm();
}

/**
	计算两个点之间的距离
*/
double Norm(const MyPoint& v1) {
	return v1.Norm();
}


/**
	计算两个点之间的距离平方值
*/
double SquaredNorm(const MyPoint& v1) {
	return v1.SquaredNorm();
}


/**
	单位化
*/
MyPoint Normalize(MyPoint& v) {
	v.Normalize();
	return v;
}


/**
	点乘操作
*/
double Dot(const MyPoint& v1, const MyPoint& v2) {
	return v1.Dot(v2);
}


/**
	叉乘操作
*/
MyPoint Cross(const MyPoint& v1, const MyPoint& v2) {
	return v1.Cross(v2);
}


/**
	计算夹角
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