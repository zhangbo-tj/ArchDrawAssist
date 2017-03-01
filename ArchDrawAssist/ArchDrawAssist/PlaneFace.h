#pragma once


#include "PlanePoint.h"
#include "Texture.h"


/**
	��άƽ���ڵ�������Ƭ�������䶥���������������
*/
class PlaneFace {
public:
	PlanePoint vertexA_, vertexB_, vertexC_;	//������Ƭ���������������
	Texture textureA_, textureB_, textureC_;	//������Ƭ�������������������

	/**
		���캯��
	*/
	PlaneFace(const PlanePoint& A, const PlanePoint& B, const PlanePoint& C,
		const Texture& TA, const Texture& TB, const Texture& TC)
		:vertexA_(A), vertexB_(B), vertexC_(C), textureA_(TA), textureB_(TB), textureC_(TC) {

	}

	/**
		�������캯��
	*/
	PlaneFace(const PlaneFace& face) 
		:vertexA_(face.VertexA()), vertexB_(face.VertexB()), vertexC_(face.VertexC()),
		textureA_(face.TextureA()), textureB_(face.TextureB()), textureC_(face.TextureC()) {
		
	}

	/**
		��ֵ�����
	*/
	PlaneFace& operator=(const PlaneFace& face) {
		vertexA_ = face.VertexA();
		vertexB_ = face.VertexB();
		vertexC_ = face.VertexC();
		textureA_ = face.TextureA();
		textureB_ = face.TextureB();
		textureC_ = face.TextureC();
		return *this;
	}

	/**
		��������Ƭ����ƽ��

		������
			offsetX, offsetY: ��Ƭ��X���Y�᷽���ƽ�ƾ���
	*/
	void Translate(double offsetX, double offsetY) {
		vertexA_.Translate(offsetX, offsetY);
		vertexB_.Translate(offsetX, offsetY);
		vertexC_.Translate(offsetX, offsetY);
	}


	///��Ա��getter��setter����
	const PlanePoint VertexA()const { return vertexA_; }
	const PlanePoint VertexB()const { return vertexB_; }
	const PlanePoint VertexC()const { return vertexC_; }
	const Texture TextureA()const { return textureA_; }
	const Texture TextureB()const { return textureB_; }
	const Texture TextureC()const { return textureC_; }


	PlanePoint VertexA() { return vertexA_; }
	PlanePoint VertexB() { return vertexB_; }
	PlanePoint VertexC() { return vertexC_; }
	Texture TextureA() { return textureA_; }
	Texture TextureB() { return textureB_; }
	Texture TextureC(){ return textureC_; }
};