#pragma once
#include <vector>



/**
	主要用于GLModel中的Group属性
*/
class Group {
private:
	char* name_;	//名称
	std::vector<int> triangles_;	//三角面片集合
	int material_num_;	//material索引

public:
	///成员的get和set操作
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