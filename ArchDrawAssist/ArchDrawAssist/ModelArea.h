#pragma once
#include "GLModel.h"
#include "Face.h"
#include "Slice.h"
#include "ModelUtils.h"
#include <vector>

///Qt相应头文件
#include <QGLWidget>
#include <QWidget>

///vcg库相应头文件
#include <wrap/gui/trackball.h>

///opencv相应头文件
#include <cv.hpp>
#include <core\core.hpp>
#include <highgui\highgui.hpp>

class QMenu;
class QAction;

using namespace std;
using namespace cv;

/**
	主要用于显示三维模型，并根据其四个方向正投影的纹理计算切分平面，
	然后再使用切分切分平面分割三维模型，之后将切分后的片段分别展开
	该类是主要功能类
*/
class ModelArea:public QGLWidget
{
public:
	Q_OBJECT
public:
	///QGlWidget必须要重写的几个函数
	ModelArea(QWidget* parent = 0);	//构造函数
	~ModelArea();	//析构函数
	void initializeGL();	//初始化控件参数
	void resizeGL(int w, int h);	//resize事件发生时的操作
	void paintGL();	//绘制控件


	///读取三维模型并加载其纹理
	void LoadModel(GLModel* model);		//读取三维模型
	void ReadMtlFile(GLModel* model);	//读取MTL文件
	//读取BMP文件并生成对应的纹理
	void ReadBMPFile(GLModel* model, char* dir, char* strFileName, int textureID);

	void DrawXYZ();	//绘制三维坐标系

	//void ComputeBestCuts();	//计算切分平面
	void ComputeGradients(vector<double>& gradients);	//计算几个方向的正投影对应的梯度
	void DrawCutPlanes();	//绘制切分平面
	//计算三角面片与平面的交点数量和坐标
	int TriInterPlane(const Vertex& vertexA, const Vertex& vertexB, const Vertex& vertexC,
		const double yValue,vector<Vertex>& interPoints);
	//计算线段与平面的交点，前提是二者一定会相交
	Vertex LinesegInterPlane(const Vertex& vertexA, const Vertex& vertexB, const double yValue);
	double CalIntersectRadius(double yValue);	//计算切分平面对应的半径

	//计算鼠标位置对应的世界坐标系下的坐标
	void GetWorldCoordinates(int mouse_x, int mouse_y,	
		double& world_x, double& world_y, double& world_z);
	void GetSelectedPlane(int mouse_x,int mouse_y);	//获取选中的切分平面
	void MovePlane(int mouse_x, int mouse_y);	//移动选中的切分平面

	void DrawSolidCircleRed(double y, double radius);		//画红色的实线圆
	void DrawWiredCircleRed(double y, double radius);		//画红色的虚线圆
	void DrawSolidCircleGreen(double y, double radius);	//画绿色的实线圆
	void DrawWiredCircleGreen(double y, double radius);	//画绿色的虚线圆

	void InitActions();	//初始化所有的actions
	void CombineRegions(vector<double>& gradients, vector<double> &average, 
		vector<double> &radii,vector<double> &cutPlanes,vector<double>& cutRadius);

	bool CombineRegion() { return combine_region_; }
	const bool CombineRegion()const { return combine_region_; }
	void SetCombineRegion(bool combine) { combine_region_ = combine; }

public slots:
	void UpdateTimer();	//刷新控件页面
	void AddCuttingPlane();	//添加切分平面
	void ResetTrackball();	//重置trackball
	void DeleteCuttingPlane();	//删除切分平面
	void ComputeBestCuts();
	void ResetCombine();


public:
	//用平行于xoz平面的切分平面去分割三维模型
	void CutPlaneY(const Face& face, const double yValue, vector<Face>& faces);
	//用平行于yoz平面的切分平面去分割三维模型
	void CutPlaneX(const Face& face, const double xValue, vector<Face>& faces);
	//用切分平面分割三维模型
	vector<EnrolledSlice> CutModel();

private:
	///事件
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

public:
	GLModel* model_;	//三维模型
	vcg::Trackball trackball_;	//trackball,用于控制缩放旋转等操作
	int paint_num_;	//用于旋转模型获得正投影
	bool need_rotate_;	//判断是否需要旋转
	int texture_threshold_;
	bool combine_region_;

	vector<double> cut_planes_;	//切分平面
	vector<double> cut_radii_;	//切分平面对应的半径


	int selected_cut_plane;	//选中的切分平面的索引
	bool right_button_pressed;	//鼠标右键是否被按下

	QMenu* right_button_menu_;	//右键菜单
	QAction* add_plane_action_;	//添加切分平面操作
	QAction* reset_trackball_action_;	//重置trackball操作
	QAction* delete_plane_action_;	//删除选中的切分平面操作


	///鼠标右键时对应的世界坐标系下的坐标
	double context_menu_x;	
	double context_menu_y;
	double context_menu_z;

};

