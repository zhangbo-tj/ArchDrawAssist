#include "Slice.h"
#include "ModelUtils.h"
#define M_PI		3.14159265358979323846
#include <math.h>


#include <QDebug>


/**
	���캯��
	
	������
		minvalue  : slice���½�
		maxvalue  : slice���Ͻ�
		lowradius : slice�±���İ뾶
		highradius: slice�ϱ���İ뾶
*/
Slice::Slice(const double minvalue, const double maxvalue, const double lowradius, const double highradius) 
	:lower_point_(minvalue),upper_point_(maxvalue),lower_radius_(lowradius),higher_radius_(highradius),is_positive_(true){

}

/**
	��������
*/
Slice::~Slice(){
}


/**
	�ж�ĳ�������Ƿ�λ�ڸ�Slice��

	������
		vertex: ����

	����ֵ��
		����λ��Slice���򷵻�true,���򷵻�false
*/
bool Slice::ContainVertex(const Vertex& vertex) const{
	bool onLowPlane = abs(vertex.Y() - lower_point_) < 1.0e-8 ? true : false;
	bool onHighPlane = abs(vertex.Y() - upper_point_) < 1.0e-8 ? true : false;

	if (onLowPlane || onHighPlane || (!onLowPlane&&onHighPlane&& vertex.Y() < upper_point_&& vertex.Y() > lower_point_)) {
		return true;
	}
	return false;
}

/**
	�ж�������Ƭ�Ƿ�λ��Slice�ڣ�ֻ���������㶼λ��Slice�ڲſ���

	����:
		face: Ҫ����������Ƭ

	����ֵ��
		true or false
*/
bool Slice::ContainFace(const Face& face) const{
	bool low = false, high = false;
	if (IsFaceUpperPlaneY(face, lower_point_) == 1) {
		low = true;
	}
	else {
		return false;
	}
	if (IsFaceUpperPlaneY(face, upper_point_) == -1) {
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


/**
	��ȡ������Ƭ������
*/
int Slice::FacesSize() const {
	return faces_.size();
}


/**
	���������Ƭ����Slice��
*/
void Slice::AddFace(const Face& face) {
	faces_.push_back(Face(face));
}


/**
	�ж���Ϻ��Բ̨�Ƿ�Ϊ������
*/
bool Slice::IsRight() const{
	if (lower_radius_ >= higher_radius_) {
		return true;
	}
	return false;
}


/**
	��ȡ������Ƭ����
*/
vector<Face> Slice::Faces() {
	return faces_;
}

/**
	��Slice���ΪԲ̨
*/
void Slice::ConialFrustumFitting() {
	
	vector<pPOINT> RefectPoints;
	vector<Vertex> vertices;
	for (int i = 0; i < faces_.size(); i++) {
		Vertex vertexA = faces_[i].VertexA();
		Vertex vertexB = faces_[i].VertexB();
		Vertex vertexC = faces_[i].VertexC();

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
					min_radius_ = start_max;
					max_radius_ = yp;
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
					min_radius_ = yp;
					max_radius_ = start_min;
				}
				start_min = start_min + distance;
			}
		}
		else
			if (RefectPoints[k - 1].x == RefectPoints[k - 2].x) {
				m1 = (RefectPoints[k - 1].y - RefectPoints[k].y) / (RefectPoints[k - 1].x - RefectPoints[k].x);
				min_radius_ = m1*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
				max_radius_ = m1*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;

			}
			else if (RefectPoints[k - 1].x == RefectPoints[k].x) {
				m2 = (RefectPoints[k - 1].y - RefectPoints[k - 2].y) / (RefectPoints[k - 1].x - RefectPoints[k - 2].x);
				max_radius_ = m2*(reflectmax_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
				min_radius_ = m2*(reflectmin_x - RefectPoints[k - 1].x) + RefectPoints[k - 1].y;
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
					min_radius_ = start_min;
					max_radius_ = yp;
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
					min_radius_ = yp;
					max_radius_ = start_max;
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
					min_radius_ = r1;
					max_radius_ = R1;
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
					min_radius_ = r2;
					max_radius_ = R2;
				}
			}

	}
	RefectPoints.clear();
}


/**
	��������Ƭ����Բ̨��ʽչ������ȡչ�����ƽ��������Ƭ

	������
		face		   : ��Ҫչ����������Ƭ
		height,R,H,flag��Բ̨�Ĳ���

	����ֵ��
		չ�����ƽ��������Ƭ
*/
PlaneFace Slice::ExpandFaceAsConial(const Face& face, const double height,const double R, const double H, const bool flag) {
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
	bool isFacePositive = face.IsPositive();//�ж�������Ƭ��λ��yozƽ���ұ߻������
											 //�����ǰslice����Բ̨
	if (flag == true)	//	��ǰ����ļ��㷽��Ĭ����άģ�͵ĶԳ�����ƽ����y���
	{
		h1 = height - y1;
		h2 = height - y2;
		h3 = height - y3;
		r1 = h1*R / H;
		r2 = h2*R / H;
		r3 = h3*R / H;
		double aa, bb, cc;

		///�ù�ʽ�ǳ���Ҫ������Ӱ��չ�����Ч��
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
	else	//��ǰslice�ǵ�Բ̨
	{
		h1 = y1 - height;
		h2 = y2 - height;
		h3 = y3 - height;
		r1 = h1*R / H;
		r2 = h2*R / H;
		r3 = h3*R / H;
		double aa, bb, cc;

		///�ù�ʽ�ǳ���Ҫ������Ӱ��չ�����Ч��
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
		//����������Ƭλ��yozƽ����Ҳ��������չ�����λ��
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
	return PlaneFace(PlanePoint(X1, Y1), PlanePoint(X2, Y2), PlanePoint(X3, Y3),
		face.TextureA(), face.TextureB(), face.TextureC());
}


/**
	����Բ̨ģʽչ����Slice

	����ֵ��
		չ�����ƽ��Slice
*/
EnrolledSlice Slice::EnrollSliceAsConial() {
	is_positive_ = IsRight();
	ConialFrustumFitting();	//���ΪԲ̨
	min_radius_ = min_radius_ < 0.0 ? 0 : min_radius_;
	if (abs(max_radius_ - min_radius_) < 1.0e-8) {
		return EnrollSliceAsCylinder();
	}


	///����Բ̨�Ĳ��ֲ���
	double h1 = upper_point_ - lower_point_;
	if (max_radius_ < min_radius_) {
		std::swap(max_radius_, min_radius_);
	}
	double step = upper_point_ - lower_point_;
	if (is_positive_) {
		H_ = max_radius_ * step / (max_radius_ - min_radius_);
		height_ = H_ + lower_point_;
	}
	else {
		H_ = max_radius_ * step / (max_radius_ - min_radius_);
		height_ = upper_point_ - H_;
	}

	///����չ����õ���ƽ��������Ƭ����
	vector<PlaneFace> plane_faces;
	for (int i = 0; i < faces_.size(); ++i) {
		plane_faces.push_back(ExpandFaceAsConial(faces_[i], height_, max_radius_, H_, is_positive_));
	}
	

	///����չ�����ƽ��Slice�Ĳ��ֲ�������Щ������û��ʵ��
	double alpha = M_PI * max_radius_ / sqrt(H_* H_ + max_radius_*max_radius_);
	double tempMinRadius = min_radius_ / max_radius_*sqrt(H_*H_ + max_radius_*max_radius_);
	double tempMaxRadius = sqrt(H_ * H_ + max_radius_*max_radius_);
	PlanePoint boundary;
	if (is_positive_) {
		boundary = PlanePoint(tempMinRadius*sin(alpha), height_ - tempMinRadius*cos(alpha));
	}
	else {
		boundary = PlanePoint(tempMinRadius*sin(alpha), height_ + tempMinRadius*cos(alpha));
	}

	///����չ�����Slice
	return EnrolledSlice(plane_faces,boundary,PlanePoint(0,height_),tempMinRadius,tempMaxRadius,is_positive_);
}



/**
	���ΪԲ��

	����ֵ��
		��Ϻ��Բ���İ뾶
*/
double Slice::CylinderFitting() {
	double max_radius = -1000;

	for (auto face : faces_) {
		double radiusA = sqrt(face.VertexA().X()*face.VertexA().X() + face.VertexA().Z()*face.VertexA().Z());
		double radiusB = sqrt(face.VertexB().X()*face.VertexB().X() + face.VertexB().Z()*face.VertexB().Z());
		double radiusC = sqrt(face.VertexC().X()*face.VertexC().X() + face.VertexC().Z()*face.VertexC().Z());

		max_radius = max_radius > radiusA ? max_radius : radiusA;
		max_radius = max_radius > radiusB ? max_radius : radiusB;
		max_radius = max_radius > radiusC ? max_radius : radiusC;
	}
	return max_radius;
}


/**
	��������Ƭ����Բ����ʽչ��

	������
		face  : Ҫչ����������Ƭ
		radius: Բ����İ뾶

	����ֵ��
		չ��������Ƭ�õ���ƽ����Ƭ
*/
PlaneFace Slice::ExpandFaceAsCylinder(const Face& face,const double radius) {
	double X1, X2, X3;
	double Y1, Y2, Y3;
	
	double Ax = face.VertexA().X();
	double Ay = face.VertexA().Y();
	double Az = face.VertexA().Z();

	double Bx = face.VertexB().X();
	double By = face.VertexB().Y();
	double Bz = face.VertexB().Z();

	double Cx = face.VertexC().X();
	double Cy = face.VertexC().Y();
	double Cz = face.VertexC().Z();

	const bool isFacePositive = face.IsPositive();

	double cosA = Az / sqrt(Ax*Ax + Az*Az);
	double cosB = Bz / sqrt(Bx*Bx + Bz*Bz);
	double cosC = Cz / sqrt(Cx*Cx + Cz*Cz);

	if (isFacePositive) {
		X1 = (M_PI - acos(cosA))*radius;
		X2 = (M_PI - acos(cosB))*radius;
		X3 = (M_PI - acos(cosC))*radius;
	}
	else {
		X1 = (acos(cosA) - M_PI)*radius;
		X2 = (acos(cosB) - M_PI)*radius;
		X3 = (acos(cosC) - M_PI)*radius;
	}

	Y1 = Ay;
	Y2 = By;
	Y3 = Cy;

	return PlaneFace(PlanePoint(X1, Y1), PlanePoint(X2, Y2), PlanePoint(X3, Y3),
		face.TextureA(), face.TextureB(), face.TextureC());
}


/**
	��Slice����Բ��ģʽ����չ��

	����ֵ��
		չ�����ƽ��Slice
*/
EnrolledSlice Slice::EnrollSliceAsCylinder() {
	double radius = CylinderFitting();
	vector<PlaneFace> enrolled_faces;

	for (auto &face : faces_) {
		enrolled_faces.push_back(ExpandFaceAsCylinder(face,radius));
	}
	return EnrolledSlice(enrolled_faces);
}

