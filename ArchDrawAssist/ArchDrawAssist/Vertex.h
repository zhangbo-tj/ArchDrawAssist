#pragma once
#pragma once
#include <math.h>


/**
	��άģ���ж�����࣬�������ϢΪ��������
*/
class Vertex {
private:
	double x_, y_, z_;// �����x,y,z����
public:
	/**
		Ĭ�Ϲ��캯��
	*/
	Vertex()
		:x_(0.0), y_(0.0), z_(0.0) {
	}



	/**
		��ʼ����������
	*/
	void Init() {
		x_ = y_ = z_ = 0.0;
	}


	/**
		�������캯��
	*/
	Vertex(const Vertex& v)
		:x_(v.X()), y_(v.Y()), z_(v.Z()) {
	
	}

	/**
		���캯��
	*/
	Vertex(double vx, double vy, double vz)
		:x_(vx), y_(vy), z_(vz) {
	}

	///�����
	Vertex &operator=(const Vertex& v) {
		x_ = v.X();
		y_ = v.Y();
		z_ = v.Z();
		return *this;
	}

	Vertex operator+(const Vertex& v) const {
		return Vertex(x_ + v.X(), y_ + v.Y(), z_ + v.Z());
	}
	Vertex operator-(const Vertex& v)const {
		return Vertex(x_ - v.X(), y_ - v.Y(), z_ - v.Z());
	}
	Vertex operator*(const double s)const {
		return Vertex(x_ *s, y_*s, z_*s);
	}
	Vertex operator/(const double s)const {
		return Vertex(x_ / s, y_ / s, z_ / s);
	}
	double operator*(const Vertex& v)const {
		return x_*v.X() + y_*v.Y() + z_*v.Z();
	}
	Vertex operator^(const Vertex& v)const {
		return Vertex(y_*v.Z() - z_*v.Y(),
			z_*v.X() - x_*v.Z(),
			x_*v.Y() - y_*v.X());
	}



	///���ظ��������
	Vertex& operator+=(const Vertex& v) {
		x_ += v.X();
		y_ += v.Y();
		z_ += v.Z();
		return *this;
	}
	Vertex& operator-=(const Vertex& v) {
		x_ -= v.X();
		y_ -= v.Y();
		z_ -= v.Z();
		return *this;
	}
	Vertex& operator *=(const double s) {
		x_ *= s;
		y_ *= s;
		z_ *= s;
		return *this;
	}
	Vertex& operator /= (const double s) {
		x_ /= s;
		y_ /= s;
		z_ /= s;
		return *this;
	}

	bool operator==(const Vertex& v)const {
		return x_ == v.X() && y_ == v.Y() && z_ == v.Z();
	}
	bool operator!=(const Vertex& v)const{
		return x_ != v.X() || y_ != v.Y() || z_ != v.Z();
	}


	///����֮�������

	//�����ཻ
	Vertex Add(const Vertex& v)const {
		return Vertex(x_ + v.X(), y_ + v.Y(), z_ + v.Z());
	}

	//�������
	Vertex Minus(const Vertex& v)const {
		return Vertex(x_ - v.X(), y_ - v.Y(), z_ - v.Z());
	}

	//�����������ʵ��
	Vertex Mul(double s)const {
		return Vertex(x_ *s, y_*s, z_*s);
	}

	//����������
	Vertex Cross(const Vertex& v) const {
		return Vertex(y_*v.Z() - z_*v.Y(),
			z_*v.X() - x_*v.Z(),
			x_*v.Y() - y_*v.X());
	}

	//����������ת��Ϊfloat*ָ��
	double* ToDouble() {
		static double n[3] = { x_,y_,z_ };
		return n;
	}

	//����������
	double Dot(const Vertex& v) const {
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