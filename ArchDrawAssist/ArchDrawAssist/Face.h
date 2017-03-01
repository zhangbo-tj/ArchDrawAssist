#pragma once
#include "Vertex.h"
#include "Texture.h"

/**
	三维模型中的三角面片类，其成员属性为三角面片的三个顶点以及顶点的纹理坐标
*/
class Face {
private:
	///三角面片的三个顶点
	Vertex vertexA_;	
	Vertex vertexB_;
	Vertex vertexC_;

	///三角面片三个顶点的纹理
	Texture textureA_;
	Texture textureB_;
	Texture textureC_;

	int groupnum_;	//所属的group
	bool is_positive_;	//该三角面片是否位于yoz平面右侧

public:
	///成员的getter和setter操作
	Vertex VertexA() { return vertexA_; }
	Vertex VertexB() { return vertexB_; }
	Vertex VertexC() { return vertexC_; }


	Texture TextureA() { return textureA_; }
	Texture TextureB() { return textureB_; }
	Texture TextureC() { return textureC_; }

	const Vertex VertexA() const { return vertexA_; }
	const Vertex VertexB() const { return vertexB_; }
	const Vertex VertexC() const { return vertexC_; }


	const Texture TextureA() const { return textureA_; }
	const Texture TextureB() const { return textureB_; }
	const Texture TextureC() const { return textureC_; }

	int Group() { return groupnum_; }
	const int Group()const { return groupnum_; }
	bool IsPositive() {return is_positive_;}
	const bool IsPositive() const {	return is_positive_;}




	/**
		默认构造函数
	*/
	Face() 
		:groupnum_(0),is_positive_(true){

	}

	/**
		拷贝构造函数
	*/
	Face(const Face& face)
		:vertexA_(face.VertexA()), vertexB_(face.VertexB()), vertexC_(face.VertexC()),
		textureA_(face.TextureA()), textureB_(face.TextureB()), textureC_(face.TextureC()),
		groupnum_(face.Group()),is_positive_(face.IsPositive()){
	
	}

	/**
		构造函数

		参数分别为三个顶点的坐标及其纹理坐标
	*/
	Face(const Vertex& va, const Vertex& vb, const Vertex& vc, 
		const Texture& ta, const Texture& tb, const Texture& tc) 
		:vertexA_(va),vertexB_(vb),vertexC_(vc),
		textureA_(ta),textureB_(tb),textureC_(tc),
		groupnum_(0),is_positive_(true){

	}

	/**
		赋值运算符
	*/
	Face& operator=(const Face& face) {
		vertexA_ = face.VertexA();
		vertexB_ = face.VertexB();
		VertexC() = face.VertexC();
		textureA_ = face.TextureA();
		textureB_ = face.TextureB();
		textureC_ = TextureC();
		return *this;
	}

	void SetGroup(int group) {
		groupnum_ = group;
	}
	void SetPositive(bool p) {
		is_positive_ = p;
	}
	
};