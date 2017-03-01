#include "TextureArea.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

///
/**
	���캯��������shareWidget�ǹ�������Ŀؼ�

	������
		shareWidget:��������Ŀؼ�
*/
TextureArea::TextureArea(QWidget* parent,QGLWidget* shareWidget)
	:QGLWidget(parent,shareWidget){
	setMouseTracking(true);
	oldX_ = oldY_ = 0.0;
	offsetX_ = offsetY_ = 0.0;
	left_pressed_ = false;
	scale_ = 1.0;
}

/**
	��������
*/
TextureArea::~TextureArea(){
}



/**
	��ʼ���ؼ�����
	�ڿؼ���ʼ��ʱֻ����һ��
*/
void TextureArea::initializeGL() {
	//glClearColor(0.9, 0.9, 0.9, 0);
	glClearColor(178.0/255, 178.0/255, 178.0/255, 0);	//�ؼ�������ɫ
	//glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glOrtho(-2, 2, -2, 2, -2, 2);

}


/**
	resize�ؼ�ʱ�Ĳ�������Ҫ�������ñȽϺõ��ӽ�
*/
void TextureArea::resizeGL(int w, int h) {
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -2, 2);
	//gluPerspective(45.0,(GLfloat)newWidth/(GLfloat)newHeight, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/**
	���ƿؼ�����
*/
void TextureArea::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(offsetX_, offsetY_, 0);
	glScaled(scale_,scale_,scale_);

	///����չ�����Slice
	for (int i = 0; i < enrolled_slices_.size(); i++) {
		enrolled_slices_[i].DrawSlice();
	}
}



/**
	����չ�����Slice

	������
		enrolledslices: չ�����ƽ��Slice����
*/
void TextureArea::LoadEnrolledSlices(const vector<EnrolledSlice>& enrolledslices) {
	oldX_ = oldY_ = 0.0;
	offsetX_ = offsetY_ = 0.0;
	left_pressed_ = false;
	scale_ = 1.0;
	enrolled_slices_ = enrolledslices;
	UpdateBoundary();
	update();
}


/**
	�������¼���������������Ч��
*/
void TextureArea::wheelEvent(QWheelEvent *e) {
	const int WHEEL_STEP = 120;
	double change_rate = 0.1;
	double change = (e->delta() < 0) ? (1 + change_rate) : (1 - change_rate);
	scale_ *= change;
	update();
}

/**
	����ƶ��¼����ڰ�ס������ʱ�ƶ�������ʵ���϶�Ч��
*/
void TextureArea::mouseMoveEvent(QMouseEvent *event) {
	if (left_pressed_) {
		double curX = (event->x()) / (static_cast<double>(width()));
		double curY = (height() - event->y()) / (static_cast<double>(height()));
		offsetX_ += curX - oldX_;
		offsetY_ += curY - oldY_;
		oldX_ = curX;
		oldY_ = curY;

		update();
		//qDebug() << "x,y :" << offsetX_ << "  " << offsetY_;
	}
}


/**
	������¼�
*/
void TextureArea::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton){
		left_pressed_ = true;
		oldX_ = (event->x()) / (static_cast<double>(width()));
		oldY_ = (height() - event->y()) / (static_cast<double>(height()));
	}
	update();
}


/**
	����ͷ��¼�
*/
void TextureArea::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		left_pressed_ = false;
	}
	update();
}

/**
	���̵���¼���ʵ��û��Ӧ�ã�ԭ����û��ʹ�õ�����
*/
void TextureArea::keyPressEvent(QKeyEvent *e) {
	update();
}


/**
	����Slice֮��ı߽磬ʹ�以�಻������ص������
	����ʹ�õ�����Ǹ���ÿ��slice��bounding boxʵ�ֵ�
*/
void TextureArea::UpdateBoundary() {
	double baseY = -100;
	double addY = 0;
	for (int i = 0; i < enrolled_slices_.size(); i++) {
		if (enrolled_slices_[i].LowerBound() < baseY) {
			addY = baseY - enrolled_slices_[i].LowerBound();
		}
		else {
			addY = 0;
		}
		enrolled_slices_[i].Translate(0, addY);
		baseY = enrolled_slices_[i].UpperBound();
	}
}


/**
	���������ͼ
*/
void TextureArea::SaveTextureFile() {
	QString fileName;
	///��ȡ��ͼ
	fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("�����ͼ"), "", "*.bmp");
	QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
		mapToGlobal(pos()).x() - frameGeometry().width(), mapToGlobal(pos()).y(),
		frameGeometry().width() - 10, frameGeometry().height() - 10);
	///�����ͼ
	if (!fileName.isNull()) {
		pixmap.save(fileName, "bmp");
		QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��άģ��չ��ͼ����ɹ�"));
	}
}


/**
	���ÿؼ������ź��϶�
*/
void TextureArea::Reset() {
	enrolled_slices_.clear();
	oldX_ = 0.0;	oldY_ = 0.0;
	offsetX_ = 0.0;	offsetY_ = 0.0;
	left_pressed_ = false;
	scale_ = 1.0;
}