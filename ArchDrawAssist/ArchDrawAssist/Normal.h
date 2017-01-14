#pragma once

#include "Vertex.h"
#include <math.h>


/**
	��Ҫ����GLModel�ķ�����
*/
class Normal {
public:
	double mVx, mVy, mVz;// �����x,y,z����
public:
	//Ĭ�Ϲ��캯��
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

	//��ʼ����������
	void Init() {
		mVx = mVy = mVz = 0.0;
	}

	//���캯��
	Normal(double vx, double vy, double vz)
		:mVx(vx), mVy(vy), mVz(vz) {
	}

	//���ظ�ֵ������
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


	//�����ཻ
	Normal Add(const Normal& v)const {
		return Normal(mVx + v.X(), mVy + v.Y(), mVz + v.Z());
	}

	//�������
	Normal Minus(const Normal& v)const {
		return Normal(mVx - v.X(), mVy - v.Y(), mVz - v.Z());
	}

	//�����������ʵ��
	Normal Mul(double s)const {
		return Normal(mVx *s, mVy*s, mVz*s);
	}

	//����������
	Normal Cross(const Normal& v) const {
		return Normal(mVy*v.Z() - mVz*v.Y(),
			mVz*v.X() - mVx*v.Z(),
			mVx*v.Y() - mVy*v.X());
	}

	//����������ת��Ϊfloat*ָ��
	double* ToDouble() {
		static double n[3] = { mVx,mVy,mVz };
		return n;
	}

	//����������
	double Dot(const Normal& v) const {
		return mVx*v.X() + mVy*v.Y() + mVz*v.Z();
	}

	//���㵥λ��
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

	//��ȡ�����x����
	const double X() const { return mVx; }
	double X() { return mVx; }

	//��ȡ�����y����
	const double Y() const { return mVy; }
	double Y() { return mVy; }

	//��ȡ�����y����
	const double Z()const { return mVz; }
	double Z() { return mVz; }

	//���ö����X����
	void SetX(double vx) { mVx = vx; }

	//���ö����Y����
	void SetY(double vy) { mVy = vy; }

	//���ö����Z����
	void SetZ(double vz) { mVz = vz; }

	//���ö�������
	void Set(double* v) { mVx = v[0]; mVy = v[1]; mVz = v[2]; }

	//���ö�������
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