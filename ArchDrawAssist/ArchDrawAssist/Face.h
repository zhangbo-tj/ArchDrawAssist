#pragma once
#include "Vertex.h"
#include "Texture.h"

/**
	��άģ���е�������Ƭ�࣬���Ա����Ϊ������Ƭ�����������Լ��������������
*/
class Face {
private:
	///������Ƭ����������
	Vertex vertexA_;	
	Vertex vertexB_;
	Vertex vertexC_;

	///������Ƭ�������������
	Texture textureA_;
	Texture textureB_;
	Texture textureC_;

	int groupnum_;	//������group
	bool is_positive_;	//��������Ƭ�Ƿ�λ��yozƽ���Ҳ�

public:
	///��Ա��getter��setter����
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
		Ĭ�Ϲ��캯��
	*/
	Face() 
		:groupnum_(0),is_positive_(true){

	}

	/**
		�������캯��
	*/
	Face(const Face& face)
		:vertexA_(face.VertexA()), vertexB_(face.VertexB()), vertexC_(face.VertexC()),
		textureA_(face.TextureA()), textureB_(face.TextureB()), textureC_(face.TextureC()),
		groupnum_(face.Group()),is_positive_(face.IsPositive()){
	
	}

	/**
		���캯��

		�����ֱ�Ϊ������������꼰����������
	*/
	Face(const Vertex& va, const Vertex& vb, const Vertex& vc, 
		const Texture& ta, const Texture& tb, const Texture& tc) 
		:vertexA_(va),vertexB_(vb),vertexC_(vc),
		textureA_(ta),textureB_(tb),textureC_(tc),
		groupnum_(0),is_positive_(true){

	}

	/**
		��ֵ�����
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