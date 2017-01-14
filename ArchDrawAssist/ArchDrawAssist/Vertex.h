#pragma once
#pragma once
#include <math.h>
class Vertex {
public:
	double mVx, mVy, mVz;// �����x,y,z����
public:
	//Ĭ�Ϲ��캯��
	Vertex()
		:mVx(0.0), mVy(0.0), mVz(0.0) {
	}

	//��ʼ����������
	void Init() {
		mVx = mVy = mVz = 0.0;
	}
	Vertex(const Vertex& v)
		:mVx(v.X()), mVy(v.Y()), mVz(v.Z()) {
	
	}

	//���캯��
	Vertex(double vx, double vy, double vz)
		:mVx(vx), mVy(vy), mVz(vz) {
	}

	//���ظ�ֵ������
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


	//�����ཻ
	Vertex Add(const Vertex& v)const {
		return Vertex(mVx + v.X(), mVy + v.Y(), mVz + v.Z());
	}

	//�������
	Vertex Minus(const Vertex& v)const {
		return Vertex(mVx - v.X(), mVy - v.Y(), mVz - v.Z());
	}

	//�����������ʵ��
	Vertex Mul(double s)const {
		return Vertex(mVx *s, mVy*s, mVz*s);
	}

	//����������
	Vertex Cross(const Vertex& v) const {
		return Vertex(mVy*v.Z() - mVz*v.Y(),
			mVz*v.X() - mVx*v.Z(),
			mVx*v.Y() - mVy*v.X());
	}

	//����������ת��Ϊfloat*ָ��
	double* ToDouble() {
		static double n[3] = { mVx,mVy,mVz };
		return n;
	}

	//����������
	double Dot(const Vertex& v) const {
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