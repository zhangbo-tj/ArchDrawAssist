#pragma once


/**
	GLModel内的三角面片
*/
class Triangle {
private:
	int vindices_[3];
	int nindices_[3];
	int tindices_[3];
	int findex_;

public:
	Triangle(int v[3], int n[3], int t[3], int f) {
		vindices_[0] = v[0]; vindices_[1] = v[1]; vindices_[2] = v[2];
		nindices_[0] = n[0]; nindices_[1] = n[1]; nindices_[2] = n[2];
		tindices_[0] = t[0]; tindices_[1] = t[1]; tindices_[2] = t[2];
		findex_ = f;
	}
	Triangle() {
		vindices_[0] = 0; vindices_[1] = 0; vindices_[2] = 0;
		nindices_[0] = 0; nindices_[1] = 0; nindices_[2] = 0;
		tindices_[0] = 0; tindices_[1] = 0; tindices_[2] = 0;
		findex_ = 0;
	}

	int* Vindices() { return vindices_; }
	int* Nindices() { return nindices_; }
	int* Tindices() { return tindices_; }
	int Findex() { return findex_; }

	const int* Vindices() const { return vindices_; }
	const int* Nindices() const { return nindices_; }
	const int* Tindices() const { return tindices_; }
	const int Findex()const { return findex_; }

	void SetVindices(int* v) {
		vindices_[0] = v[0]; 
		vindices_[1] = v[1];
		vindices_[2] = v[2];

	}
	void SetNindices(int* n) {
		nindices_[0] = n[0];
		nindices_[1] = n[1]; 
		nindices_[2] = n[2];
	}
	void SetTindices(int* t) {
		tindices_[0] = t[0]; 
		tindices_[1] = t[1]; 
		tindices_[2] = t[2];
	}
	void SetFindex(int f) {
		findex_ = f;
	}
};