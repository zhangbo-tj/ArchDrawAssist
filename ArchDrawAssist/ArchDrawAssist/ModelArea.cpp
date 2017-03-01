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
	���캯��,����ʼ���ؼ�����
	�ú���Ϊ�����
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
	InitActions();	//��ʼ�����ڵĿؼ���������Ҽ��ļ�������
}




/**
	��������
*/
ModelArea::~ModelArea(){
}




/**
	��ʼ���ؼ��������������������ա��켣���
	�ú����Ǳ���ģ�ÿ�����ɿؼ�ʱֻ����һ��
*/
void ModelArea::initializeGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClearColor(178.0 / 255, 178.0 / 255, 178.0 / 255, 0);	//���ÿؼ�������ɫ

	//glClearColor(1.0, 1.0, 1.0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);

	///����
	GLfloat light_model_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_direction[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);

	///���ÿؼ��й켣�����
	trackball_.center = vcg::Point3f(0, 0, 0);
	trackball_.radius = 1;
	trackball_.GetView();
	trackball_.Apply();
}




/**
	�ú���Ϊ���ؼ���С�����ı�ʱҪ���еĲ�������Ҫ��������ͶӰģʽ�Ի�ø��õĹ۲���άģ�͵��ӽ�
	�ú����Ǳ����

	������
		width : �ؼ��µĿ��
		height: �ؼ��ĸ߶�
*/
void ModelArea::resizeGL(int width, int height) {
	glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	//���ò����Ե�������ӽ�
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
	�ؼ���ʵ�ʻ������ݵĺ�������������ʵʱ���Ƹ��ָı������
	�ú����Ǳ���ģ���ÿ�����κβ���ʱ���ᱻ����һ��
*/
void ModelArea::paintGL() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0, 0, -1);
	glPushMatrix();

	///ʹ��trackball
	trackball_.GetView();
	trackball_.Apply();

	///�ж��Ƿ�����ģ��
 	if (model_) {

		///�ж��Ƿ���Ҫ��תģ���Լ�������ͼ
 		if (need_rotate_) {
 			glClearColor(0.9, 0.9, 0.9, 0);
 			string image_name = GLMDirName(model_->mPathname);
 			QPixmap pixmap;

 			///��ȡģ�ͺ�����תһȦ���ȡ�ĸ�����������ͼ�����ڼ����з�ƽ��
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
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);		//����ģ��
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);		//��ȡ��ͼ
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//�����ͼ
 				break;
 			case 1:
 				image_name += "image1.bmp";
 				glRotated(90, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//����ģ��
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//��ȡ��ͼ
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//�����ͼ
 				break;
 			case 2:
 				image_name += "image2.bmp";
 				glRotated(180, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//����ģ��
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//��ȡ��ͼ
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//�����ͼ
 				break;
 			case 3:
 				image_name += "image3.bmp";
 				glRotated(270, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//����ģ��
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//��ȡ��ͼ
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//�����ͼ
 				break;
 			case 4:
 				image_name += "image4.bmp";
 				//glRotated(270, 0, 1, 0);
 				glColor3f(1, 1, 1);
 				GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//����ģ��
 				pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
 					mapToGlobal(pos()).x(),
 					mapToGlobal(pos()).y(),
 					frameGeometry().width() - 10,
 					frameGeometry().height() - 10);	//��ȡ��ͼ
 				pixmap.save(QString(QString::fromLocal8Bit(image_name.c_str())), "bmp");	//�����ͼ
 				break;
 			default:
 				break;
 			}
 			paint_num_++;
 			if (paint_num_ == 5) {
 				need_rotate_ = false;
 			}
 		}
		///������תģ�ͣ�����������ģ�ͼ���
 		else {
			glClearColor(178.0 / 255, 178.0 / 255, 178.0 / 255, 0);		//���ÿؼ�������ɫ
 			//glClearColor(0.9,0.9,0.9, 0);
 			glColor3f(1, 1, 1);
 			GLMDraw(model_, GLM_SMOOTH | GLM_TEXTURE);	//����ģ��
 			///�����и�ƽ��
 			DrawCutPlanes();
 		}
 	}
 	glPopMatrix();
	
}


/**
	������άģ�ͣ��������з�ƽ��

	������
		model: ��άģ��
*/
void ModelArea::LoadModel(GLModel* model) {
	model_ = model;		//����ģ��
	ReadMtlFile(model_);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -3, 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	///���ÿؼ�����
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
	

	trackball_.Reset();		//���ù켣��Ĳ���
	update();	//���¿ؼ�������
}




/**
	��ȡMTL�ļ�����������
*/
void ModelArea::ReadMtlFile(GLModel* glmModel) {
	FILE* file;
	char* filename;
	char buf[128];
	char buf1[128];
	int materialNum = 0;
	char* map_file;

	///��ȡģ��Ŀ¼����MTL�ļ���
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
	��ȡ�����ļ�������ģ�͵�����
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
	��Qt�������¼�ת��ΪVCG���ڵ��������¼�
	ԭ����ʹ�õ�trackballΪVCG�����trackball

	������
		qtbt     : Qt����¼�
		modifiers: Qt�����¼�
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
	�������¼������ڴ��ݸ��켣��
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
	����ƶ��¼��������ƶ��и�ƽ��
*/
void ModelArea::mouseMoveEvent(QMouseEvent *event) {
	if (selected_cut_plane_ != -1 && right_button_pressed) {
		MovePlane(event->x(), height() - event->y());	//�ƶ��з�ƽ��
	}
	else {
		trackball_.MouseMove(event->x(), height() - event->y());
	}
	
	update();
}


/**
	������¼�������������Ҽ�ѡ�������Ӧ�Ĳ���
*/
void ModelArea::mousePressEvent(QMouseEvent* event){
	if (event->button() == Qt::RightButton) {
		GetSelectedPlane(event->x(), height() - event->y());	//��ȡ�з�ƽ��

		right_button_pressed = true;
	}
	else {
		trackball_.MouseDown(event->x(), height() - event->y(), QT2VCG(event->button(), event->modifiers()));
	}
	
	update();
}

/**
	�ɿ���갴�����¼�
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
	��Qt�����¼�ת��ΪVCG�����¼�
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
	����Ҽ��¼������������Ҽ��˵�
*/
void ModelArea::contextMenuEvent(QContextMenuEvent* event) {
	///δѡ���з�ƽ��
	if (selected_cut_plane_ == -1) {
		right_button_menu_->clear();
		right_button_menu_->addAction(add_plane_action_);	//����з�ƽ��
		right_button_menu_->addAction(reset_trackball_action_);	//���ù켣��
		right_button_menu_->exec(QCursor::pos());
	}
	///��ѡ���з�ƽ��
	else {
		right_button_menu_->clear();
		right_button_menu_->addAction(delete_plane_action_);	//ɾ���з�ƽ��
		right_button_menu_->addAction(reset_trackball_action_);	//���ù켣��
		right_button_menu_->exec(QCursor::pos());
	}
	event->accept();
}


/**
	������ά����ϵ
*/
void ModelArea::DrawXYZ() {
	glPushMatrix();
	glLineWidth(2);
	float fCursor[4];
	glGetFloatv(GL_CURRENT_COLOR, fCursor);	//��ȡ��ǰ��ɫ

	///x��
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); //X
	glVertex3f(0, 0, 0);
	glVertex3f(0.2, 0, 0);


	///y��
	glColor3f(0.0, 1.0, 0.0);//Y
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.2, 0);

	///z��
	glColor3f(0.0, 0.0, 1.0);//Z
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.2);
	glEnd();

	glColor4fv(fCursor);
	glLineWidth(1.0);
	glPopMatrix();
}

/**
	�Ƚϴ�С�������������
*/
bool compare(int x, int y) {
	return x < y;
}



/**
	������άģ��4���������ͶӰ�����з�ƽ���λ��
*/
void ModelArea::ComputeBestCuts() {

	//������ز���
	cut_planes_.clear();
	cut_radii_.clear();
	selected_cut_plane_ = -1;
	right_button_pressed = false;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -3, 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	///����trackball,��������ͼ
	trackball_.Reset();
	paint_num_ = -1;
	need_rotate_ = true;

	///�ֱ��ȡģ����4�����������ͼ
	repaint();
	repaint();
	repaint();
	repaint();
	repaint();
	repaint();

	
	vector<double> gradients;
	ComputeGradients(gradients);	//����4������ͼ���ݶȺ�	
	std::reverse(gradients.begin(), gradients.end());

	int startCol;
	int endCol;

	///�����ݶ�ֵ��Ϊ0������
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


	///�����ݶ�ֵС���ٽ�ֵ����
	vector<double> small_gradients;
	for (int i = 0; i < gradients.size(); ++i) {
		if (gradients[i] <= texture_threshold_) {	///���ٽ�ֵ���ڲ�ͬ��ģ�Ͷ���ͬ
			small_gradients.push_back(i);
		}
	}

	///�����з�ƽ�����ڵ���
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


	///�����з�ƽ����ģ���е�λ�ã��Լ��зֺ�õ��İ뾶
	for (int i = 0; i < cut_cols.size(); ++i) {
		double scale = (cut_cols[i] - startCol) / (static_cast<double>(colNum));
		double cur_plane =  scale * 2-1;
		cut_planes_.push_back(cur_plane);
		cut_radii_.push_back(CalIntersectRadius(cur_plane));
	}


	///�ж��Ƿ���Ҫ�ϲ�����
	if(combine_region_) {
		vector<double> average;
		
		///����ÿһ�ε��ݶ�ƽ��ֵ
		for (int i = 0; i < cut_cols.size() - 1; i++) {
			int sum = std::accumulate(gradients.begin() + cut_cols[i], gradients.begin() + cut_cols[i + 1], 0);
			average.push_back(sum / (static_cast<double>(cut_cols[i + 1] - cut_cols[i])));
		}

		///����άģ���е�����������Ƭת��ΪFaces
		vector<Face> faces;
		for (auto tri : model_->mTriangles) {
			int* vindices = tri.Vindices();
			int* tindices = tri.Tindices();
			faces.push_back(Face(model_->mVertices[vindices[0]], model_->mVertices[vindices[1]], model_->mVertices[vindices[2]],
				model_->mTexcoods[tindices[0]], model_->mTexcoods[tindices[1]], model_->mTexcoods[tindices[2]]));
		}

		///ʹ��yozƽ���������Ƭ���Ͻ����и������CuttedFaces��
		vector<Face> cuttedFaces;
		for (auto face : faces) {
			CutPlaneX(face, 0.0, cuttedFaces);
		}
		faces.clear();
		faces = cuttedFaces;
		cuttedFaces.clear();

		///ʹ���и�ƽ���������Ƭ�����и������CuttedFaces��
		for (auto plane : cut_planes_) {
			for (auto face : faces) {
				CutPlaneY(face, plane, cuttedFaces);
			}
			faces.clear();
			faces = cuttedFaces;
			cuttedFaces.clear();
		}
		
		///����Slices
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


		///����ÿ�����ֵ�Բ̨��ϵİ뾶
		vector<double> radii;
		for (auto slice : slices) {
			slice.ConialFrustumFitting();
			radii.push_back(slice.MaxRadius());
		}
		///���и�����������
		CombineRegions(gradients, average, radii, cut_planes_,cut_radii_);
	}

	///������Ϣ���Ѿ���������з�ƽ��
	QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�з�ƽ��������"));


	update();	//���ƿؼ�
}



/**
	ʹ��opencv������άģ��4�����������ͼ���ݶ�֮�ͣ�������gradients��

	������
		gradients: Ҫ������ݶȼ���
*/
void ModelArea::ComputeGradients(vector<double>& gradients) {
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat src_gray, dst, grad_x;
	vector<Mat> src;

	///��ȡ4������ͼ���������ݶ�ֵ
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

	///���ݶ�ֵ�������ļ��У����Ժ�����
	string gradientFile = GLMDirName(model_->mPathname);
	gradientFile += "gradient.txt";
	std::ofstream ofs;
	ofs.open(gradientFile);
	for (auto i = 0; i < gradients.size(); ++i) {
		ofs << gradients[i] << endl;
	}
	ofs.close();	//�ر��ļ�
	return;
}


/**
	���ҵ��з�ƽ����ģ�͵Ľ��㣬Ȼ�������Щ������Y��ľ������뾶��
	���ڸð뾶��Ҫ����ʵʱ��ʾ���������С������Ҫ�ر�ȷ

	������
		yValue: �з�ƽ��

	����ֵ��
		�뾶ֵ
*/
double ModelArea::CalIntersectRadius(double yValue) {
	Triangle tri;
	Vertex vertexA, vertexB, vertexC;
	vector<Vertex> interPoints;

	///�����з�ƽ����ģ�͵Ľ���
	for (int i = 0; i < model_->mTriangles.size(); i++) {
		tri = model_->mTriangles[i];
		int* vindices = tri.Vindices();
		vertexA = model_->mVertices[vindices[0]];
		vertexB = model_->mVertices[vindices[1]];
		vertexC = model_->mVertices[vindices[2]];
		interPoints.clear();
		if (TriInterPlane(vertexA, vertexB, vertexC, yValue,interPoints) > 0) {
			double sum = 0.0;
			///���ݽ��㵽Y��ľ�����Ϊ�뾶
			for (int i = 0; i < interPoints.size(); ++i) {
				sum += sqrt(interPoints[i].X()*interPoints[i].X() + interPoints[i].Z()*interPoints[i].Z());
			}
			return sum / interPoints.size();
		}
	}
	return 0;
}




/**
	����������Ƭ��ƽ��Ľ��������Լ���������
	vertexA,vertexB,vertexC�ֱ��Ǹ�������Ƭ����������
	interPoints���ǽ�������

	������
		vertexA, vertexB, vertexC: ������Ƭ����������
		yValue                   : �з�ƽ��

	����ֵ��
		interPoints: ��������
*/
int ModelArea::TriInterPlane(const Vertex& vertexA, const Vertex& vertexB, const Vertex& vertexC, 
	const double yValue,vector<Vertex>& interPoints) {
	double EPSILON = 1.0e-8;

	///A,B,C���������Ƿ�λ��ƽ����
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

	///A,B,C����ͬһ��
	if ((upA&&upB&&upC) || (downA&&downB&&downC)) {
		return 0;
	}
	else if (onA&& onB&& onC) {
		interPoints.push_back(vertexA);
		interPoints.push_back(vertexB);
		interPoints.push_back(vertexC);
		return 3;
	}
	
	///����������ƽ����
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

	///һ��������ƽ����
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


	///��������ֱ�λ��ƽ������
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
	�����߶���ƽ��Ľ���
	
	������
		vertexA,vertexB: �߶ε������˵�
		yValue         : �з�ƽ��

	����ֵ��
		��������ֵ
*/
Vertex ModelArea::LinesegInterPlane(const Vertex& vertexA, const Vertex& vertexB,const double yValue) {
	Vertex AB = vertexB - vertexA;
	double scale = (yValue - vertexA.Y()) / AB.Y();
	return Vertex(vertexA.X() + AB.X()*scale, yValue, vertexA.Z() + AB.Z()*scale);
}


/**
	�ȶ�Բ������ɢ����Ȼ��������Щ��ɢ�������ɫʵ��Բ
	
	������
		y     : Բ��λ��
		radius�� Բ�İ뾶
*/
void ModelArea::DrawSolidCircleGreen(double y, double radius) {
	int sections = 200;

	double TWOPI = 2 * M_PI;
	glColor3f(0, 1, 0);		//������ɫΪ��ɫ
	glBegin(GL_TRIANGLE_FAN);
	for (int count = 0; count <= sections; count++) {
		glVertex3d(radius*cos(count*TWOPI / sections), y, radius*sin(count *TWOPI / sections));
	}
	glEnd();
}


/**
	�ȶ�Բ������ɢ����Ȼ��������Щ��ɢ����ƺ�ɫʵ��Բ
	
	������
		y     : Բ��λ��
		radius�� Բ�İ뾶
*/
void ModelArea::DrawSolidCircleRed(double y, double radius) {
	int sections = 200;

	double TWOPI = 2 * M_PI;
	glColor3f(1, 0, 0);		//������ɫΪ��ɫ
	glBegin(GL_TRIANGLE_FAN);
	for (int count = 0; count <= sections; count++) {
		glVertex3d(radius*cos(count*TWOPI / sections), y, radius*sin(count *TWOPI / sections));
	}
	glEnd();
}



/**
	�ȶ�Բ������ɢ����Ȼ��������Щ��ɢ����ƺ�ɫ����Բ

	������
		y     : Բ��λ��
		radius�� Բ�İ뾶
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
	�ȶ�Բ������ɢ����Ȼ��������Щ��ɢ�������ɫ����Բ

	������
		y     : Բ��λ��
		radius�� Բ�İ뾶
*/
void ModelArea::DrawWiredCircleGreen(double y, double radius) {
	int count;
	int sections = 200;
	float TWOPI = 2 * M_PI;
	glColor3f(0, 1, 0);		//������ɫ
	glBegin(GL_LINE_STRIP);
	for (count = 0; count <= sections; count++) {
		glVertex3d(radius *cos(count * TWOPI / sections), y,radius * sin(count * TWOPI / sections));
	}
	glEnd();
}


/**
	�����з�ƽ�棬ѡ�е�Բ����ɫ��ʾ��δѡ�е��ú�ɫ��ʾ
*/
void ModelArea::DrawCutPlanes() {
	glLineWidth(1);
	for (int i = 0; i < cut_planes_.size(); ++i) {
		if (i == selected_cut_plane_) {		//��ѡ�е��з�ƽ��
			DrawWiredCircleGreen(cut_planes_[i], cut_radii_[i]);
		}
		else {								//δ��ѡ�е��з�ƽ��
			DrawWiredCircleRed(cut_planes_[i], cut_radii_[i]);
		}
	}
}


/**
	�����λ��ת��Ϊ��Ӧ����������ϵ�µ�����
	
	������	
		mouse_x, mouse_y��Ϊ���λ��

	����ֵ��
		world_x, world_y, world_zΪ��Ӧ����������ϵ�µ�ֵ
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

	///ӳ�䵽��������ϵ
	gluUnProject((GLdouble)mouse_x, (GLdouble)mouse_y, winZ,
		modelView, projection, viewport,
		&world_x, &world_y, &world_z);
}



/**
	��ȡ��ѡ�е��з�ƽ��
	
	������
		mouse_x, mouse_y��ʾ���λ�õ�����
*/
void ModelArea::GetSelectedPlane(int mouse_x, int mouse_y) {
	double world_x, world_y, world_z;
	
	///��ȡ���λ�ö�Ӧ����������ϵ
	GetWorldCoordinates(mouse_x, mouse_y, world_x, world_y, world_z);

	context_menu_x_ = world_x;
	context_menu_y_ = world_y;
	context_menu_z_ = world_z;

	double dis_delta = 0.03;
	
	///��ȡѡ�е��з�ƽ��
	for (int i = 0; i < cut_planes_.size(); ++i) {
		if (abs(cut_planes_[i] - world_y) < dis_delta) {
			selected_cut_plane_ = i;
			return;
		}
	}
	selected_cut_plane_ = -1;
}



/**
	�ƶ�ѡ�е��з�ƽ��

	������
		mouse_x, mouse_y��ʾ���λ�õ�����
*/
void ModelArea::MovePlane(int mouse_x, int mouse_y) {
	double world_x, world_y, world_z;
	///��ȡ���λ�ö�Ӧ����������ϵ�µ�����ֵ
	GetWorldCoordinates(mouse_x, mouse_y, world_x, world_y, world_z);

	///�ƶ�ѡ�е��з�ƽ�沢ʵʱ�����зְ뾶
	if (selected_cut_plane_ != 0 &&
		selected_cut_plane_ != cut_planes_.size() - 1
		&& world_y < cut_planes_[selected_cut_plane_ + 1] &&
		world_y > cut_planes_[selected_cut_plane_ - 1]) {

		cut_planes_[selected_cut_plane_] = world_y;
		cut_radii_[selected_cut_plane_] = CalIntersectRadius(world_y);
	}
}


/**
	��ʼ������Ҽ��˵��Լ���Ӧ�Ĳ���	
*/
void ModelArea::InitActions() {
	///�����˵��Ͳ���
	right_button_menu_ = new QMenu();
	add_plane_action_ = new QAction(QString::fromLocal8Bit("��Ӻ����"), this);
	reset_trackball_action_ = new QAction(QString::fromLocal8Bit("����"), this);
	delete_plane_action_ = new QAction(QString::fromLocal8Bit("ɾ�������"), this);

	///Ϊ��������źŲ�
	connect(add_plane_action_, SIGNAL(triggered()), this, SLOT(AddCuttingPlane()));
	connect(reset_trackball_action_, SIGNAL(triggered()), this, SLOT(ResetTrackball()));
	connect(delete_plane_action_, SIGNAL(triggered()), this, SLOT(DeleteCuttingPlane()));
}


/**
	����з�ƽ��
*/
void ModelArea::AddCuttingPlane() {
	for (int i = 0; i < cut_planes_.size() - 1; i++) {
		if (context_menu_y_ > cut_planes_[i] && context_menu_y_ < cut_planes_[i + 1]) {
			cut_planes_.insert(cut_planes_.begin() + i+1, context_menu_y_);
			cut_radii_.insert(cut_radii_.begin() + i+1, CalIntersectRadius(context_menu_y_));
			break;
		}
	}
	update();	//ˢ�¿ؼ�����
}


/**
	����trackball,�����ÿؼ�����ת������,ʹģ�ͻص���ʼ״̬
*/
void ModelArea::ResetTrackball() {
	trackball_.SetIdentity();
	update();
}


/**
	����Face��ƽ����xoz��ƽ���зֺ�Ľ����������faces��
	
	������
		face  ��Ҫ�зֵ�������Ƭ
	    yValue��ƽ����xoz��ƽ��
	
	����ֵ��
		faces���зֺ�õ���������Ƭ
*/
void ModelArea::CutPlaneY(const Face& face, const double yValue, vector<Face>& faces) {

	double EPSILON = 1.0e-8;
	Vertex vertexA = face.VertexA();
	Vertex vertexB = face.VertexB();
	Vertex vertexC = face.VertexC();


	///A,B,C���������Ƿ�λ��ƽ����
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

	///A,B,C����ͬһ��
	if ((upA&&upB&&upC) || (downA&&downB&&downC)) {
		faces.push_back(face);
		return;
	}
	else if (onA && onB && onC) {
		faces.push_back(face);
		return;
	}
	///����������ƽ����
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

	///һ��������ƽ����
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


	///��������ֱ�λ��ƽ������
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
	����Face��ƽ����xoz��ƽ���зֺ�Ľ���������ڼ���faces��
	
	������
		face  ��Ҫ�зֵ�������Ƭ
		yValue��ƽ����xozƽ���ƽ��

	����ֵ��faces
*/
void ModelArea::CutPlaneX(const Face& face, const double xValue, vector<Face>& faces) {


	double EPSILON = 1.0e-8;

	///A,B,C���������Ƿ�λ��ƽ����
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

	///A,B,C����ͬһ��
	if ((upA&&upB&&upC) || (downA&&downB&&downC)) {
		faces.push_back(face);
		return;
	}
	else if (onA && onB && onC) {
		faces.push_back(face);
		return;
	}
	///����������ƽ����
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

	///һ��������ƽ����
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


	///��������ֱ�λ��ƽ������
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
	ʹ���з�ƽ�����άģ�ͽ����и�����и���Ƭ��ʹ��Բ̨�������չ��
	
	����ֵ��
		���չ����Ƭ�μ���
*/
vector<EnrolledSlice> ModelArea::CutInConicalMode() {
	if (cut_planes_.size() == 0) {
		ComputeBestCuts();
	}

	///��ȡ������Ƭ����
	vector<Face> faces;

	for (auto& tri:model_->mTriangles) {
		int* vindices = tri.Vindices();
		int* tindices = tri.Tindices();

		faces.push_back(
			Face(model_->mVertices[vindices[0]], model_->mVertices[vindices[1]], model_->mVertices[vindices[2]],
			model_->mTexcoods[tindices[0]], model_->mTexcoods[tindices[1]], model_->mTexcoods[tindices[2]])
		);
	}
	
	

	///����������Ƭ���з�ƽ���yozƽ���зֺ��������Ƭ
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


	///��ʼ��slices�������зֺ��������Ƭ��Ϊ��ͬ��slice
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


	///�ֱ��ÿһ��Slice����չ�����õ�չ�����EnrolledSlice
	vector<EnrolledSlice> enrolled_slices;
	for (auto& slice:slices) {
		enrolled_slices.push_back(slice.EnrollSliceAsConial());
	}
	return enrolled_slices;
}



/**
	ʹ���з�ƽ�����άģ�ͽ����и�����и���Ƭ��ʹ��Բ���������չ��
	
	����ֵ��
		���չ����Ƭ�μ���
*/
vector<EnrolledSlice> ModelArea::CutInCylinderMode() {
	
	///��ȡ������Ƭ����
	vector<Face> faces;
	for (auto& tri : model_->mTriangles) {
		int* vindices = tri.Vindices();
		int* tindices = tri.Tindices();

		faces.push_back(
			Face(model_->mVertices[vindices[0]], model_->mVertices[vindices[1]], model_->mVertices[vindices[2]],
				model_->mTexcoods[tindices[0]], model_->mTexcoods[tindices[1]], model_->mTexcoods[tindices[2]])
		);
	}

	///����������Ƭ���з�ƽ���yozƽ���зֺ��������Ƭ
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


	///��ʼ��slices�������зֺ��������Ƭ��Ϊ��ͬ��slice
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


	///�ֱ��ÿһ��Slice����չ�����õ�չ�����EnrolledSlice
	vector<EnrolledSlice> enrolled_slices;
	for (auto &slice:slices) {
		enrolled_slices.push_back(slice.EnrollSliceAsCylinder());
	}
	return enrolled_slices;
}


/**
	ɾ��ѡ�к���з�ƽ��
*/
void ModelArea::DeleteCuttingPlane() {
	cut_planes_.erase(cut_planes_.begin() + selected_cut_plane_);
	cut_radii_.erase(cut_radii_.begin() + selected_cut_plane_);
	selected_cut_plane_ = -1;
}


/**
	�ϲ�û�����������������з�Ƭ�Σ���Ҫ������ÿһ�ε�����ƽ��ֵ�Լ���ϰ뾶

*/
void ModelArea::CombineRegions(vector<double>& gradients,vector<double>& average, 
	vector<double>& radii,vector<double>& cutPlanes,vector<double>& cutRadius) {
	vector<int> not_cut_index;
	for (int i = 0; i < average.size(); i++) {
		///�������һ���з�Ƭ����������������
		if (i == 0 && average[i] < texture_threshold_) {
			not_cut_index.push_back(1);
		}

		///�м���з�Ƭ����������������
		if ((i >= 1) && (i < average.size() - 1) && (average[i] < texture_threshold_)) {
			if (abs(radii[i] - radii[i - 1]) < abs(radii[i] - radii[i - 1])) {
				not_cut_index.push_back(i);
			}
			else {
				not_cut_index.push_back(i + 1);
			}
		}
		
		///��������з�Ƭ����������������
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
	�л����Ƿ���Ҫ�ϲ����򡱵�ѡ��
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