#pragma once


#include "PlanePoint.h"
#include "Texture.h"


/**
	二维平面内的三角面片
*/
class PlaneFace {
public:
	PlanePoint vertexA_, vertexB_, vertexC_;
	Texture textureA_, textureB_, textureC_;

	PlaneFace(const PlanePoint& A, const PlanePoint& B, const PlanePoint& C,
		const Texture& TA, const Texture& TB, const Texture& TC)
		:vertexA_(A), vertexB_(B), vertexC_(C), textureA_(TA), textureB_(TB), textureC_(TC) {

	}
	PlaneFace(const PlaneFace& face) 
		:vertexA_(face.VertexA()), vertexB_(face.VertexB()), vertexC_(face.VertexC()),
		textureA_(face.TextureA()), textureB_(face.TextureB()), textureC_(face.TextureC()) {
		
	}

	PlaneFace& operator=(const PlaneFace& face) {
		vertexA_ = face.VertexA();
		vertexB_ = face.VertexB();
		vertexC_ = face.VertexC();
		textureA_ = face.TextureA();
		textureB_ = face.TextureB();
		textureC_ = face.TextureC();
		return *this;
	}

	void Translate(double offsetX, double offsetY) {
		vertexA_.Translate(offsetX, offsetY);
		vertexB_.Translate(offsetX, offsetY);
		vertexC_.Translate(offsetX, offsetY);
	}

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