#pragma once

#include "Vertex.h"
#include <math.h>


/**
	主要用于GLModel的法向量
*/
class Normal {
public:
	double x_, y_, z_;// 顶点的x,y,z坐标
public:
	//默认构造函数
	Normal()
		:x_(0.0), y_(0.0), z_(0.0) {
	}

	Normal(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
		Vertex u = v2 - v1;
		Vertex v = v3 - v1;
		float nx, ny, nz;
		x_ = u.Y() * v.Z() - u.Z() * v.Y();
		y_ = u.Z() * v.X() - u.X() * v.Z();
		z_ = u.X() * v.Y() - u.Y() * v.X();
	}

	//初始化顶点坐标
	void Init() {
		x_ = y_ = z_ = 0.0;
	}

	//构造函数
	Normal(double vx, double vy, double vz)
		:x_(vx), y_(vy), z_(vz) {
	}

	//重载赋值操作符
	Normal &operator=(const Normal& v) {
		x_ = v.X();
		y_ = v.Y();
		z_ = v.Z();
		return *this;
	}

	Normal operator+(const Normal& v) const {
		return Normal(x_ + v.X(), y_ + v.Y(), z_ + v.Z());
	}
	Normal operator-(const Normal& v)const {
		return Normal(x_ - v.X(), y_ - v.Y(), z_ - v.Z());
	}
	Normal operator*(const double s)const {
		return Normal(x_ *s, y_*s, z_*s);
	}
	Normal operator/(const double s)const {
		return Normal(x_ / s, y_ / s, z_ / s);
	}
	double operator*(const Normal& v)const {
		return x_*v.X() + y_*v.Y() + z_*v.Z();
	}
	Normal operator^(const Normal& v)const {
		return Normal(y_*v.Z() - z_*v.Y(),
			z_*v.X() - x_*v.Z(),
			x_*v.Y() - y_*v.X());
	}

	Normal& operator+=(const Normal& v) {
		x_ += v.X();
		y_ += v.Y();
		z_ += v.Z();
		return *this;
	}
	Normal& operator-=(const Normal& v) {
		x_ -= v.X();
		y_ -= v.Y();
		z_ -= v.Z();
		return *this;
	}
	Normal& operator *=(const double s) {
		x_ *= s;
		y_ *= s;
		z_ *= s;
		return *this;
	}
	Normal& operator /= (const double s) {
		x_ /= s;
		y_ /= s;
		z_ /= s;
		return *this;
	}

	bool operator==(const Normal& v)const {
		return x_ == v.X() && y_ == v.Y() && z_ == v.Z();
	}
	bool operator!=(const Normal& v)const {
		return x_ != v.X() || y_ != v.Y() || z_ != v.Z();
	}


	//顶点相交
	Normal Add(const Normal& v)const {
		return Normal(x_ + v.X(), y_ + v.Y(), z_ + v.Z());
	}

	//顶点相减
	Normal Minus(const Normal& v)const {
		return Normal(x_ - v.X(), y_ - v.Y(), z_ - v.Z());
	}

	//顶点坐标乘以实数
	Normal Mul(double s)const {
		return Normal(x_ *s, y_*s, z_*s);
	}

	//顶点叉乘运算
	Normal Cross(const Normal& v) const {
		return Normal(y_*v.Z() - z_*v.Y(),
			z_*v.X() - x_*v.Z(),
			x_*v.Y() - y_*v.X());
	}

	//将顶点坐标转换为float*指针
	double* ToDouble() {
		static double n[3] = { x_,y_,z_ };
		return n;
	}

	//顶点点乘运算
	double Dot(const Normal& v) const {
		return x_*v.X() + y_*v.Y() + z_*v.Z();
	}

	//顶点单位化
	void Normalize() {
		double norm = Norm();
		x_ /= norm;
		y_ /= norm;
		z_ /= norm;
	}

	double Norm()const {
		return sqrt(x_*x_ + y_*y_ + z_*z_);
	}
	double SquaredNorm()const {
		return x_*x_ + y_*y_ + z_*z_;
	}

	//获取顶点的x坐标
	const double X() const { return x_; }
	double X() { return x_; }

	//获取顶点的y坐标
	const double Y() const { return y_; }
	double Y() { return y_; }

	//获取顶点的y坐标
	const double Z()const { return z_; }
	double Z() { return z_; }

	//设置顶点的X坐标
	void SetX(double vx) { x_ = vx; }

	//设置顶点的Y坐标
	void SetY(double vy) { y_ = vy; }

	//设置顶点的Z坐标
	void SetZ(double vz) { z_ = vz; }

	//设置顶点坐标
	void Set(double* v) { x_ = v[0]; y_ = v[1]; z_ = v[2]; }

	//设置顶点坐标
	void Set(double vx, double vy, double vz) { x_ = vx; y_ = vy; z_ = vz; }
};



double Distance(const Normal& v1, const Normal& v2) {
	return (v1 - v2).Norm();
}

double SquaredDistance(const Normal& v1, const Normal& v2) {
	return (v1 - v2).SquaredNorm();
}

double Norm(const Normal& v1) {
	return v1.Norm();
}

double SquaredNorm(const Normal& v1) {
	return v1.SquaredNorm();
}

Normal Normalize(Normal& v) {
	v.Normalize();
	return v;
}

double Dot(const Normal& v1, const Normal& v2) {
	return v1.Dot(v2);
}

Normal Cross(const Normal& v1, const Normal& v2) {
	return v1.Cross(v2);
}

double Angle(const Normal& v1, const Normal& v2) {
	double w = v1.Norm()* v2.Norm();
	if (w == 0) {
		return -1;
	}
	double t = (v1 * v2) / w;
	if (t > 1) t = 1;
	else if (t < -1) t = -1;
	return acos(t);
}