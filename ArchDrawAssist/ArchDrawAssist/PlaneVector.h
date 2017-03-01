#pragma once
#include "PlanePoint.h"

#include <math.h>


/**
	��ά�ռ��ڵ�����������ά����
*/
class PlaneVector {
private:
	double x_;	//������xֵ
	double y_;	//������yֵ

public:

	///���캯��
	PlaneVector()
		:x_(0), y_(0.0) {

	}
	PlaneVector(const PlanePoint& p1, const PlanePoint& p2)
		:x_(p2.X() - p1.X()), y_(p2.Y() - p1.Y()) {

	}

	PlaneVector(const double x, const double y)
		:x_(x), y_(y) {

	}


	///��Ա��getter��setter����
	const double X()const { return x_; }
	double X() { return x_; }
	const double Y()const { return y_; }
	double Y() { return y_; }

	///����norm��squared normֵ
	double Norm() const { return sqrt(x_*x_ + y_*y_); }
	double SquaredNorm()const { return x_*x_ + y_*y_; }


	///���س�Ա�����
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

	///���ظ��������
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
		��˲���
	*/
	double Dot(const PlaneVector& vec)const {
		return x_* vec.X() + y_*vec.Y();
	}


	/**
		�Զ�ά�������е�λ��
	*/
	void Unit() {
		double norm = Norm();
		x_ /= norm;
		y_ /= norm;
	}

};