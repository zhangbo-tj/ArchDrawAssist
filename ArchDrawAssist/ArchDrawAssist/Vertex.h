#pragma once
#pragma once
#include <math.h>
class Vertex {
public:
	double mVx, mVy, mVz;// 顶点的x,y,z坐标
public:
	//默认构造函数
	Vertex()
		:mVx(0.0), mVy(0.0), mVz(0.0) {
	}

	//初始化顶点坐标
	void Init() {
		mVx = mVy = mVz = 0.0;
	}
	Vertex(const Vertex& v)
		:mVx(v.X()), mVy(v.Y()), mVz(v.Z()) {
	
	}

	//构造函数
	Vertex(double vx, double vy, double vz)
		:mVx(vx), mVy(vy), mVz(vz) {
	}

	//重载赋值操作符
	Vertex &operator=(const Vertex& v) {
		mVx = v.X();
		mVy = v.Y();
		mVz = v.Z();
		return *this;
	}

	Vertex operator+(const Vertex& v) const {
		return Vertex(mVx + v.X(), mVy + v.Y(), mVz + v.Z());
	}
	Vertex operator-(const Vertex& v)const {
		return Vertex(mVx - v.X(), mVy - v.Y(), mVz - v.Z());
	}
	Vertex operator*(const double s)const {
		return Vertex(mVx *s, mVy*s, mVz*s);
	}
	Vertex operator/(const double s)const {
		return Vertex(mVx / s, mVy / s, mVz / s);
	}
	double operator*(const Vertex& v)const {
		return mVx*v.X() + mVy*v.Y() + mVz*v.Z();
	}
	Vertex operator^(const Vertex& v)const {
		return Vertex(mVy*v.Z() - mVz*v.Y(),
			mVz*v.X() - mVx*v.Z(),
			mVx*v.Y() - mVy*v.X());
	}

	Vertex& operator+=(const Vertex& v) {
		mVx += v.X();
		mVy += v.Y();
		mVz += v.Z();
		return *this;
	}
	Vertex& operator-=(const Vertex& v) {
		mVx -= v.X();
		mVy -= v.Y();
		mVz -= v.Z();
		return *this;
	}
	Vertex& operator *=(const double s) {
		mVx *= s;
		mVy *= s;
		mVz *= s;
		return *this;
	}
	Vertex& operator /= (const double s) {
		mVx /= s;
		mVy /= s;
		mVz /= s;
		return *this;
	}

	bool operator==(const Vertex& v)const {
		return mVx == v.X() && mVy == v.Y() && mVz == v.Z();
	}
	bool operator!=(const Vertex& v)const{
		return mVx != v.X() || mVy != v.Y() || mVz != v.Z();
	}


	//顶点相交
	Vertex Add(const Vertex& v)const {
		return Vertex(mVx + v.X(), mVy + v.Y(), mVz + v.Z());
	}

	//顶点相减
	Vertex Minus(const Vertex& v)const {
		return Vertex(mVx - v.X(), mVy - v.Y(), mVz - v.Z());
	}

	//顶点坐标乘以实数
	Vertex Mul(double s)const {
		return Vertex(mVx *s, mVy*s, mVz*s);
	}

	//顶点叉乘运算
	Vertex Cross(const Vertex& v) const {
		return Vertex(mVy*v.Z() - mVz*v.Y(),
			mVz*v.X() - mVx*v.Z(),
			mVx*v.Y() - mVy*v.X());
	}

	//将顶点坐标转换为float*指针
	double* ToDouble() {
		static double n[3] = { mVx,mVy,mVz };
		return n;
	}

	//顶点点乘运算
	double Dot(const Vertex& v) const {
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



double Distance(const Vertex& v1, const Vertex& v2) {
	return (v1 - v2).Norm();
}

double SquaredDistance(const Vertex& v1, const Vertex& v2) {
	return (v1 - v2).SquaredNorm();
}

double Norm(const Vertex& v1) {
	return v1.Norm();
}

double SquaredNorm(const Vertex& v1) {
	return v1.SquaredNorm();
}

Vertex Normalize(Vertex& v) {
	v.Normalize();
	return v;
}

double Dot(const Vertex& v1, const Vertex& v2) {
	return v1.Dot(v2);
}

Vertex Cross(const Vertex& v1, const Vertex& v2) {
	return v1.Cross(v2);
}

double Angle(const Vertex& v1, const Vertex& v2) {
	double w = v1.Norm()* v2.Norm();
	if (w == 0) {
		return -1;
	}
	double t = (v1 * v2) / w;
	if (t > 1) t = 1;
	else if (t < -1) t = -1;
	return acos(t);
}


double TriangleArea(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
	Vertex AB = v2 - v1;
	Vertex AC = v3 - v1;

	Vertex cross = AB.Cross(AC);
	return 0.5 * cross.Norm();
}