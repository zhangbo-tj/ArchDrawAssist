#include "Slice.h"
#include "ModelUtils.h"
#define M_PI		3.14159265358979323846
#include <math.h>


#include <QDebug>


///构造函数
Slice::Slice(const double minvalue, const double maxvalue, const double lowradius, const double highradius) 
	:mLowerPoint(minvalue),mUpperPoint(maxvalue),mLowerRadius(lowradius),mHigherRadius(highradius),mFlag(true){

}

///析构函数
Slice::~Slice(){
}


///判断Slice是否包含顶点
bool Slice::ContainVertex(const Vertex& vertex) const{
	bool onLowPlane = abs(vertex.Y() - mLowerPoint) < 1.0e-8 ? true : false;
	bool onHighPlane = abs(vertex.Y() - mUpperPoint) < 1.0e-8 ? true : false;

	if (onLowPlane || onHighPlane || (!onLowPlane&&onHighPlane&& vertex.Y() < mUpperPoint&& vertex.Y() > mLowerPoint)) {
		return true;
	}
	return false;
}

///判断Slice是否包含三角面片
bool Slice::ContainFace(const Face& face) const{
	//return ContainVertex(face.VertexA()) && ContainVertex(face.VertexB()) && ContainVertex(face.VertexC());
	bool low = false, high = false;
	if (IsFaceUpperPlaneY(face, mLowerPoint) == 1) {
		low = true;
	}
	else {
		return false;
	}
	if (IsFaceUpperPlaneY(face, mUpperPoint) == -1) {
		high = true;
	}
	else {
		return false;
	}

	if (low&& high) {
		return true;
	}	
	return false;
}


///获取三角面片的数量
int Slice::FacesSize() const {
	return mFaces.size();
}


///添加三角面片
void Slice::AddFace(const Face& face) {
	mFaces.push_back(Face(face));
}


///判断拟合后的圆台是否为正立的
bool Slice::IsRight() const{
	if (mLowerRadius >= mHigherRadius) {
		return true;
	}
	return false;
}


///获取三角面片集合
vector<Face> Slice::Faces() {
	return mFaces;
}

///将Slice拟合为圆台
void Slice::ConialFrustumFitting() {
	
	vector<pPOINT> RefectPoints;
	vector<Vertex> vertices;
	for (int i = 0; i < mFaces.size(); i++) {
		Vertex vertexA = mFaces[i].VertexA();
		Vertex vertexB = mFaces[i].VertexB();
		Vertex vertexC = mFaces[i].VertexC();

		pPOINT p1 = { abs(vertexA.Y()), sqrt(vertexA.X() * vertexA.X() + vertexA.Z() * vertexA.Z()) };
		pPOINT p2 = { abs(vertexB.Y()), sqrt(vertexB.X() * vertexB.X() + vertexB.Z() * vertexB.Z()) };
		pPOINT p3 = { abs(vertexC.Y()), sqrt(vertexC.X() * vertexC.X() + vertexC.Z() * vertexC.Z()) };

		RefectPoints.push_back(p1);
		RefectPoints.push_back(p2);
		RefectPoints.push_back(p3);
	}
	CalcConvexHull(RefectPoints);

	float reflectmax_y = RefectPoints[0].y;
	float reflectmin_x = RefectPoints[0].x;
	float reflectmax_x = RefectPoints[0].x;
	float m1 = 0, m2 = 0, m3 = 0, m4 = 0;
	float x_y1 = 0, x_y2 = 0, x_y3 = 0, x_y4 = 0;
	float start_max = 0, end_max = 0, start_min = 0, end_min = 0;
	int k = 0;
	float V = 100;
	float VC = 0;
	float r1 = 0, r2 = 0, R1 = 0, R2 = 0;
	float distance = 0;
	for (int i = 1; i < RefectPoints.size(); i++) {
		if (RefectPoints[i].y > reflectmax_y)
		{
			reflectmax_y = RefectPoints[i].y;
			k = i;
		}
		if (RefectPoints[i].x > reflectmax_x)
			reflectmax_x = RefectPoints[i].x;
		if (RefectPoints[i].x < reflectmin_x)
			reflectmin_x = RefectPoints[i].x;
	}
	if (k - 2 >= 0) {
		if ((RefectPoints[k - 1].x != RefectPoints[k].x) && (RefectPoints[k - 1].x != RefectPoints[k - 2].x))
		{
			m1 = (RefectPoints[k - 1].y - RefectPoints[k].y) / (RefectPoints[k - 1].x - RefectPoints[k].x);
			m2 = (RefectPoints[k - 1].y - RefectPoints[k - 2].y) / (RefectPoints[k - 1].x - RefectPoints[k - 2].x);
			x_y1 = m1*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
			x_y2 = m2*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
			x_y3 = m1*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
			x_y4 = m2*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
			start_max = x_y1 < x_y2 ? x_y1 : x_y2;
			end_max = x_y1 > x_y2 ? x_y1 : x_y2;
			start_min = x_y3 < x_y4 ? x_y3 : x_y4;
			end_min = x_y3 > x_y4 ? x_y3 : x_y4;
			distance = (end_max - start_max) / 10;
			while ((start_max < end_max)&&abs(start_max-end_max) > 1.0e-3) {
				float  mp = (start_max - RefectPoints[k - 1].y) / (reflectmax_x - RefectPoints[k - 1].x);
				float  yp = mp*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
				VC = start_max *start_max + start_max*yp + yp*yp;
				if (VC < V)
				{
					V = VC;
					mMinRadius = start_max;
					mMaxRadius = yp;
				}
				start_max = start_max + distance;
			}
			distance = (end_min - start_min) / 10;
			while ((start_min < end_min)&&(abs(start_min-end_min)>1.0e-3) ){
				float  mp = (start_min - RefectPoints[k - 1].y) / (reflectmin_x - RefectPoints[k - 1].x);
				float  yp = mp*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
				VC = start_min*start_min + start_min*yp + yp*yp;
				if (VC < V)
				{
					V = VC;
					mMinRadius = yp;
					mMaxRadius = start_min;
				}
				start_min = start_min + distance;
			}
		}
		else
			if (RefectPoints[k - 1].x == RefectPoints[k - 2].x) {
				m1 = (RefectPoints[k - 1].y - RefectPoints[k].y) / (RefectPoints[k - 1].x - RefectPoints[k].x);
				mMinRadius = m1*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
				mMaxRadius = m1*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;

			}
			else if (RefectPoints[k - 1].x == RefectPoints[k].x) {
				m2 = (RefectPoints[k - 1].y - RefectPoints[k - 2].y) / (RefectPoints[k - 1].x - RefectPoints[k - 2].x);
				mMaxRadius = m2*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
				mMinRadius = m2*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
			}
	}


	if (k + 2 < RefectPoints.size()) {
		if ((RefectPoints[k + 1].x != RefectPoints[k].x) && (RefectPoints[k + 1].x != RefectPoints[k + 2].x))
		{
			m1 = (RefectPoints[k + 1].y - RefectPoints[k].y) / (RefectPoints[k + 1].x - RefectPoints[k].x);
			m2 = (RefectPoints[k + 1].y - RefectPoints[k + 2].y) / (RefectPoints[k + 1].x - RefectPoints[k + 2].x);
			x_y1 = m1*(reflectmin_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
			x_y2 = m2*(reflectmin_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
			x_y3 = m1*(reflectmax_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
			x_y4 = m2*(reflectmax_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
			start_min = x_y1 > x_y2 ? (x_y2) : (x_y1);
			end_min = x_y1 < x_y2 ? (x_y2) : (x_y1);
			start_max = x_y3 < x_y4 ? x_y3 : x_y4;
			end_max = x_y3 > x_y4 ? x_y3 : x_y4;
			distance = (end_min - start_min) / 10;
			while ((start_min < end_min)&&(abs(start_min-end_min) > 1.0e-3)) {
				float  mp = (start_min - RefectPoints[k + 1].y) / (reflectmin_x - RefectPoints[k + 1].x);
				float  yp = mp*(reflectmax_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
				VC = start_min *start_min + start_min*yp + yp*yp;
				if (VC < V)
				{
					V = VC;
					mMinRadius = start_min;
					mMaxRadius = yp;
				}
				start_min = start_min + distance;
			}
			distance = (end_max - start_max) / 10;
			while ((start_max < end_max)&&(abs(start_max-end_max)>1.0e-3)) {
				float  mp = (start_max - RefectPoints[k + 1].y) / (reflectmax_x - RefectPoints[k + 1].x);
				float  yp = mp*(reflectmin_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
				VC = start_max*start_max + start_max*yp + yp*yp;
				if (VC < V)
				{
					V = VC;
					mMinRadius = yp;
					mMaxRadius = start_max;
				}
				start_max = start_max + distance;
			}
		}
		else
			if (RefectPoints[k + 1].x == RefectPoints[k + 2].x) {
				m1 = (RefectPoints[k + 1].y - RefectPoints[k].y) / (RefectPoints[k + 1].x - RefectPoints[k].x);
				r1 = m1*(reflectmax_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
				R1 = m1*(reflectmin_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
				VC = r1*r1 + r1*R1 + R1*R1;
				if (VC < V)
				{
					V = VC;
					mMinRadius = r1;
					mMaxRadius = R1;
				}
			}
			else if (RefectPoints[k + 1].x == RefectPoints[k].x) {
				m2 = (RefectPoints[k + 1].y - RefectPoints[k + 2].y) / (RefectPoints[k + 1].x - RefectPoints[k + 2].x);
				R2 = m2*(reflectmax_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
				r2 = m2*(reflectmin_x - RefectPoints[k + 1].x) + RefectPoints[k + 1].y;
				VC = r2*r2 + r2*R2 + R2*R2;
				if (VC < V)
				{
					V = VC;
					mMinRadius = r2;
					mMaxRadius = R2;
				}
			}

	}
	RefectPoints.clear();
}


///将三角面片按照圆台方式展开
PlaneFace Slice::ExpandAsConial(const Face& face, const double height,const double R, const double H, const bool flag) {
	double x1 = face.VertexA().X();
	double y1 = face.VertexA().Y();
	double z1 = face.VertexA().Z();

	double x2 = face.VertexB().X();
	double y2 = face.VertexB().Y();
	double z2 = face.VertexB().Z();

	double x3 = face.VertexC().X();
	double y3 = face.VertexC().Y();
	double z3 = face.VertexC().Z();

	double  X1, X2, X3;
	double  Y1, Y2, Y3;
	double h1 = 0;
	double h2 = 0;
	double h3 = 0;
	double r1 = 0;
	double r2 = 0;
	double r3 = 0;
	double angle1 = 0.0, angle2 = 0.0, angle3 = 0.0;
	X1 = X2 = X3 = Y1 = Y2 = Y3 = 0;
	bool isFacePositive = face.IsPositive();//判断三角面片是位于yoz平面右边还是左边
											 //如果当前slice是正圆台
	if (flag == true)	//	当前代码的计算方法默认三维模型的对称轴是平行于y轴的
	{
		h1 = height - y1;
		h2 = height - y2;
		h3 = height - y3;
		r1 = h1*R / H;
		r2 = h2*R / H;
		r3 = h3*R / H;
		double aa, bb, cc;

		///该公式非常重要，可以影响展开后的效果
		aa = z1 / sqrt(x1*x1+z1*z1);
		bb = z2 / sqrt(x2*x2+z2*z2);
		cc = z3 / sqrt(x3*x3+z3*z3);
		if (aa > 1)
			aa = 1;
		if (aa < -1)
			aa = -1;
		if (bb > 1)
			bb = 1;
		if (bb < -1)
			bb = -1;
		if (cc > 1)
			cc = 1;
		if (cc < -1)
			cc = -1;
		if (isFacePositive) {
			X1 =		  sqrt(r1 * r1 + h1 * h1) * sin(M_PI * R / sqrt(H * H + R * R) - (r1 * acos(aa)) / sqrt(r1 * r1 + h1 * h1));
			Y1 = height - sqrt(r1 * r1 + h1 * h1) * cos(M_PI * R / sqrt(H * H + R * R) - (r1 * acos(aa)) / sqrt(r1 * r1 + h1 * h1));
			X2 =		  sqrt(r2 * r2 + h2 * h2) * sin(M_PI * R / sqrt(H * H + R * R) - (r2 * acos(bb)) / sqrt(r2 * r2 + h2 * h2));
			Y2 = height - sqrt(r2 * r2 + h2 * h2) * cos(M_PI * R / sqrt(H * H + R * R) - (r2 * acos(bb)) / sqrt(r2 * r2 + h2 * h2));
			X3 =		  sqrt(r3 * r3 + h3 * h3) * sin(M_PI * R / sqrt(H * H + R * R) - (r3 * acos(cc)) / sqrt(r3 * r3 + h3 * h3));
			Y3 = height - sqrt(r3 * r3 + h3 * h3) * cos(M_PI * R / sqrt(H * H + R * R) - (r3 * acos(cc)) / sqrt(r3 * r3 + h3 * h3));
		}
		else {
			X1 =		  sqrt(r1 * r1 + h1 * h1) * sin(M_PI * R / sqrt(H * H + R * R) - r1 * (acos((-1)*aa) + M_PI) / sqrt(r1 * r1 + h1 * h1));
			Y1 = height - sqrt(r1 * r1 + h1 * h1) * cos(M_PI * R / sqrt(H * H + R * R) - r1 * (acos((-1)*aa) + M_PI) / sqrt(r1 * r1 + h1 * h1));
			X2 =		  sqrt(r2 * r2 + h2 * h2) * sin(M_PI * R / sqrt(H * H + R * R) - r2 * (acos((-1)*bb) + M_PI) / sqrt(r2 * r2 + h2 * h2));
			Y2 = height - sqrt(r2 * r2 + h2 * h2) * cos(M_PI * R / sqrt(H * H + R * R) - r2 * (acos((-1)*bb) + M_PI) / sqrt(r2 * r2 + h2 * h2));
			X3 =		  sqrt(r3 * r3 + h3 * h3) * sin(M_PI * R / sqrt(H * H + R * R) - r3 * (acos((-1)*cc) + M_PI) / sqrt(r3 * r3 + h3 * h3));
			Y3 = height - sqrt(r3 * r3 + h3 * h3) * cos(M_PI * R / sqrt(H * H + R * R) - r3 * (acos((-1)*cc) + M_PI) / sqrt(r3 * r3 + h3 * h3));
		}
	}
	else	//当前slice是倒圆台
	{
		h1 = y1 - height;
		h2 = y2 - height;
		h3 = y3 - height;
		r1 = h1*R / H;
		r2 = h2*R / H;
		r3 = h3*R / H;
		double aa, bb, cc;

		///该公式非常重要，可以影响展开后的效果
		aa = z1 / sqrt(x1*x1 + z1*z1);
		bb = z2 / sqrt(x2*x2 + z2*z2);
		cc = z3 / sqrt(x3*x3 + z3*z3);
		if (aa > 1)
			aa = 1;
		if (aa < -1)
			aa = -1;
		if (bb > 1)
			bb = 1;
		if (bb < -1)
			bb = -1;
		if (cc > 1)
			cc = 1;
		if (cc < -1)
			cc = -1;
		//根据三角面片位于yoz平面的右侧或左侧计算展开后的位置
		if (isFacePositive) {
			X1 =          sqrt(r1 * r1 + h1 * h1) * sin(M_PI * R / sqrt(H * H + R * R) - (r1 * acos(aa)) / sqrt(r1 * r1 + h1 * h1));
			Y1 = height + sqrt(r1 * r1 + h1 * h1) * cos(M_PI * R / sqrt(H * H + R * R) - (r1 * acos(aa)) / sqrt(r1 * r1 + h1 * h1));
			X2 =		  sqrt(r2 * r2 + h2 * h2) * sin(M_PI * R / sqrt(H * H + R * R) - (r2 * acos(bb)) / sqrt(r2 * r2 + h2 * h2));
			Y2 = height + sqrt(r2 * r2 + h2 * h2) * cos(M_PI * R / sqrt(H * H + R * R) - (r2 * acos(bb)) / sqrt(r2 * r2 + h2 * h2));
			X3 =		  sqrt(r3 * r3 + h3 * h3) * sin(M_PI * R / sqrt(H * H + R * R) - (r3 * acos(cc)) / sqrt(r3 * r3 + h3 * h3));
			Y3 = height + sqrt(r3 * r3 + h3 * h3) * cos(M_PI * R / sqrt(H * H + R * R) - (r3 * acos(cc)) / sqrt(r3 * r3 + h3 * h3));
		}
		else {
			X1 =		  sqrt(r1 * r1 + h1 * h1) * sin(M_PI * R / sqrt(H * H + R * R) - r1 * (acos((-1)*aa) + M_PI) / sqrt(r1 * r1 + h1 * h1));
			Y1 = height + sqrt(r1 * r1 + h1 * h1) * cos(M_PI * R / sqrt(H * H + R * R) - r1 * (acos((-1)*aa) + M_PI) / sqrt(r1 * r1 + h1 * h1));
			X2 =		  sqrt(r2 * r2 + h2 * h2) * sin(M_PI * R / sqrt(H * H + R * R) - r2 * (acos((-1)*bb) + M_PI) / sqrt(r2 * r2 + h2 * h2));
			Y2 = height + sqrt(r2 * r2 + h2 * h2) * cos(M_PI * R / sqrt(H * H + R * R) - r2 * (acos((-1)*bb) + M_PI) / sqrt(r2 * r2 + h2 * h2));
			X3 =		  sqrt(r3 * r3 + h3 * h3) * sin(M_PI * R / sqrt(H * H + R * R) - r3 * (acos((-1)*cc) + M_PI) / sqrt(r3 * r3 + h3 * h3));
			Y3 = height + sqrt(r3 * r3 + h3 * h3) * cos(M_PI * R / sqrt(H * H + R * R) - r3 * (acos((-1)*cc) + M_PI) / sqrt(r3 * r3 + h3 * h3));
		}
	}

	if (h1 == 0)
	{
		X1 = 0;
		Y1 = 0;
	}
	if (h2 == 0)
	{
		X2 = 0;
		Y2 = 0;
	}
	if (h3 == 0)
	{
		X3 = 0;
		Y3 = 0;
	}
	return PlaneFace(PlanePoint(X1, Y1), PlanePoint(X2, Y2), PlanePoint(X3, Y3), face.TextureA(), face.TextureB(), face.TextureC());
}


///展开该Slice
EnrolledSlice Slice::EnrollSlice() {
	mFlag = IsRight();
	ConialFrustumFitting();	//拟合为圆台

	double h1 = mUpperPoint - mLowerPoint;
 	//mMinRadius = mLowerRadius < mHigherRadius ? mLowerRadius : mHigherRadius;
 	//mMaxRadius = mLowerRadius > mHigherRadius ? mLowerRadius : mHigherRadius;
	qDebug() << "max: min" << mMaxRadius << " " << mMinRadius<<"  "<<mLowerRadius<<"  "<<mHigherRadius<<" "<<mLowerPoint<<" "<<mUpperPoint ;
	if (mMaxRadius < mMinRadius) {
		std::swap(mMaxRadius, mMinRadius);
	}
	double step = mUpperPoint - mLowerPoint;
	if (mFlag) {
		H = mMaxRadius * step / (mMaxRadius - mMinRadius);
		mHeight = H + mLowerPoint;
	}
	else {
		H = mMaxRadius * step / (mMaxRadius - mMinRadius);
		mHeight = mUpperPoint - H;
	}
	vector<PlaneFace> plane_faces;
	for (int i = 0; i < mFaces.size(); ++i) {
		plane_faces.push_back(ExpandAsConial(mFaces[i], mHeight, mMaxRadius, H, mFlag));
		//plane_faces.push_back(ExpandAsCylinder(mFaces[i], mMaxRadius));
	}
	
	double alpha = M_PI * mMaxRadius / sqrt(H* H + mMaxRadius*mMaxRadius);
	double tempMinRadius = mMinRadius / mMaxRadius*sqrt(H*H + mMaxRadius*mMaxRadius);
	double tempMaxRadius = sqrt(H * H + mMaxRadius*mMaxRadius);
	PlanePoint boundary;
	if (mFlag) {
		boundary = PlanePoint(tempMinRadius*sin(alpha), mHeight - tempMinRadius*cos(alpha));
	}
	else {
		boundary = PlanePoint(tempMinRadius*sin(alpha), mHeight + tempMinRadius*cos(alpha));
	}
	return EnrolledSlice(plane_faces,boundary,PlanePoint(0,mHeight),tempMinRadius,tempMaxRadius,mFlag);
}

