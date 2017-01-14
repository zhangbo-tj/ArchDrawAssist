#include "TextureArea.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

///构造函数，其中shareWidget是共享纹理的控件
TextureArea::TextureArea(QWidget* parent,QGLWidget* shareWidget)
	:QGLWidget(parent,shareWidget){
	setMouseTracking(true);
	oldX_ = oldY_ = 0.0;
	offsetX_ = offsetY_ = 0.0;
	left_pressed_ = false;
	scale_ = 1.0;
}

///析构函数
TextureArea::~TextureArea(){
}



///初始化控件参数
void TextureArea::initializeGL() {
	glClearColor(1.0, 1.0, 1.0, 0);
	//glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glOrtho(-2, 2, -2, 2, -2, 2);

}


///resize控件时的操作
void TextureArea::resizeGL(int w, int h) {
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -2, 2);
	//gluPerspective(45.0,(GLfloat)newWidth/(GLfloat)newHeight, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


///绘制控件内容
void TextureArea::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(offsetX_, offsetY_, 0);
	glScaled(scale_,scale_,scale_);
	for (int i = 0; i < enrolled_slices_.size(); i++) {
		enrolled_slices_[i].DrawSlice();
	}
}



///读取展开后的纹理
void TextureArea::LoadEnrolledSlices(const vector<EnrolledSlice>& enrolledslices) {
	oldX_ = oldY_ = 0.0;
	offsetX_ = offsetY_ = 0.0;
	left_pressed_ = false;
	scale_ = 1.0;
	enrolled_slices_ = enrolledslices;
	UpdateBoundary();
	update();
}


///鼠标滚轮事件
void TextureArea::wheelEvent(QWheelEvent *e) {
	const int WHEEL_STEP = 120;
	double change_rate = 0.1;
	double change = (e->delta() < 0) ? (1 + change_rate) : (1 - change_rate);
	scale_ *= change;
	update();
}

///鼠标移动事件
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


///鼠标点击事件
void TextureArea::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton){
		left_pressed_ = true;
		oldX_ = (event->x()) / (static_cast<double>(width()));
		oldY_ = (height() - event->y()) / (static_cast<double>(height()));
	}
	update();
}


///鼠标释放事件
void TextureArea::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		left_pressed_ = false;
	}
	update();
}

///键盘点击事件，实际没有应用
void TextureArea::keyPressEvent(QKeyEvent *e) {
	update();
}


///更新Slice之间的边界，使其互相不会出现重叠的情况
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


///保存纹理截图
void TextureArea::SaveTextureFile() {
	QString fileName;
	fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存截图"), "", "*.bmp");
	QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
		mapToGlobal(pos()).x() - frameGeometry().width(), mapToGlobal(pos()).y(),
		frameGeometry().width() - 10, frameGeometry().height() - 10);
	if (!fileName.isNull()) {
		pixmap.save(fileName, "bmp");
	}
}