#pragma once
#include "GLModel.h"
#include "EnrolledSlice.h"
#include "Slice.h"
#include <QGLWidget>
#include <QWidget>

/**
	��Ҫ������ʾչ���������ؼ��������Խ��н�ͼ��������չ���������
	���������̲����������ź��϶�
*/
class TextureArea:public QGLWidget
{
public:
	Q_OBJECT
public:

	///QGLWidget����Ҫ��д�ĺ���
	TextureArea(QWidget* parent = 0,QGLWidget* sharedWidget = 0);	//���캯��
	~TextureArea();	//��������
	void initializeGL();	//��ʼ���ؼ�����
	void resizeGL(int w, int h);	//resize�¼�ʱ�Ĳ���
	void paintGL();	//���ƿؼ�����


	//��ȡչ��������
	void LoadEnrolledSlices(const vector<EnrolledSlice>& enrolledslices);
	
	void UpdateBoundary();	//����slice֮���λ�ã�ʹ�䲻������ص�
	
	void Reset();	//���ÿؼ�����

public slots:
	void SaveTextureFile();	//�����ͼ

private:
	///�¼�
	void wheelEvent(QWheelEvent *event);	//�����¼�
	void mouseMoveEvent(QMouseEvent *event);	//����ƶ��¼�
	void mousePressEvent(QMouseEvent* event);	//������¼�
	void mouseReleaseEvent(QMouseEvent *event);	//����ͷ��¼�
	void keyPressEvent(QKeyEvent *event);	//���̵���¼�

private:
	vector<EnrolledSlice> enrolled_slices_;	//չ�����Slice


	double oldX_, oldY_;	//���ԭ����λ��
	double offsetX_, offsetY_;	//ƫ����
	bool left_pressed_;	//�������Ƿ񱻵��
	double scale_;	//���Ŷ�
};

