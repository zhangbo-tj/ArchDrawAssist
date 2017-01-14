#pragma once
#include <vector>



/**
	��Ҫ����GLModel�е�Group����
*/
class Group {
private:
	char* name_;	//����
	std::vector<int> triangles_;	//������Ƭ����
	int material_num_;	//material����

public:
	///��Ա��get��set����
	Group() 
		:name_(""){}
	Group(char* n,int m)
		:name_(n),material_num_(m){
	}

	char* Name() { return name_; }
	const char* Name()const { return name_; }

	std::vector<int> Triangles() { return triangles_; }
	int MaterialNo() { return material_num_; }
	const int MaterialNo()const { return material_num_; }

	void SetName(char* n) { name_ = n; }
	void SetMaterialno(int m) { material_num_ = m; }
	void AddTriangle(int t) { triangles_.push_back(t); }
};