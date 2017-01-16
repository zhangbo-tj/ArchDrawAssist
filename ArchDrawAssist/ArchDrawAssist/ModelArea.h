#pragma once
#include "GLModel.h"
#include "Face.h"
#include "Slice.h"
#include "ModelUtils.h"
#include <vector>

///Qt��Ӧͷ�ļ�
#include <QGLWidget>
#include <QWidget>

///vcg����Ӧͷ�ļ�
#include <wrap/gui/trackball.h>

///opencv��Ӧͷ�ļ�
#include <cv.hpp>
#include <core\core.hpp>
#include <highgui\highgui.hpp>

class QMenu;
class QAction;

using namespace std;
using namespace cv;

/**
	��Ҫ������ʾ��άģ�ͣ����������ĸ�������ͶӰ����������з�ƽ�棬
	Ȼ����ʹ���з��з�ƽ��ָ���άģ�ͣ�֮���зֺ��Ƭ�ηֱ�չ��
	��������Ҫ������
*/
class ModelArea:public QGLWidget
{
public:
	Q_OBJECT
public:
	///QGlWidget����Ҫ��д�ļ�������
	ModelArea(QWidget* parent = 0);	//���캯��
	~ModelArea();	//��������
	void initializeGL();	//��ʼ���ؼ�����
	void resizeGL(int w, int h);	//resize�¼�����ʱ�Ĳ���
	void paintGL();	//���ƿؼ�


	///��ȡ��άģ�Ͳ�����������
	void LoadModel(GLModel* model);		//��ȡ��άģ��
	void ReadMtlFile(GLModel* model);	//��ȡMTL�ļ�
	//��ȡBMP�ļ������ɶ�Ӧ������
	void ReadBMPFile(GLModel* model, char* dir, char* strFileName, int textureID);

	void DrawXYZ();	//������ά����ϵ

	//void ComputeBestCuts();	//�����з�ƽ��
	void ComputeGradients(vector<double>& gradients);	//���㼸���������ͶӰ��Ӧ���ݶ�
	void DrawCutPlanes();	//�����з�ƽ��
	//����������Ƭ��ƽ��Ľ�������������
	int TriInterPlane(const Vertex& vertexA, const Vertex& vertexB, const Vertex& vertexC,
		const double yValue,vector<Vertex>& interPoints);
	//�����߶���ƽ��Ľ��㣬ǰ���Ƕ���һ�����ཻ
	Vertex LinesegInterPlane(const Vertex& vertexA, const Vertex& vertexB, const double yValue);
	double CalIntersectRadius(double yValue);	//�����з�ƽ���Ӧ�İ뾶

	//�������λ�ö�Ӧ����������ϵ�µ�����
	void GetWorldCoordinates(int mouse_x, int mouse_y,	
		double& world_x, double& world_y, double& world_z);
	void GetSelectedPlane(int mouse_x,int mouse_y);	//��ȡѡ�е��з�ƽ��
	void MovePlane(int mouse_x, int mouse_y);	//�ƶ�ѡ�е��з�ƽ��

	void DrawSolidCircleRed(double y, double radius);		//����ɫ��ʵ��Բ
	void DrawWiredCircleRed(double y, double radius);		//����ɫ������Բ
	void DrawSolidCircleGreen(double y, double radius);	//����ɫ��ʵ��Բ
	void DrawWiredCircleGreen(double y, double radius);	//����ɫ������Բ

	void InitActions();	//��ʼ�����е�actions
	void CombineRegions(vector<double>& gradients, vector<double> &average, 
		vector<double> &radii,vector<double> &cutPlanes,vector<double>& cutRadius);

	bool CombineRegion() { return combine_region_; }
	const bool CombineRegion()const { return combine_region_; }
	void SetCombineRegion(bool combine) { combine_region_ = combine; }

public slots:
	void UpdateTimer();	//ˢ�¿ؼ�ҳ��
	void AddCuttingPlane();	//����з�ƽ��
	void ResetTrackball();	//����trackball
	void DeleteCuttingPlane();	//ɾ���з�ƽ��
	void ComputeBestCuts();
	void ResetCombine();


public:
	//��ƽ����xozƽ����з�ƽ��ȥ�ָ���άģ��
	void CutPlaneY(const Face& face, const double yValue, vector<Face>& faces);
	//��ƽ����yozƽ����з�ƽ��ȥ�ָ���άģ��
	void CutPlaneX(const Face& face, const double xValue, vector<Face>& faces);
	//���з�ƽ��ָ���άģ��
	vector<EnrolledSlice> CutModel();

private:
	///�¼�
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

public:
	GLModel* model_;	//��άģ��
	vcg::Trackball trackball_;	//trackball,���ڿ���������ת�Ȳ���
	int paint_num_;	//������תģ�ͻ����ͶӰ
	bool need_rotate_;	//�ж��Ƿ���Ҫ��ת
	int texture_threshold_;
	bool combine_region_;

	vector<double> cut_planes_;	//�з�ƽ��
	vector<double> cut_radii_;	//�з�ƽ���Ӧ�İ뾶


	int selected_cut_plane;	//ѡ�е��з�ƽ�������
	bool right_button_pressed;	//����Ҽ��Ƿ񱻰���

	QMenu* right_button_menu_;	//�Ҽ��˵�
	QAction* add_plane_action_;	//����з�ƽ�����
	QAction* reset_trackball_action_;	//����trackball����
	QAction* delete_plane_action_;	//ɾ��ѡ�е��з�ƽ�����


	///����Ҽ�ʱ��Ӧ����������ϵ�µ�����
	double context_menu_x;	
	double context_menu_y;
	double context_menu_z;

};

