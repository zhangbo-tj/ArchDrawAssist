#pragma once


/**
	用于保存顶点纹理值的类，成员为x坐标和y坐标
*/
class Texture {
private:
	double x_, y_;	//纹理坐标值

public:

	///构造函数
	Texture()
		:x_(0.0), y_(0.0) {
	}
	Texture(double tx, double ty)
		:x_(tx), y_(ty) {
	}
	Texture(const Texture& tex)
		:x_(tex.X()), y_(tex.Y()) {
		
	}

	/**
		初始化成员
	*/
	void Init() {
		x_ = y_ = 0.0;
	}


	/**
		重载赋值运算符
	*/
	Texture& operator=(const Texture& t) {
		x_ = t.X();
		y_ = t.Y();
		return *this;
	}


	
	///	重载复合运算符
	Texture& operator +=(const Texture& texture) {
		x_ += texture.X();
		y_ += texture.Y();
		return *this;
	}
	Texture& operator -=(const Texture& texture) {
		x_ -= texture.X();
		y_ -= texture.Y();
		return *this;
	}
	Texture& operator *=(const double s) {
		x_ *= s;
		y_ *= s;
		return *this;
	}
	Texture& operator /=(const double s) {
		x_ /= s;
		y_ /= s;
		return *this;
	}

	Texture operator +(const Texture& texture)const {
		return Texture(x_ + texture.X(), y_ + texture.Y());
	}
	Texture operator - (const Texture& texture)const {
		return Texture(x_ - texture.X(), y_ - texture.Y());
	}
	Texture operator *(const double s)const {
		return Texture(x_ * s, y_*s);
	}
	Texture operator/(const double s)const {
		return Texture(x_ / s, y_ / s);
	}


	///成员的Getter和Setter方法
	double X() { return x_; }
	const double X()const { return x_; }
	double Y() { return y_; }
	const double Y()const { return y_; }

	void SetX(double x) { x_ = x; }
	void SetY(double y) { y_ = y; }
	void Set(double x, double y) { x_ = x, y_ = y; }
	void Set(double* t) { x_ = t[0]; y_ = t[1]; }

	double* ToDouble()const {
		static double n[2] = { x_,y_ };
		return n;
	}
};