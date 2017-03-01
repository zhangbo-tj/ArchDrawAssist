#include "ModelArea.h"

#include "Material.h"

#include <QOpenGLFunctions>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include <string>
#include <fstream>
#include <algorithm>

#include <gl/GLAUX.H>

/**
	构造函数,并初始化控件参数
	该函数为必需的
*/
ModelArea::ModelArea(QWidget* parent)
	:QGLWidget(parent)
	,model_(nullptr),paint_num_(0),need_rotate_(true),selected_cut_plane_(-1),
	right_button_pressed(false),combine_region_(false),texture_threshold_(25000){
	setMouseTracking(true);
// 	model_ = NULL;
// 	paint_num_ = 0;
// 	need_rotate_ = true;
// 	selected_cut_plane = -1;
// 	right_button_pressed = false;
// 	combine_region_ = false;

//	texture_threshold_ = 25000;
	glLoadIdentity();
	InitActions();	//初始化窗口的控件，即鼠标右键的几个操作
}




/**
	析构函数
*/
ModelArea::~ModelArea(){
}




/**
	初始化控件参数，包括背景、光照、轨迹球等
	该函数是必需的，每次生成控件时只调用一次
*/
void ModelArea::initializeGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClearColor(178.0 / 255, 178.0 / 255, 178.0 / 255, 0);	//设置控件背景颜色

	//glClearColor(1.0, 1.0, 1.0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);

	///光照
	GLfloat light_model_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_direction[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);

	///设置控件中轨迹球参数
	trackball_.center = vcg::Point3f(0, 0, 0);
	trackball_.radius = 1;
	trackball_.GetView();
	trackball_.Apply();
}




/**
	该函数为当控件大小发生改变时要进行的操作，主要用于设置投影模式以获得更好的观察三维模型的视角
	该函数是必需的

	参数：
		width : 控件新的宽度
		height: 控件的高度
*/
void ModelArea::resizeGL(int width, int height) {
	glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	//设置参数以调整最好视角
	double r = height / (static_cast<double>(width));
	double offsetX, offsetY;
	if (model_) {
		if (r >= 1.25) {
			offsetX = model_->mMaxPt[0] * 1.25;
			offsetY = offsetX * r;
		}
		else {
			offsetY = model_->mMaxPt[1] * 1.25;
			offsetX = offsetY * r;
		}
		glOrtho(-offsetX, offsetX, -offsetY, offsetY, -3, 2);
	}
	else {
		//gluPerspective(60, r, 1, 4);
		glOrtho(-1.25, 1.25, -1.25, 1.25, -3, 3);
	}
	glMatrixMode(GL_MODELVIEW);
}



/**
	控件的实际绘制内容的函数，可以用于实时绘制各种改变的内容
	该函数是必需的，且每次有任何操作时都会被调用一次
*/
void ModelArea::paintGL() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0, 0, -1);
	glPushMatrix();

	///使用trackball
	trackball_.GetView();
	trackball_.Apply();

	///判断是否载入模型
 	if (model_) {

		///判断是否需要旋转模型以计算正视图
 		if (need_rotate_) {
 			glClearColor(0.9, 0.9, 0.9, 0);
 			string image_name = GLMDirName(model_->mPathname);
 			QPixmap pixmap;

 			///读取模型后先旋转一圈后获取四个方向的纹理截图，用于计算切分平面
 			switch (paint_num_)
 			{
			case  -1:
				glColor3f(1, 1, 1);
				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);
				break;
 			case 0:
 				image_name += "image0.bmp";
 				//glRotated(90, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);		//绘制模型
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);		//获取截图
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//保存截图
 				break;
 			case 1:
 				image_name += "image1.bmp";
 				glRotated(90, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//绘制模型
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//获取截图
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//保存截图
 				break;
 			case 2:
 				image_name += "image2.bmp";
 				glRotated(180, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//绘制模型
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//获取截图
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//保存截图
 				break;
 			case 3:
 				image_name += "image3.bmp";
 				glRotated(270, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//绘制模型
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//获取截图
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//保存截图
 				break;
 			case 4:
 				image_name += "image4.bmp";
 				//glRotated(270, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//绘制模型
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//获取截图
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//保存截图
 				break;
 			default:
 				break;
 			}
 			paint_num_++;
 			if (paint_num_ == 5) {
 				need_rotate_ = false;
 			}
 		}
		///不需旋转模型，即正常绘制模型集合
 		else {
			glClearColor(178.0 / 255, 178.0 / 255, 178.0 / 255, 0);		//设置控件背景颜色
 			//glClearColor(0.9,0.9,0.9, 0);
 			glColor3f(1, 1, 1);
 			GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//绘制模型
 			///绘制切割平面
 			DrawCutPlanes();
 		}
 	}
 	glPopMatrix();
	
}


/**
	导入三维模型，并计算切分平面

	参数：
		model: 三维模型
*/
void ModelArea::LoadModel(GLModel* model) {
	model_ = model;		//载入模型
	ReadMtlFile(model_);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -3, 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	///重置控件参数
	paint_num_ = 0;
	need_rotate_ = false;
	combine_region_ = false;

	cut_planes_.clear();
	cut_radii_.clear();
	selected_cut_plane_ = -1;
	right_button_pressed = false;
	context_menu_x_ = 0.0;
	context_menu_y_ = 0.0;
	context_menu_z_ = 0.0;
	

	trackball_.Reset();		//重置轨迹球的参数
	update();	//更新控件的内容
}




/**
	读取MTL文件，加载纹理
*/
void ModelArea::ReadMtlFile(GLModel* glmModel) {
	FILE* file;
	char* filename;
	char buf[128];
	char buf1[128];
	int materialNum = 0;
	char* map_file;

	///获取模型目录，和MTL文件名
	char* dir = GLMDirName(glmModel->mPathname);
	char* mtlFileName = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(glmModel->mMtllibname) + 1));
	strcpy(mtlFileName, dir);
	strcat(mtlFileName, glmModel->mMtllibname);
	file = fopen(mtlFileName, "r");
	if (!file) {
		fprintf(stderr, "readMTLfile() failed: can't open material file \"%s\".\n", filename);
	}
	

	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case 'n':
			materialNum++;
			break;
		case 'm':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf1);
			map_file = buf;
			glmModel->mMaterials[materialNum].SetMapFile(map_file);
			ReadBMPFile(glmModel,dir, map_file, materialNum);
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
}


/**
	读取纹理文件，生成模型的纹理
*/
void ModelArea::ReadBMPFile(GLModel *glmModel, char* dir, char* strFileName, int textureID) {
	char* bmpFileName = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(strFileName) + 1));
	qDebug() << "ReadBMPFile: filename: " << strFileName;
	strcpy(bmpFileName, dir);
	strcat(bmpFileName, strFileName);
	AUX_RGBImageRec *pBitmap = NULL;
	pBitmap = auxDIBImageLoadA(bmpFileName);
	if (!pBitmap) {
		qDebug() << "Can't find the BMP file" << bmpFileName;
		return;
	}

	if (pBitmap == NULL) {
		qDebug() << "Can't read the BMP file: " << bmpFileName;
		return;
	}
	if (glIsTexture(glmModel->mTextureArray[textureID])) {
		glDeleteTextures(1, &glmModel->mTextureArray[textureID]);
	}
	glGenTextures(1, &glmModel->mTextureArray[textureID]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, glmModel->mTextureArray[textureID]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (pBitmap) {
		if (pBitmap->data) {
			free(pBitmap->data);
		}
		free(pBitmap);
	}
	return;
}


/**
	将Qt鼠标键盘事件转换为VCG库内的鼠标键盘事件
	原因是使用的trackball为VCG库里的trackball

	参数：
		qtbt     : Qt鼠标事件
		modifiers: Qt键盘事件
*/
vcg::Trackball::Button QT2VCG(Qt::MouseButton qtbt, Qt::KeyboardModifiers modifiers) {
	int vcgbt = vcg::Trackball::BUTTON_NONE;
	if (qtbt&Qt::LeftButton) vcgbt |= vcg::Trackball::BUTTON_LEFT;
	if (qtbt& Qt::RightButton) vcgbt |= vcg::Trackball::BUTTON_RIGHT;
	if (qtbt& Qt::MidButton) vcgbt |= vcg::Trackball::BUTTON_MIDDLE;
	if (modifiers& Qt::ShiftModifier) vcgbt |= vcg::Trackball::KEY_SHIFT;
	if (modifiers & Qt::ControlModifier) vcgbt |= vcg::Trackball::KEY_CTRL;
	if (modifiers & Qt::AltModifier) vcgbt |= vcg::Trackball::KEY_ALT;

	return vcg::Trackball::Button(vcgbt);
};




/**
	鼠标滚轮事件，用于传递给轨迹球
*/
void ModelArea::wheelEvent(QWheelEvent *event) {
	if (event->delta() > 0) {
		trackball_.MouseWheel(1);
	}
	else {
		trackball_.MouseWheel(-1);
	}
	update();
}


/**
	鼠标移动事件，用于移动切割平面
*/
void ModelArea::mouseMoveEvent(QMouseEvent *event) {
	if (selected_cut_plane_ != -1 && right_button_pressed) {
		MovePlane(event->x(), height() - event->y());	//移动切分平面
	}
	else {
		trackball_.MouseMove(event->x(), height() - event->y());
	}
	
	update();
}


/**
	鼠标点击事件，根据左键或右键选择进行相应的操作
*/
void ModelArea::mousePressEvent(QMouseEvent* event){
	if (event->button() == Qt::RightButton) {
		GetSelectedPlane(event->x(), height() - event->y());	//获取切分平面

		right_button_pressed = true;
	}
	else {
		trackball_.MouseDown(event->x(), height() - event->y(), QT2VCG(event->button(), event->modifiers()));
	}
	
	update();
}

/**
	松开鼠标按键的事件
*/
void ModelArea::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		right_button_pressed = false;
	}
	else {
		trackball_.MouseUp(event->x(), height() - event->y(), QT2VCG(event->button(), event->modifiers()));
	}
	
	update();
}

/**
	将Qt键盘事件转换为VCG键盘事件
*/
void ModelArea::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Control)
		trackball_.MouseDown(0, 0, QT2VCG(Qt::NoButton, Qt::ControlModifier));
	if (e->key() == Qt::Key_Shift)
		trackball_.MouseDown(0, 0, QT2VCG(Qt::NoButton, Qt::ShiftModifier));
	if (e->key() == Qt::Key_Alt)
		trackball_.MouseDown(0, 0, QT2VCG(Qt::NoButton, Qt::AltModifier));
	update();
}


/**
	鼠标右键事件，用于生成右键菜单
*/
void ModelArea::contextMenuEvent(QContextMenuEvent* event) {
	///未选中切分平面
	if (selected_cut_plane_ == -1) {
		right_button_menu_->clear();
		right_button_menu_->addAction(add_plane_action_);	//添加切分平面
		right_button_menu_->addAction(reset_trackball_action_);	//重置轨迹球
		right_button_menu_->exec(QCursor::pos());
	}
	///已选中切分平面
	else {
		right_button_menu_->clear();
		right_button_menu_->addAction(delete_plane_action_);	//删除切分平面
		right_button_menu_->addAction(reset_trackball_action_);	//重置轨迹球
		right_button_menu_->exec(QCursor::pos());
	}
	event->accept();
}


/**
	绘制三维坐标系
*/
void ModelArea::DrawXYZ() {
	glPushMatrix();
	glLineWidth(2);
	float fCursor[4];
	glGetFloatv(GL_CURRENT_COLOR, fCursor);	//获取当前颜色

	///x轴
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); //X
	glVertex3f(0, 0, 0);
	glVertex3f(0.2, 0, 0);


	///y轴
	glColor3f(0.0, 1.0, 0.0);//Y
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.2, 0);

	///z轴
	glColor3f(0.0, 0.0, 1.0);//Z
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.2);
	glEnd();

	glColor4fv(fCursor);
	glLineWidth(1.0);
	glPopMatrix();
}

/**
	比较大小，用于排序操作
*/
bool compare(int x, int y) {
	return x < y;
}



/**
	根据三维模型4个方向的正投影计算切分平面的位置
*/
void ModelArea::ComputeBestCuts() {

	//重置相关参数
	cut_planes_.clear();
	cut_radii_.clear();
	selected_cut_plane_ = -1;
	right_button_pressed = false;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -3, 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	///重置trackball,即重置视图
	trackball_.Reset();
	paint_num_ = -1;
	need_rotate_ = true;

	///分别获取模型在4个方向的正视图
	repaint();
	repaint();
	repaint();
	repaint();
	repaint();
	repaint();

	
	vector<double> gradients;
	ComputeGradients(gradients);	//计算4个正视图的梯度和	
	std::reverse(gradients.begin(), gradients.end());

	int startCol;
	int endCol;

	///计算梯度值不为0的区间
	for (int i = 0; i < gradients.size(); i++) {
		if (gradients[i] != 0) {
			startCol = i;
			break;
		}
	}
	for (int j = gradients.size(); j > 0; j--) {
		if (gradients[j - 1] != 0) {
			endCol = j;
			break;
		}
	}

	int colNum = endCol - startCol;


	///计算梯度值小于临界值的行
	vector<double> small_gradients;
	for (int i = 0; i < gradients.size(); ++i) {
		if (gradients[i] <= texture_threshold_) {	///该临界值对于不同的模型都不同
			small_gradients.push_back(i);
		}
	}

	///计算切分平面所在的行
	vector<int> cut_cols;
	for (int i = 1; i < small_gradients.size(); i++) {
		if (gradients[small_gradients[i - 1]] == 0 && gradients[small_gradients[i]] != 0) {
			cut_cols.push_back(small_gradients[i]);
		}
		else if (gradients[small_gradients[i - 1]] != 0 && gradients[small_gradients[i]] == 0) {
			cut_cols.push_back(small_gradients[i]);
		}
		else if (small_gradients[i] - small_gradients[i - 1] != 1) {
			cut_cols.push_back(small_gradients[i-1]+1);
			cut_cols.push_back(small_gradients[i]);
		}
	}


	///计算切分平面在模型中的位置，以及切分后得到的半径
	for (int i = 0; i < cut_cols.size(); ++i) {
		double scale = (cut_cols[i] - startCol) / (static_cast<double>(colNum));
		double cur_plane =  scale * 2-1;
		cut_planes_.push_back(cur_plane);
		cut_radii_.push_back(CalIntersectRadius(cur_plane));
	}


	///判断是否需要合并区域
	if(combine_region_) {
		vector<double> average;
		
		///计算每一段的梯度平均值
		for (int i = 0; i < cut_cols.size() - 1; i++) {
			int sum = std::accumulate(gradients.begin() + cut_cols[i], gradients.begin() + cut_cols[i + 1], 0);
			average.push_back(sum / (static_cast<double>(cut_cols[i + 1] - cut_cols[i])));
		}

		///将三维模型中的所有三角面片转换为Faces
		vector<Face> faces;
		for (auto tri : model_->mTriangles) {
			int* vindices = tri.Vindices();
			int* tindices = tri.Tindices();
			faces.push_back(Face(model_->mVertices[vindices[0]], model_->mVertices[vindices[1]], model_->mVertices[vindices[2]],
				model_->mTexcoods[tindices[0]], model_->mTexcoods[tindices[1]], model_->mTexcoods[tindices[2]]));
		}

		///使用yoz平面对三角面片集合进行切割，保存在CuttedFaces里
		vector<Face> cuttedFaces;
		for (auto face : faces) {
			CutPlaneX(face, 0.0, cuttedFaces);
		}
		faces.clear();
		faces = cuttedFaces;
		cuttedFaces.clear();

		///使用切割平面对三角面片进行切割，保存在CuttedFaces里
		for (auto plane : cut_planes_) {
			for (auto face : faces) {
				CutPlaneY(face, plane, cuttedFaces);
			}
			faces.clear();
			faces = cuttedFaces;
			cuttedFaces.clear();
		}
		
		///计算Slices
		vector<Slice> slices;
		for (int i = 0; i < cut_planes_.size() - 1; i++) {
			slices.push_back(Slice(cut_planes_[i], cut_planes_[i + 1], cut_radii_[i], cut_radii_[i + 1]));
		}

		for (int i = 0; i < faces.size(); i++) {
			int up = IsFaceUpperYOZ(faces[i]);
			if (up == 1) {
				faces[i].SetPositive(true);
			}
			else {
				faces[i].SetPositive(false);
			}
			for (int j = 0; j < slices.size(); j++) {
				if (slices[j].ContainFace(faces[i])) {
					slices[j].AddFace(faces[i]);
					break;
				}
			}
		}


		///计算每个部分的圆台拟合的半径
		vector<double> radii;
		for (auto slice : slices) {
			slice.ConialFrustumFitting();
			radii.push_back(slice.MaxRadius());
		}
		///对切割区域进行拟合
		CombineRegions(gradients, average, radii, cut_planes_,cut_radii_);
	}

	///弹出消息：已经计算完成切分平面
	QMessageBox::information(this, QString::fromLocal8Bit("提醒"), QString::fromLocal8Bit("切分平面计算完成"));


	update();	//绘制控件
}



/**
	使用opencv计算三维模型4个方向的正视图的梯度之和，保存在gradients里

	参数：
		gradients: 要计算的梯度集合
*/
void ModelArea::ComputeGradients(vector<double>& gradients) {
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat src_gray, dst, grad_x;
	vector<Mat> src;

	///读取4个正视图，并计算梯度值
	string str;
	for (int k = 1; k <= 4; ++k) {
		str = GLMDirName(model_->mPathname);
		str += "image" + to_string(k) + ".bmp";
		src.push_back(imread(str));
		cvtColor(src.at(k - 1), src_gray, CV_RGB2GRAY);
		cv::Sobel(src_gray, grad_x, ddepth, 1, 0, 3,scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_x, dst);

		if (gradients.size() == 0) {
			gradients.resize(src.at(k - 1).rows);
		}

		for (int i = 0; i < src.at(k - 1).rows; i++) {
			float sum = 0.0;
			uchar* p;
			p = dst.ptr<uchar>(i);
			for (int j = 0; j < src.at(k - 1).cols; j++) {
				sum += p[j];
			}
			gradients[i] += sum;
		}
	}

	///将梯度值保存在文件中，供以后处理备用
	string gradientFile = GLMDirName(model_->mPathname);
	gradientFile += "gradient.txt";
	std::ofstream ofs;
	ofs.open(gradientFile);
	for (auto i = 0; i < gradients.size(); ++i) {
		ofs << gradients[i] << endl;
	}
	ofs.close();	//关闭文件
	return;
}


/**
	先找到切分平面与模型的交点，然后根据这些交点与Y轴的距离计算半径，
	由于该半径主要用于实时显示，所以其大小并不需要特别精确

	参数：
		yValue: 切分平面

	返回值：
		半径值
*/
double ModelArea::CalIntersectRadius(double yValue) {
	Triangle tri;
	Vertex vertexA, vertexB, vertexC;
	vector<Vertex> interPoints;

	///计算切分平面与模型的交点
	for (int i = 0; i < model_->mTriangles.size(); i++) {
		tri = model_->mTriangles[i];
		int* vindices = tri.Vindices();
		vertexA = model_->mVertices[vindices[0]];
		vertexB = model_->mVertices[vindices[1]];
		vertexC = model_->mVertices[vindices[2]];
		interPoints.clear();
		if (TriInterPlane(vertexA, vertexB, vertexC, yValue,interPoints) > 0) {
			double sum = 0.0;
			///根据交点到Y轴的距离作为半径
			for (int i = 0; i < interPoints.size(); ++i) {
				sum += sqrt(interPoints[i].X()*interPoints[i].X() + interPoints[i].Z()*interPoints[i].Z());
			}
			return sum / interPoints.size();
		}
	}
	return 0;
}




/**
	计算三角面片与平面的交点数量以及交点坐标
	vertexA,vertexB,vertexC分别是该三角面片的三个顶点
	interPoints则是交点坐标

	参数：
		vertexA, vertexB, vertexC: 三角面片的三个顶点
		yValue                   : 切分平面

	返回值：
		interPoints: 交点数组
*/
int ModelArea::TriInterPlane(const Vertex& vertexA, const Vertex& vertexB, const Vertex& vertexC, 
	const double yValue,vector<Vertex>& interPoints) {
	double EPSILON = 1.0e-8;

	///A,B,C三个顶点是否位于平面上
	bool freeA = abs(vertexA.Y() - yValue) > EPSILON ? true : false;
	bool freeB = abs(vertexB.Y() - yValue) > EPSILON ? true : false;
	bool freeC = abs(vertexC.Y() - yValue) > EPSILON ? true : false;

	bool upA = freeA && (vertexA.Y() > yValue?true:false);
	bool downA = freeA && (vertexA.Y() < yValue?true:false);
	bool onA = !freeA;

	bool upB = freeB && (vertexB.Y() > yValue?true:false);
	bool downB = freeB && (vertexB.Y() < yValue);
	bool onB = !freeB;


	bool upC = freeC && (vertexC.Y() > yValue ? true : false);
	bool downC = freeC && (vertexC.Y() < yValue ? true : false);
	bool onC = !freeC;

	///A,B,C都在同一侧
	if ((upA&&upB&&upC) || (downA&&downB&&downC)) {
		return 0;
	}
	else if (onA&& onB&& onC) {
		interPoints.push_back(vertexA);
		interPoints.push_back(vertexB);
		interPoints.push_back(vertexC);
		return 3;
	}
	
	///两个顶点在平面上
	else if (onA&&onB) {
		interPoints.push_back(Vertex(vertexA));
		interPoints.push_back(Vertex(vertexB));
		return 2;
	}
	else if (onA && onC) {
		interPoints.push_back(Vertex(vertexA));
		interPoints.push_back(Vertex(vertexC));
		return 2;
	}
	else if (onB && onC) {
		interPoints.push_back(Vertex(vertexB));
		interPoints.push_back(Vertex(vertexC));
		return 2;
	}

	///一个顶点在平面上
	else if ((onA && upB && upC) || (onA && downB && downC)) {
		interPoints.push_back(Vertex(vertexA));
		return 1;
	}
	else if ((onB && upA && upC) || (onB && downA && downC)) {
		interPoints.push_back(Vertex(vertexB));
		return 1;
	}
	else if ((onC && upA && upB) || (onC && downA && downB)) {
		interPoints.push_back(Vertex(vertexC));
		return 1;
	}

	else if ((onA && upB && downC) || (onA && downB && upC)) {
		interPoints.push_back(Vertex(vertexA));
		interPoints.push_back(LinesegInterPlane(vertexB, vertexC, yValue));
		return 2;
	}
	else if ((onB && upA && downC) || (onB && downA && upC)) {
		interPoints.push_back(Vertex(vertexB));
		interPoints.push_back(LinesegInterPlane(vertexA, vertexC, yValue));
		return 2;
	}
	else if ((onC && upA && downB) || (onC && downA && upB)) {
		interPoints.push_back(Vertex(vertexC));
		interPoints.push_back(LinesegInterPlane(vertexA, vertexB, yValue));
		return 2;
	}


	///三个顶点分别位于平面两侧
	else if ((upA && downB && downC) || (downA && upB && upC)) {
		interPoints.push_back(LinesegInterPlane(vertexA, vertexB, yValue));
		interPoints.push_back(LinesegInterPlane(vertexA, vertexC, yValue));
		return 2;
	}
	else if ((downA && upB && downC) || (upA && downB && upC)) {
		interPoints.push_back(LinesegInterPlane(vertexB, vertexA, yValue));
		interPoints.push_back(LinesegInterPlane(vertexB, vertexC, yValue));
		return 2;
	}
	else if ((downA && downB && upC) || (upA && upB && downC)) {
		interPoints.push_back(LinesegInterPlane(vertexC, vertexA, yValue));
		interPoints.push_back(LinesegInterPlane(vertexC, vertexB, yValue));
		return 2;
	}
	else {
		return 0;
	}


}


/**
	计算线段与平面的交点
	
	参数：
		vertexA,vertexB: 线段的两个端点
		yValue         : 切分平面

	返回值：
		交点坐标值
*/
Vertex ModelArea::LinesegInterPlane(const Vertex& vertexA, const Vertex& vertexB,const double yValue) {
	Vertex AB = vertexB - vertexA;
	double scale = (yValue - vertexA.Y()) / AB.Y();
	return Vertex(vertexA.X() + AB.X()*scale, yValue, vertexA.Z() + AB.Z()*scale);
}


/**
	先对圆进行离散化，然后连接这些离散点绘制绿色实心圆
	
	参数：
		y     : 圆的位置
		radius： 圆的半径
*/
void ModelArea::DrawSolidCircleGreen(double y, double radius) {
	int sections = 200;

	double TWOPI = 2 * M_PI;
	glColor3f(0, 1, 0);		//设置颜色为绿色
	glBegin(GL_TRIANGLE_FAN);
	for (int count = 0; count <= sections; count++) {
		glVertex3d(radius*cos(count*TWOPI / sections), y, radius*sin(count *TWOPI / sections));
	}
	glEnd();
}


/**
	先对圆进行离散化，然后连接这些离散点绘制红色实心圆
	
	参数：
		y     : 圆的位置
		radius： 圆的半径
*/
void ModelArea::DrawSolidCircleRed(double y, double radius) {
	int sections = 200;

	double TWOPI = 2 * M_PI;
	glColor3f(1, 0, 0);		//设置颜色为红色
	glBegin(GL_TRIANGLE_FAN);
	for (int count = 0; count <= sections; count++) {
		glVertex3d(radius*cos(count*TWOPI / sections), y, radius*sin(count *TWOPI / sections));
	}
	glEnd();
}



/**
	先对圆进行离散化，然后连接这些离散点绘制红色虚线圆

	参数：
		y     : 圆的位置
		radius： 圆的半径
*/
void ModelArea::DrawWiredCircleRed(double y, double radius) {
	int count;
	int sections = 200;
	float TWOPI = 2 * M_PI;
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);	
	for (count = 0; count <= sections; count++) {
		glVertex3d(radius *cos(count * TWOPI / sections), y, radius * sin(count * TWOPI / sections));
	}
	glEnd();
}


/**
	先对圆进行离散化，然后连接这些离散点绘制绿色虚线圆

	参数：
		y     : 圆的位置
		radius： 圆的半径
*/
void ModelArea::DrawWiredCircleGreen(double y, double radius) {
	int count;
	int sections = 200;
	float TWOPI = 2 * M_PI;
	glColor3f(0, 1, 0);		//设置颜色
	glBegin(GL_LINE_STRIP);
	for (count = 0; count <= sections; count++) {
		glVertex3d(radius *cos(count * TWOPI / sections), y,radius * sin(count * TWOPI / sections));
	}
	glEnd();
}


/**
	绘制切分平面，选中的圆用绿色表示，未选中的用红色表示
*/
void ModelArea::DrawCutPlanes() {
	glLineWidth(1);
	for (int i = 0; i < cut_planes_.size(); ++i) {
		if (i == selected_cut_plane_) {		//被选中的切分平面
			DrawWiredCircleGreen(cut_planes_[i], cut_radii_[i]);
		}
		else {								//未被选中的切分平面
			DrawWiredCircleRed(cut_planes_[i], cut_radii_[i]);
		}
	}
}


/**
	将鼠标位置转换为对应的世界坐标系下的坐标
	
	参数：	
		mouse_x, mouse_y都为鼠标位置

	返回值：
		world_x, world_y, world_z为对应的世界坐标系下的值
*/
void ModelArea::GetWorldCoordinates(int mouse_x, int mouse_y,
	double& world_x, double& world_y, double& world_z) {
	
	glPushMatrix();
	glTranslated(0, 0, -1);
	float scale = trackball_.track.sca;
	glScaled(1,scale,1);
	trackball_.SetScale(1);
	GLdouble modelView[16];
	GLdouble projection[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glPopMatrix();
	float winZ;
	glReadPixels(mouse_x, mouse_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	///映射到世界坐标系
	gluUnProject((GLdouble)mouse_x, (GLdouble)mouse_y, winZ,
		modelView, projection, viewport,
		&world_x, &world_y, &world_z);
}



/**
	获取被选中的切分平面
	
	参数：
		mouse_x, mouse_y表示鼠标位置的坐标
*/
void ModelArea::GetSelectedPlane(int mouse_x, int mouse_y) {
	double world_x, world_y, world_z;
	
	///获取鼠标位置对应的世界坐标系
	GetWorldCoordinates(mouse_x, mouse_y, world_x, world_y, world_z);

	context_menu_x_ = world_x;
	context_menu_y_ = world_y;
	context_menu_z_ = world_z;

	double dis_delta = 0.03;
	
	///获取选中的切分平面
	for (int i = 0; i < cut_planes_.size(); ++i) {
		if (abs(cut_planes_[i] - world_y) < dis_delta) {
			selected_cut_plane_ = i;
			return;
		}
	}
	selected_cut_plane_ = -1;
}



/**
	移动选中的切分平面

	参数：
		mouse_x, mouse_y表示鼠标位置的坐标
*/
void ModelArea::MovePlane(int mouse_x, int mouse_y) {
	double world_x, world_y, world_z;
	///获取鼠标位置对应的世界坐标系下的坐标值
	GetWorldCoordinates(mouse_x, mouse_y, world_x, world_y, world_z);

	///移动选中的切分平面并实时计算切分半径
	if (selected_cut_plane_ != 0 &&
		selected_cut_plane_ != cut_planes_.size() - 1
		&& world_y < cut_planes_[selected_cut_plane_ + 1] &&
		world_y > cut_planes_[selected_cut_plane_ - 1]) {

		cut_planes_[selected_cut_plane_] = world_y;
		cut_radii_[selected_cut_plane_] = CalIntersectRadius(world_y);
	}
}


/**
	初始化鼠标右键菜单以及对应的操作	
*/
void ModelArea::InitActions() {
	///创建菜单和操作
	right_button_menu_ = new QMenu();
	add_plane_action_ = new QAction(QString::fromLocal8Bit("添加横截面"), this);
	reset_trackball_action_ = new QAction(QString::fromLocal8Bit("重置"), this);
	delete_plane_action_ = new QAction(QString::fromLocal8Bit("删除横截面"), this);

	///为操作添加信号槽
	connect(add_plane_action_, SIGNAL(triggered()), this, SLOT(AddCuttingPlane()));
	connect(reset_trackball_action_, SIGNAL(triggered()), this, SLOT(ResetTrackball()));
	connect(delete_plane_action_, SIGNAL(triggered()), this, SLOT(DeleteCuttingPlane()));
}


/**
	添加切分平面
*/
void ModelArea::AddCuttingPlane() {
	for (int i = 0; i < cut_planes_.size() - 1; i++) {
		if (context_menu_y_ > cut_planes_[i] && context_menu_y_ < cut_planes_[i + 1]) {
			cut_planes_.insert(cut_planes_.begin() + i+1, context_menu_y_);
			cut_radii_.insert(cut_radii_.begin() + i+1, CalIntersectRadius(context_menu_y_));
			break;
		}
	}
	update();	//刷新控件内容
}


/**
	重置trackball,即重置控件的旋转和缩放,使模型回到初始状态
*/
void ModelArea::ResetTrackball() {
	trackball_.SetIdentity();
	update();
}


/**
	计算Face被平行于xoz的平面切分后的结果，保存在faces里
	
	参数：
		face  ：要切分的三角面片
	    yValue：平行于xoz的平面
	
	返回值：
		faces：切分后得到的三角面片
*/
void ModelArea::CutPlaneY(const Face& face, const double yValue, vector<Face>& faces) {

	double EPSILON = 1.0e-8;
	Vertex vertexA = face.VertexA();
	Vertex vertexB = face.VertexB();
	Vertex vertexC = face.VertexC();


	///A,B,C三个顶点是否位于平面上
	bool freeA = abs(face.VertexA().Y() - yValue) > EPSILON ? true : false;
	bool freeB = abs(face.VertexB().Y() - yValue) > EPSILON ? true : false;
	bool freeC = abs(face.VertexC().Y() - yValue) > EPSILON ? true : false;

	bool upA = freeA&&(face.VertexA().Y() > yValue?true:false);
	bool downA = freeA && (face.VertexA().Y() < yValue?true:false);
	bool onA = !freeA;

	bool upB = freeB&&(face.VertexB().Y() > yValue ? true : false);
	bool downB =freeB&&(face.VertexB().Y() < yValue ? true : false);
	bool onB = !freeB;


	bool upC = freeC&&(face.VertexC().Y() > yValue ? true : false);
	bool downC =freeC&&(face.VertexC().Y() < yValue ? true : false);
	bool onC = !freeC;

	///A,B,C都在同一侧
	if ((upA&&upB&&upC) || (downA&&downB&&downC)) {
		faces.push_back(face);
		return;
	}
	else if (onA && onB && onC) {
		faces.push_back(face);
		return;
	}
	///两个顶点在平面上
	else if ((onA&&onB&&downC)||(onA&&onB&&upC)) {
		faces.push_back(face);
		return;
	}
	else if ((onA&&onC&&downB) || (onA&&onB&&upB)) {
		faces.push_back(face);
		return;
	}
	else if ((onB&&onC&&downA) || (onB&&onC&&upA)) {
		faces.push_back(face);
		return;
	}

	///一个顶点在平面上
	else if ((onA && upB && upC) || (onA && downB && downC)) {
		faces.push_back(face);
		return;
	}
	else if ((onB && upA && upC) || (onB && downA && downC)) {
		faces.push_back(face);
		return;
	}
	else if ((onC && upA && upB) || (onC && downA && downB)) {
		faces.push_back(face);
		return;
	}

	else if ((onA && upB && downC) || (onA && downB && upC)) {
		Vertex vertexBC = face.VertexC() - face.VertexB();
		Texture textureBC = face.TextureC() - face.TextureB();
		Vertex vertexD = face.VertexB() + vertexBC * (yValue - face.VertexB().Y()) / vertexBC.Y();
		Texture textureD = face.TextureB() + textureBC*(yValue - face.VertexB().Y()) / vertexBC.Y();
		faces.push_back(Face(face.VertexA(), vertexD, face.VertexB(),  face.TextureA(), textureD, face.TextureB()));
		faces.push_back(Face(face.VertexA(), face.VertexC(), vertexD, face.TextureA(), face.TextureC(), textureD));
		return;
	}
	else if ((onB && upA && downC) || (onB && downA && upC)) {
		Vertex vertexAC = face.VertexC() - face.VertexA();
		Texture textureAC = face.TextureC() - face.TextureA();
		Vertex vertexD = face.VertexA() + vertexAC * (yValue - face.VertexA().Y()) / vertexAC.Y();
		Texture textureD = face.TextureA() + textureAC*(yValue - face.VertexA().Y()) / vertexAC.Y();
		faces.push_back(Face(face.VertexA(), vertexD, face.VertexB(),  face.TextureA(), textureD, face.TextureB()));
		faces.push_back(Face(vertexD, face.VertexC(), face.VertexB(), textureD,  face.TextureC(),   face.TextureB()));
		return;
	}
	else if ((onC && upA && downB) || (onC && downA && upB)) {
		Vertex vertexAB = face.VertexB() - face.VertexA();
		Texture textureAB = face.TextureB() - face.TextureA();
		Vertex vertexD = face.VertexA() + vertexAB * (yValue - face.VertexA().Y()) / vertexAB.Y();
		Texture textureD = face.TextureA() + textureAB*(yValue - face.VertexA().Y()) / vertexAB.Y();
		faces.push_back(Face(face.VertexB(), face.VertexC(), vertexD, face.TextureB(), face.TextureC(), textureD));
		faces.push_back(Face(face.VertexA(), vertexD, face.VertexC(), face.TextureA(), textureD, face.TextureC()));
		return;
	}


	///三个顶点分别位于平面两侧
	else if ((upA && downB && downC) || (downA && upB && upC)) {
		Vertex vertexAB = face.VertexB() - face.VertexA();
		Vertex vertexAC = face.VertexC() - face.VertexA();
		Texture textureAB = face.TextureB() - face.TextureA();
		Texture textureAC = face.TextureC() - face.TextureA();

		Vertex vertexD = face.VertexA() + vertexAB * (yValue - face.VertexA().Y()) / vertexAB.Y();
		Vertex vertexE = face.VertexA() + vertexAC * (yValue - face.VertexA().Y()) / vertexAC.Y();
		Texture textureD = face.TextureA() + textureAB * (yValue - face.VertexA().Y()) / vertexAB.Y();
		Texture textureE = face.TextureA() + textureAC * (yValue - face.VertexA().Y()) / vertexAC.Y();

		faces.push_back(Face(face.VertexA(), vertexD, vertexE, face.TextureA(), textureD, textureE));
		faces.push_back(Face(face.VertexB(), face.VertexC(), vertexD, face.TextureB(), face.TextureC(), textureD));
		faces.push_back(Face(vertexE, vertexD, face.VertexC(), textureE, textureD, face.TextureC()));
		return;
	}
	else if ((downA && upB && downC) || (upA && downB && upC)) {
		Vertex vertexBA = face.VertexA() - face.VertexB();
		Vertex vertexBC = face.VertexC() - face.VertexB();
		Texture textureBA = face.TextureA() - face.TextureB();
		Texture textureBC = face.TextureC() - face.TextureB();

		Vertex vertexD = face.VertexB() + vertexBA * (yValue - face.VertexB().Y()) / vertexBA.Y();
		Vertex vertexE = face.VertexB() + vertexBC * (yValue - face.VertexB().Y()) / vertexBC.Y();
		Texture textureD = face.TextureB() + textureBA * (yValue - face.VertexB().Y()) / vertexBA.Y();
		Texture textureE = face.TextureB() + textureBC * (yValue - face.VertexB().Y()) / vertexBC.Y();

		faces.push_back(Face(face.VertexB(), vertexE, vertexD, face.TextureB(), textureE, textureD));
		faces.push_back(Face(face.VertexA(), vertexD,vertexE, face.TextureA(), textureD, textureE));
		faces.push_back(Face(face.VertexC(), face.VertexA(), vertexE, face.TextureC(), face.TextureA(), textureE));
		return;
	}
	else if ((downA && downB && upC) || (upA && upB && downC)) {
		Vertex vertexCB = face.VertexB() - face.VertexC();
		Vertex vertexCA = face.VertexA() - face.VertexC();
		Texture textureCB = face.TextureB() - face.TextureC();
		Texture textureCA = face.TextureA() - face.TextureC();

		Vertex vertexD = face.VertexC() + vertexCA * (yValue - face.VertexC().Y()) / vertexCA.Y();
		Vertex vertexE = face.VertexC() + vertexCB * (yValue - face.VertexC().Y()) / vertexCB.Y();
		Texture textureD = face.TextureC() + textureCA * (yValue - face.VertexC().Y()) / vertexCA.Y();
		Texture textureE = face.TextureC() + textureCB * (yValue - face.VertexC().Y()) / vertexCB.Y();

		faces.push_back(Face(face.VertexC(), vertexD, vertexE, face.TextureC(), textureD, textureE));
		faces.push_back(Face(face.VertexA(), face.VertexB(), vertexD, face.TextureA(), face.TextureB(), textureD));
		faces.push_back(Face(vertexE, vertexD, face.VertexB(), textureE, textureD, face.TextureB()));
		return;
	}
	else {
		faces.push_back(face);
		return;
	}
}




/**
	计算Face被平行于xoz的平面切分后的结果，保存在集合faces里
	
	参数：
		face  ：要切分的三角面片
		yValue：平行于xoz平面的平面

	返回值：faces
*/
void ModelArea::CutPlaneX(const Face& face, const double xValue, vector<Face>& faces) {


	double EPSILON = 1.0e-8;

	///A,B,C三个顶点是否位于平面上
	bool freeA = abs(face.VertexA().X() - xValue) > EPSILON ? true : false;
	bool freeB = abs(face.VertexB().X() - xValue) > EPSILON ? true : false;
	bool freeC = abs(face.VertexC().X() - xValue) > EPSILON ? true : false;

	bool upA = face.VertexA().X() > xValue ? true : false;
	bool downA = face.VertexA().X() < xValue ? true : false;
	bool onA = !freeA;

	bool upB = face.VertexB().X() > xValue ? true : false;
	bool downB =face.VertexB().X() < xValue ? true : false;
	bool onB = !freeB;


	bool upC = face.VertexC().X() > xValue ? true : false;
	bool downC = face.VertexC().X() < xValue ? true : false;
	bool onC = !freeC;

	///A,B,C都在同一侧
	if ((upA&&upB&&upC) || (downA&&downB&&downC)) {
		faces.push_back(face);
		return;
	}
	else if (onA && onB && onC) {
		faces.push_back(face);
		return;
	}
	///两个顶点在平面上
	else if ((onA&&onB&&downC) || (onA&&onB&&upC)) {
		faces.push_back(face);
		return;
	}
	else if ((onA&&onC&&downB) || (onA&&onB&&upB)) {
		faces.push_back(face);
		return;
	}
	else if ((onB&&onC&&downA) || (onB&&onC&&upA)) {
		faces.push_back(face);
		return;
	}

	///一个顶点在平面上
	else if ((onA && upB && upC) || (onA && downB && downC)) {
		faces.push_back(face);
		return;
	}
	else if ((onB && upA && upC) || (onB && downA && downC)) {
		faces.push_back(face);
		return;
	}
	else if ((onC && upA && upB) || (onC && downA && downB)) {
		faces.push_back(face);
		return;
	}

	else if ((onA && upB && downC) || (onA && downB && upC)) {
		Vertex vertexBC = face.VertexC() - face.VertexB();
		Texture textureBC = face.TextureC() - face.TextureB();
		Vertex vertexD = face.VertexB() + vertexBC * (xValue - face.VertexB().X()) / vertexBC.X();
		Texture textureD = face.TextureB() + textureBC*(xValue - face.VertexB().X()) / vertexBC.X();
		faces.push_back(Face(face.VertexA(), vertexD, face.VertexB(), face.TextureA(), textureD, face.TextureB()));
		faces.push_back(Face(face.VertexA(), face.VertexC(), vertexD, face.TextureA(), face.TextureC(), textureD));
		return;
	}
	else if ((onB && upA && downC) || (onB && downA && upC)) {
		Vertex vertexAC = face.VertexC() - face.VertexA();
		Texture textureAC = face.TextureC() - face.TextureA();
		Vertex vertexD = face.VertexA() + vertexAC * (xValue - face.VertexA().X()) / vertexAC.X();
		Texture textureD = face.TextureA() + textureAC*(xValue - face.VertexA().X()) / vertexAC.X();
		faces.push_back(Face(face.VertexA(), vertexD, face.VertexB(), face.TextureA(), textureD, face.TextureB()));
		faces.push_back(Face(vertexD, face.VertexC(), face.VertexB(), textureD, face.TextureC(), face.TextureB()));
		return;
	}
	else if ((onC && upA && downB) || (onC && downA && upB)) {
		Vertex vertexAB = face.VertexB() - face.VertexA();
		Texture textureAB = face.TextureB() - face.TextureA();
		Vertex vertexD = face.VertexA() + vertexAB * (xValue - face.VertexA().X()) / vertexAB.X();
		Texture textureD = face.TextureA() + textureAB*(xValue - face.VertexA().X()) / vertexAB.X();
		faces.push_back(Face(face.VertexB(), face.VertexC(), vertexD, face.TextureB(), face.TextureC(), textureD));
		faces.push_back(Face(face.VertexA(), vertexD, face.VertexC(), face.TextureA(), textureD, face.TextureC()));
		return;
	}


	///三个顶点分别位于平面两侧
	else if ((upA && downB && downC) || (downA && upB && upC)) {
		Vertex vertexAB = face.VertexB() - face.VertexA();
		Vertex vertexAC = face.VertexC() - face.VertexA();
		Texture textureAB = face.TextureB() - face.TextureA();
		Texture textureAC = face.TextureC() - face.TextureA();

		Vertex vertexD = face.VertexA() + vertexAB * (xValue - face.VertexA().X()) / vertexAB.X();
		Vertex vertexE = face.VertexA() + vertexAC * (xValue - face.VertexA().X()) / vertexAC.X();
		Texture textureD = face.TextureA() + textureAB * (xValue - face.VertexA().X()) / vertexAB.X();
		Texture textureE = face.TextureA() + textureAC * (xValue - face.VertexA().X()) / vertexAC.X();

		faces.push_back(Face(face.VertexA(), vertexD, vertexE, face.TextureA(), textureD, textureE));
		faces.push_back(Face(face.VertexB(), face.VertexC(), vertexD, face.TextureB(), face.TextureC(), textureD));
		faces.push_back(Face(vertexE, vertexD, face.VertexC(), textureE, textureD, face.TextureC()));
		return;
	}
	else if ((downA && upB && downC) || (upA && downB && upC)) {
		Vertex vertexBA = face.VertexA() - face.VertexB();
		Vertex vertexBC = face.VertexC() - face.VertexB();
		Texture textureBA = face.TextureA() - face.TextureB();
		Texture textureBC = face.TextureC() - face.TextureB();

		Vertex vertexD = face.VertexB() + vertexBA * (xValue - face.VertexB().X()) / vertexBA.X();
		Vertex vertexE = face.VertexB() + vertexBC * (xValue - face.VertexB().X()) / vertexBC.X();
		Texture textureD = face.TextureB() + textureBA * (xValue - face.VertexB().X()) / vertexBA.X();
		Texture textureE = face.TextureB() + textureBC * (xValue - face.VertexB().X()) / vertexBC.X();

		faces.push_back(Face(face.VertexB(), vertexE, vertexD, face.TextureB(), textureE, textureD));
		faces.push_back(Face(face.VertexA(), vertexD, vertexE, face.TextureA(), textureD, textureE));
		faces.push_back(Face(face.VertexC(), face.VertexA(), vertexE, face.TextureC(), face.TextureA(), textureE));
		return;
	}
	else if ((downA && downB && upC) || (upA && upB && downC)) {
		Vertex vertexCB = face.VertexB() - face.VertexC();
		Vertex vertexCA = face.VertexA() - face.VertexC();
		Texture textureCB = face.TextureB() - face.TextureC();
		Texture textureCA = face.TextureA() - face.TextureC();

		Vertex vertexD = face.VertexC() + vertexCA * (xValue - face.VertexC().X()) / vertexCA.X();
		Vertex vertexE = face.VertexC() + vertexCB * (xValue - face.VertexC().X()) / vertexCB.X();
		Texture textureD = face.TextureC() + textureCA * (xValue - face.VertexC().X()) / vertexCA.X();
		Texture textureE = face.TextureC() + textureCB * (xValue - face.VertexC().X()) / vertexCB.X();

		faces.push_back(Face(face.VertexC(), vertexD,vertexE, face.TextureC(), textureD, textureE));
		faces.push_back(Face(face.VertexA(), face.VertexB(), vertexD, face.TextureA(), face.TextureB(), textureD));
		faces.push_back(Face(vertexE, vertexD, face.VertexB(), textureE, textureD, face.TextureB()));
		return;
	}
	else {
		faces.push_back(face);
		return;
	}
}




/**
	使用切分平面对三维模型进行切割，并对切割后的片段使用圆台进行拟合展开
	
	返回值：
		拟合展开的片段集合
*/
vector<EnrolledSlice> ModelArea::CutInConicalMode() {
	if (cut_planes_.size() == 0) {
		ComputeBestCuts();
	}

	///获取三角面片集合
	vector<Face> faces;

	for (auto& tri:model_->mTriangles) {
		int* vindices = tri.Vindices();
		int* tindices = tri.Tindices();

		faces.push_back(
			Face(model_->mVertices[vindices[0]], model_->mVertices[vindices[1]], model_->mVertices[vindices[2]],
			model_->mTexcoods[tindices[0]], model_->mTexcoods[tindices[1]], model_->mTexcoods[tindices[2]])
		);
	}
	
	

	///计算三角面片被切分平面和yoz平面切分后的三角面片
	vector<Face> cuttedFaces;
	for (int i = 0; i < faces.size(); i++) {
		CutPlaneX(faces[i], 0.0, cuttedFaces);
	}
	faces.clear();
	faces = cuttedFaces;
	cuttedFaces.clear();

	for (int i = 0; i < cut_planes_.size(); i++) {
		for (int j = 0; j < faces.size(); j++) {
			CutPlaneY(faces[j], cut_planes_[i], cuttedFaces);
		}
		faces.clear();
		faces = cuttedFaces;
		cuttedFaces.clear();
	}


	///初始化slices，并将切分后的三角面片分为不同的slice
	vector<Slice> slices;
	for (int i = 0; i < cut_planes_.size() - 1; i++) {
		slices.push_back(Slice(cut_planes_[i], cut_planes_[i + 1], cut_radii_[i], cut_radii_[i + 1]));
	}

	for (int i = 0; i < faces.size(); i++) {
		int up = IsFaceUpperYOZ(faces[i]);
		if (up == 1){
			faces[i].SetPositive(true);
		}
		else {
			faces[i].SetPositive(false);
		}
		for (int j = 0; j < slices.size(); j++) {
			if (slices[j].ContainFace(faces[i])) {
				slices[j].AddFace(faces[i]);
				break;
			}
		}
	}


	///分别对每一个Slice进行展开，得到展开后的EnrolledSlice
	vector<EnrolledSlice> enrolled_slices;
	for (auto& slice:slices) {
		enrolled_slices.push_back(slice.EnrollSliceAsConial());
	}
	return enrolled_slices;
}



/**
	使用切分平面对三维模型进行切割，并对切割后的片段使用圆柱进行拟合展开
	
	返回值：
		拟合展开的片段集合
*/
vector<EnrolledSlice> ModelArea::CutInCylinderMode() {
	
	///获取三角面片集合
	vector<Face> faces;
	for (auto& tri : model_->mTriangles) {
		int* vindices = tri.Vindices();
		int* tindices = tri.Tindices();

		faces.push_back(
			Face(model_->mVertices[vindices[0]], model_->mVertices[vindices[1]], model_->mVertices[vindices[2]],
				model_->mTexcoods[tindices[0]], model_->mTexcoods[tindices[1]], model_->mTexcoods[tindices[2]])
		);
	}

	///计算三角面片被切分平面和yoz平面切分后的三角面片
	vector<Face> cuttedFaces;
	for (int i = 0; i < faces.size(); i++) {
		CutPlaneX(faces[i], 0.0, cuttedFaces);
	}
	faces.clear();
	faces = cuttedFaces;
	cuttedFaces.clear();

	for (int i = 0; i < cut_planes_.size(); i++) {
		for (int j = 0; j < faces.size(); j++) {
			CutPlaneY(faces[j], cut_planes_[i], cuttedFaces);
		}
		faces.clear();
		faces = cuttedFaces;
		cuttedFaces.clear();
	}


	///初始化slices，并将切分后的三角面片分为不同的slice
	vector<Slice> slices;
	for (int i = 0; i < cut_planes_.size() - 1; i++) {
		slices.push_back(Slice(cut_planes_[i], cut_planes_[i + 1], cut_radii_[i], cut_radii_[i + 1]));
	}

	for (int i = 0; i < faces.size(); i++) {
		int up = IsFaceUpperYOZ(faces[i]);
		if (up == 1) {
			faces[i].SetPositive(true);
		}
		else {
			faces[i].SetPositive(false);
		}
		for (int j = 0; j < slices.size(); j++) {
			if (slices[j].ContainFace(faces[i])) {
				slices[j].AddFace(faces[i]);
				break;
			}
		}
	}


	///分别对每一个Slice进行展开，得到展开后的EnrolledSlice
	vector<EnrolledSlice> enrolled_slices;
	for (auto &slice:slices) {
		enrolled_slices.push_back(slice.EnrollSliceAsCylinder());
	}
	return enrolled_slices;
}


/**
	删除选中后的切分平面
*/
void ModelArea::DeleteCuttingPlane() {
	cut_planes_.erase(cut_planes_.begin() + selected_cut_plane_);
	cut_radii_.erase(cut_radii_.begin() + selected_cut_plane_);
	selected_cut_plane_ = -1;
}


/**
	合并没有明显纹理特征的切分片段，主要依据是每一段的纹理平均值以及拟合半径

*/
void ModelArea::CombineRegions(vector<double>& gradients,vector<double>& average, 
	vector<double>& radii,vector<double>& cutPlanes,vector<double>& cutRadius) {
	vector<int> not_cut_index;
	for (int i = 0; i < average.size(); i++) {
		///最下面的一个切分片段纹理特征不明显
		if (i == 0 && average[i] < texture_threshold_) {
			not_cut_index.push_back(1);
		}

		///中间的切分片段纹理特征不明显
		if ((i >= 1) && (i < average.size() - 1) && (average[i] < texture_threshold_)) {
			if (abs(radii[i] - radii[i - 1]) < abs(radii[i] - radii[i - 1])) {
				not_cut_index.push_back(i);
			}
			else {
				not_cut_index.push_back(i + 1);
			}
		}
		
		///最上面的切分片段纹理特征不明显
		if (i == average.size() - 1 && average[i] < texture_threshold_) {
			not_cut_index.push_back(average.size() - 1);
		}
	}
	for (auto iter = not_cut_index.rbegin(); iter != not_cut_index.rend();++iter) {
		cutPlanes.erase(cutPlanes.begin() + *iter);
		cutRadius.erase(cutRadius.begin() + *iter);
	}
}



/**
	切换“是否需要合并区域”的选项
*/
void ModelArea::ResetCombine() {
	if (combine_region_) {
		combine_region_ = false;
	}
	else {
		combine_region_ = true;
	}
	//ComputeBestCuts();
}