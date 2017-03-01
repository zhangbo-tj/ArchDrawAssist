#pragma once

#include <math.h>


/**
	二维空间内的点，成员为点的坐标值
*/
class PlanePoint {
private:
	double px_;		//x坐标值
	double py_;		//y坐标值

public:
	/**
		构造函数
	*/
	PlanePoint(double x, double y)
		:px_(x), py_(y) {

	}

	PlanePoint()
		:px_(0.0), py_(0.0) {

	}

	/**
		拷贝构造函数
	*/
	PlanePoint(const PlanePoint& p)
		:px_(p.X()), py_(p.Y()) {
	
	}


	///成员的getter和setter方法
	const double X() const { return px_; }
	double X() { return px_; }

	const double Y()const { return py_; }
	double Y() { return py_; }


	///重载运算符
	PlanePoint operator +(const PlanePoint& point) const { 
		return PlanePoint(px_ + point.X(), py_ + point.Y()); 
	}
	PlanePoint operator-(const PlanePoint& point) const{
		return PlanePoint(px_ + point.X(), py_ - point.Y());
	}
	double operator*(const PlanePoint& p) const{
		return px_*p.X()+py_*p.Y();
	}
	PlanePoint operator*(double scale) const{
		return PlanePoint(px_*scale, py_*scale);
	}
	PlanePoint operator/(double scale)const {
		return PlanePoint(px_ / scale, py_ / scale);
	}
	

	///重载复合运算符
	PlanePoint& operator +=(const PlanePoint& point) {
		px_ += point.X();
		py_ += point.Y();
		return *this;
	}
	PlanePoint& operator-=(const PlanePoint& point) {
		px_ -= point.X();
		py_ -= point.Y();
		return *this;
	}
	PlanePoint& operator*=(const double scale) {
		px_ *= scale;
		py_ *= scale;
		return *this;
	}
	PlanePoint& operator/=(const double scale) {
		px_ /= scale;
		py_ /= scale;
		return *this;
	}


	double Dot(const PlanePoint& point) const{
		return px_ * point.X() + py_*point.Y();
	}


	/**
		点的平移操作

		参数：
			offsetX, offsetY:表示点在X方向和Y方向的平移距离
	*/
	void Translate(const double offsetX, double offsetY) {
		px_ += offsetX;
		py_ += offsetY;
	}

	double Norm()const{
		return sqrt(px_*px_ + py_*py_);
	}

	double SquaredNorm()const{
		return px_*px_ + py_*py_;
	}
};

double Distance(const PlanePoint& p1, const PlanePoint& p2) {
	return (p1 - p2).Norm();
}