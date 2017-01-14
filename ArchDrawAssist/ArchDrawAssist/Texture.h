#pragma once
class Texture {
private:
	double x_, y_;

public:
	Texture()
		:x_(0.0), y_(0.0) {
	}
	Texture(double tx, double ty)
		:x_(tx), y_(ty) {
	}
	Texture(const Texture& tex)
		:x_(tex.X()), y_(tex.Y()) {
		
	}
	void Init() {
		x_ = y_ = 0.0;
	}
	Texture& operator=(const Texture& t) {
		x_ = t.X();
		y_ = t.Y();
		return *this;
	}


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