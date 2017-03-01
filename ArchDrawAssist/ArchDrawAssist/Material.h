#pragma once

/**
	GLModel中的Material类
*/
class Material {
private:
	char* name_;	//名称
	float diffuse_[4];	//漫反射光参数
	float ambient_[4];	//环境光参数
	float specular_[4];	//
	float emmissive_[4];	
	float shiness_;	
	char* map_file_;	//映射文件

public:
	///成员的getter和setter操作
	Material(char* n, float d[4], float a[4], float s[4], float e[4], float sh, char* file) {
		name_ = n;
		diffuse_[0] = d[0]; diffuse_[1] = d[1]; diffuse_[2] = d[2]; diffuse_[3] = d[3];
		ambient_[0] = a[0]; ambient_[1] = a[1]; ambient_[2] = a[2]; ambient_[3] = a[3];
		specular_[0] = s[0]; specular_[1] = s[1]; specular_[2] = s[2]; specular_[3] = s[3];
		emmissive_[0] = e[0]; emmissive_[1] = e[1]; emmissive_[2] = e[2]; emmissive_[3] = e[3];
		shiness_ = sh;
		map_file_ = file;
	}

	Material(char* n) {
		name_ = n;
		diffuse_[0] = 0; diffuse_[1] = 0; diffuse_[2] = 0; diffuse_[3] = 0;
		ambient_[0] = 0; ambient_[1] = 0; ambient_[2] = 0; ambient_[3] =0;
		specular_[0] = 0; specular_[1] = 0; specular_[2] = 0; specular_[3] = 0;
		emmissive_[0] = 0; emmissive_[1] =0; emmissive_[2] = 0; emmissive_[3] = 0;
		shiness_ = 0;
		map_file_ = "";
	}

	char* Name() { return name_; }
	const char* Name()const { return name_; }
	float* Diffuse() { return diffuse_; }
	float* Ambient() { return ambient_; }
	float* Specular() { return specular_; }
	float* Emmissive() { return emmissive_; }
	float Shiness() { return shiness_; }
	char* MapFile() { return map_file_; }

	const float* Diffuse() const { return diffuse_; }
	const float* Ambient() const { return ambient_; }
	const float* Specular() const { return specular_; }
	const float* Emmissive() const { return emmissive_; }
	const float Shiness() const { return shiness_; }
	const char* MapFile() const { return map_file_; }

	void SetDiffuse(float d[4]){
		diffuse_[0] = d[0]; 
		diffuse_[1] = d[1];
		diffuse_[2] = d[2]; 
		diffuse_[3] = d[3];
	}
	void SetAmbient(float a[4]) {
		ambient_[0] = a[0];
		ambient_[1] = a[1];
		ambient_[2] = a[2]; 
		ambient_[3] = a[3];
	}
	void SetSpecular(float s[4]) {
		specular_[0] = s[0]; 
		specular_[1] = s[1]; 
		specular_[2] = s[2];
		specular_[3] = s[3];
	}
	void SetEmmissive(float e[4]) {
		emmissive_[0] = e[0];
		emmissive_[1] = e[1]; 
		emmissive_[2] = e[2]; 
		emmissive_[3] = e[3];
	}
	void SetShiness(float sh) {
		shiness_ = sh;
	}
	void SetMapFile(char* file) {
		map_file_ = file;
	}
};