#pragma once
#include "PlanePoint.h"

#include <math.h>


/**
	二维空间内的向量，即二维向量
*/
class PlaneVector {
private:
	double x_;	//向量的x值
	double y_;	//向量的y值

public:

	///构造函数
	PlaneVector()
		:x_(0), y_(0.0) {

	}
	PlaneVector(const PlanePoint& p1, const PlanePoint& p2)
		:x_(p2.X() - p1.X()), y_(p2.Y() - p1.Y()) {

	}

	PlaneVector(const double x, const double y)
		:x_(x), y_(y) {

	}


	///成员的getter和setter方法
	const double X()const { return x_; }
	double X() { return x_; }
	const double Y()const { return y_; }
	double Y() { return y_; }

	///计算norm和squared norm值
	double Norm() const { return sqrt(x_*x_ + y_*y_); }
	double SquaredNorm()const { return x_*x_ + y_*y_; }


	///重载成员运算符
	PlaneVector operator+(const PlaneVector& vec)const {
		return PlaneVector(x_ + vec.X(), y_ + vec.Y());
	}
	PlaneVector operator-(const PlaneVector& vec)const {
		return PlaneVector(x_ - vec.X(), y_ - vec.Y());
	}
	PlaneVector operator*(double scale)const {
		return PlaneVector(x_*scale, y_*scale);
	}
	PlaneVector operator/(double scale)const{
		return PlaneVector(x_ / scale, y_ / scale);
	}
	double operator*(const PlaneVector& vec) const{
		return x_*vec.X() + y_*vec.Y();
	}

	///重载复合运算符
	PlaneVector& operator+=(const PlaneVector& vec) {
		x_ += vec.X();
		y_ += vec.Y();
		return *this;
	}
	PlaneVector& operator-=(const PlaneVector& vec) {
		x_ -= vec.X();
		y_ -= vec.Y();
		return *this;
	}
	PlaneVector& operator*=(const double scale) {
		x_ *= scale;
		y_ *= scale;
		return *this;
	}
	PlaneVector& operator/=(const double scale) {
		x_ /= scale;
		y_ /= scale;
		return *this;
	}


	/**
		点乘操作
	*/
	double Dot(const PlaneVector& vec)const {
		return x_* vec.X() + y_*vec.Y();
	}


	/**
		对二维向量进行单位化
	*/
	void Unit() {
		double norm = Norm();
		x_ /= norm;
		y_ /= norm;
	}

};