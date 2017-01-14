#pragma once

#include "Vertex.h"
#include <math.h>


/**
	主要用于GLModel的法向量
*/
class Normal {
public:
	double mVx, mVy, mVz;// 顶点的x,y,z坐标
public:
	//默认构造函数
	Normal()
		:mVx(0.0), mVy(0.0), mVz(0.0) {
	}

	Normal(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
		Vertex u = v2 - v1;
		Vertex v = v3 - v1;
		float nx, ny, nz;
		mVx = u.Y() * v.Z() - u.Z() * v.Y();
		mVy = u.Z() * v.X() - u.X() * v.Z();
		mVz = u.X() * v.Y() - u.Y() * v.X();
	}

	//初始化顶点坐标
	void Init() {
		mVx = mVy = mVz = 0.0;
	}

	//构造函数
	Normal(double vx, double vy, double vz)
		:mVx(vx), mVy(vy), mVz(vz) {
	}

	//重载赋值操作符
	Normal &operator=(const Normal& v) {
		mVx = v.X();
		mVy = v.Y();
		mVz = v.Z();
		return *this;
	}

	Normal operator+(const Normal& v) const {
		return Normal(mVx + v.X(), mVy + v.Y(), mVz + v.Z());
	}
	Normal operator-(const Normal& v)const {
		return Normal(mVx - v.X(), mVy - v.Y(), mVz - v.Z());
	}
	Normal operator*(const double s)const {
		return Normal(mVx *s, mVy*s, mVz*s);
	}
	Normal operator/(const double s)const {
		return Normal(mVx / s, mVy / s, mVz / s);
	}
	double operator*(const Normal& v)const {
		return mVx*v.X() + mVy*v.Y() + mVz*v.Z();
	}
	Normal operator^(const Normal& v)const {
		return Normal(mVy*v.Z() - mVz*v.Y(),
			mVz*v.X() - mVx*v.Z(),
			mVx*v.Y() - mVy*v.X());
	}

	Normal& operator+=(const Normal& v) {
		mVx += v.X();
		mVy += v.Y();
		mVz += v.Z();
		return *this;
	}
	Normal& operator-=(const Normal& v) {
		mVx -= v.X();
		mVy -= v.Y();
		mVz -= v.Z();
		return *this;
	}
	Normal& operator *=(const double s) {
		mVx *= s;
		mVy *= s;
		mVz *= s;
		return *this;
	}
	Normal& operator /= (const double s) {
		mVx /= s;
		mVy /= s;
		mVz /= s;
		return *this;
	}

	bool operator==(const Normal& v)const {
		return mVx == v.X() && mVy == v.Y() && mVz == v.Z();
	}
	bool operator!=(const Normal& v)const {
		return mVx != v.X() || mVy != v.Y() || mVz != v.Z();
	}


	//顶点相交
	Normal Add(const Normal& v)const {
		return Normal(mVx + v.X(), mVy + v.Y(), mVz + v.Z());
	}

	//顶点相减
	Normal Minus(const Normal& v)const {
		return Normal(mVx - v.X(), mVy - v.Y(), mVz - v.Z());
	}

	//顶点坐标乘以实数
	Normal Mul(double s)const {
		return Normal(mVx *s, mVy*s, mVz*s);
	}

	//顶点叉乘运算
	Normal Cross(const Normal& v) const {
		return Normal(mVy*v.Z() - mVz*v.Y(),
			mVz*v.X() - mVx*v.Z(),
			mVx*v.Y() - mVy*v.X());
	}

	//将顶点坐标转换为float*指针
	double* ToDouble() {
		static double n[3] = { mVx,mVy,mVz };
		return n;
	}

	//顶点点乘运算
	double Dot(const Normal& v) const {
		return mVx*v.X() + mVy*v.Y() + mVz*v.Z();
	}

	//顶点单位化
	void Normalize() {
		double norm = Norm();
		mVx /= norm;
		mVy /= norm;
		mVz /= norm;
	}

	double Norm()const {
		return sqrt(mVx*mVx + mVy*mVy + mVz*mVz);
	}
	double SquaredNorm()const {
		return mVx*mVx + mVy*mVy + mVz*mVz;
	}

	//获取顶点的x坐标
	const double X() const { return mVx; }
	double X() { return mVx; }

	//获取顶点的y坐标
	const double Y() const { return mVy; }
	double Y() { return mVy; }

	//获取顶点的y坐标
	const double Z()const { return mVz; }
	double Z() { return mVz; }

	//设置顶点的X坐标
	void SetX(double vx) { mVx = vx; }

	//设置顶点的Y坐标
	void SetY(double vy) { mVy = vy; }

	//设置顶点的Z坐标
	void SetZ(double vz) { mVz = vz; }

	//设置顶点坐标
	void Set(double* v) { mVx = v[0]; mVy = v[1]; mVz = v[2]; }

	//设置顶点坐标
	void Set(double vx, double vy, double vz) { mVx = vx; mVy = vy; mVz = vz; }
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