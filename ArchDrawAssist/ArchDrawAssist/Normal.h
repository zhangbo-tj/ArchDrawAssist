#pragma once

#include "Vertex.h"
#include <math.h>


/**
	��Ҫ����GLModel�ķ�����
*/
class Normal {
public:
	double x_, y_, z_;// �����x,y,z����
public:
	//Ĭ�Ϲ��캯��
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

	//��ʼ����������
	void Init() {
		x_ = y_ = z_ = 0.0;
	}

	//���캯��
	Normal(double vx, double vy, double vz)
		:x_(vx), y_(vy), z_(vz) {
	}

	//���ظ�ֵ������
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


	//�����ཻ
	Normal Add(const Normal& v)const {
		return Normal(x_ + v.X(), y_ + v.Y(), z_ + v.Z());
	}

	//�������
	Normal Minus(const Normal& v)const {
		return Normal(x_ - v.X(), y_ - v.Y(), z_ - v.Z());
	}

	//�����������ʵ��
	Normal Mul(double s)const {
		return Normal(x_ *s, y_*s, z_*s);
	}

	//����������
	Normal Cross(const Normal& v) const {
		return Normal(y_*v.Z() - z_*v.Y(),
			z_*v.X() - x_*v.Z(),
			x_*v.Y() - y_*v.X());
	}

	//����������ת��Ϊfloat*ָ��
	double* ToDouble() {
		static double n[3] = { x_,y_,z_ };
		return n;
	}

	//����������
	double Dot(const Normal& v) const {
		return x_*v.X() + y_*v.Y() + z_*v.Z();
	}

	//���㵥λ��
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

	//��ȡ�����x����
	const double X() const { return x_; }
	double X() { return x_; }

	//��ȡ�����y����
	const double Y() const { return y_; }
	double Y() { return y_; }

	//��ȡ�����y����
	const double Z()const { return z_; }
	double Z() { return z_; }

	//���ö����X����
	void SetX(double vx) { x_ = vx; }

	//���ö����Y����
	void SetY(double vy) { y_ = vy; }

	//���ö����Z����
	void SetZ(double vz) { z_ = vz; }

	//���ö�������
	void Set(double* v) { x_ = v[0]; y_ = v[1]; z_ = v[2]; }

	//���ö�������
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