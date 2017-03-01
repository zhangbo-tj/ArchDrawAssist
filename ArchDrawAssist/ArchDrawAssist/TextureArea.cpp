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
	构造函数，其中shareWidget是共享纹理的控件

	参数：
		shareWidget:共享纹理的控件
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
	析构函数
*/
TextureArea::~TextureArea(){
}



/**
	初始化控件参数
	在控件初始化时只调用一次
*/
void TextureArea::initializeGL() {
	//glClearColor(0.9, 0.9, 0.9, 0);
	glClearColor(178.0/255, 178.0/255, 178.0/255, 0);	//控件背景颜色
	//glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glOrtho(-2, 2, -2, 2, -2, 2);

}


/**
	resize控件时的操作，主要用于设置比较好的视角
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
	绘制控件内容
*/
void TextureArea::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(offsetX_, offsetY_, 0);
	glScaled(scale_,scale_,scale_);

	///绘制展开后的Slice
	for (int i = 0; i < enrolled_slices_.size(); i++) {
		enrolled_slices_[i].DrawSlice();
	}
}



/**
	导入展开后的Slice

	参数：
		enrolledslices: 展开后的平面Slice集合
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
	鼠标滚轮事件，用于设置缩放效果
*/
void TextureArea::wheelEvent(QWheelEvent *e) {
	const int WHEEL_STEP = 120;
	double change_rate = 0.1;
	double change = (e->delta() < 0) ? (1 + change_rate) : (1 - change_rate);
	scale_ *= change;
	update();
}

/**
	鼠标移动事件，在按住鼠标左键时移动鼠标可以实现拖动效果
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
	鼠标点击事件
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
	鼠标释放事件
*/
void TextureArea::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		left_pressed_ = false;
	}
	update();
}

/**
	键盘点击事件，实际没有应用，原因是没有使用到键盘
*/
void TextureArea::keyPressEvent(QKeyEvent *e) {
	update();
}


/**
	更新Slice之间的边界，使其互相不会出现重叠的情况
	现在使用的情况是根据每个slice的bounding box实现的
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
	保存纹理截图
*/
void TextureArea::SaveTextureFile() {
	QString fileName;
	///获取截图
	fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存截图"), "", "*.bmp");
	QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
		mapToGlobal(pos()).x() - frameGeometry().width(), mapToGlobal(pos()).y(),
		frameGeometry().width() - 10, frameGeometry().height() - 10);
	///保存截图
	if (!fileName.isNull()) {
		pixmap.save(fileName, "bmp");
		QMessageBox::information(this,QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("三维模型展开图保存成功"));
	}
}


/**
	重置控件的缩放和拖动
*/
void TextureArea::Reset() {
	enrolled_slices_.clear();
	oldX_ = 0.0;	oldY_ = 0.0;
	offsetX_ = 0.0;	offsetY_ = 0.0;
	left_pressed_ = false;
	scale_ = 1.0;
}