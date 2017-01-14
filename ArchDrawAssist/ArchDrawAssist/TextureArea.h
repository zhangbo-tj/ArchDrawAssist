#pragma once
#include "GLModel.h"
#include "EnrolledSlice.h"
#include "Slice.h"
#include <QGLWidget>
#include <QWidget>

/**
	主要用于显示展开后的纹理，并可以进行截图操作保存展开后的纹理
*/
class TextureArea:public QGLWidget
{
public:
	Q_OBJECT
public:

	///QGLWidget必须要重写的函数
	TextureArea(QWidget* parent = 0,QGLWidget* sharedWidget = 0);	//构造函数
	~TextureArea();	//析构函数
	void initializeGL();	//初始化控件参数
	void resizeGL(int w, int h);	//resize事件时的操作
	void paintGL();	//绘制控件内容


	//读取展开的纹理
	void LoadEnrolledSlices(const vector<EnrolledSlice>& enrolledslices);
	
	void UpdateBoundary();	//更新slice之间的位置，使其不会出现重叠
	
	
public slots:
	void SaveTextureFile();	//保存截图

private:
	///事件
	void wheelEvent(QWheelEvent *event);	//滚轮事件
	void mouseMoveEvent(QMouseEvent *event);	//鼠标移动事件
	void mousePressEvent(QMouseEvent* event);	//鼠标点击事件
	void mouseReleaseEvent(QMouseEvent *event);	//鼠标释放事件
	void keyPressEvent(QKeyEvent *event);	//键盘点击事件

private:
	vector<EnrolledSlice> enrolled_slices_;	//展开后的Slice


	double oldX_, oldY_;	//鼠标原来的位置
	double offsetX_, offsetY_;	//偏移量
	bool left_pressed_;	//鼠标左键是否被点击
	double scale_;	//缩放度
};

