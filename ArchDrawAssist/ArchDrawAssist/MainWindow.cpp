#include "MainWindow.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QWidget> 
#include <QHBoxLayout>
#include <QFileDialog>

/**
	���캯��
*/
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(QString::fromLocal8Bit("���Ż�ͼ����ϵͳ"));

	///��ʼ�����������еĿؼ�
	InitActions();	//��ʼ������
	InitMenus();	//��ʼ���˵�
	InitWidgets();	//��ʼ���ؼ�
	InitLayout();	//��ʼ������
	InitConnect();	//��ʼ���ؼ����źŲ۵�����

	setContentsMargins(5, 5, 10, 5);
	statusBar()->showMessage(tr("Ready"));


	if (!isMaximized()) {	//�������
		showMaximized();
	}
}


/**
	��ʼ�����еĲ���
*/
void MainWindow::InitActions() {
	///���ļ�
	import_action_ = new QAction(QString::fromLocal8Bit("��"));
	QIcon icon;
	icon.addFile(QString::fromUtf8("Icons/import_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
	import_action_->setIcon(icon);


	///�����ļ�
	save_action_ = new QAction(QString::fromLocal8Bit("����"));
	QIcon icon1;
	icon1.addFile(QString::fromUtf8("Icons/save_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
	save_action_->setIcon(icon1);

	///���������ͼ
	save_texture_action_ = new QAction(QString::fromLocal8Bit("��������"));
	QIcon icon2;
	icon2.addFile(QString::fromUtf8("Icons/save_texture_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
	save_texture_action_->setIcon(icon2);


	///�Ƿ�ϲ�����
	combine_region_action_ = new QAction(QString::fromLocal8Bit("�ϲ�����"));
	QIcon icon3;
	icon3.addFile(QString::fromUtf8("Icons/combine_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
	combine_region_action_->setIcon(icon3);
	combine_region_action_->setCheckable(true);


	///�����з�ƽ���λ��
	cal_cutplane_action_ = new QAction(QString::fromLocal8Bit("�з�λ��"));
	QIcon icon4;
	icon4.addFile(QString::fromUtf8("Icons/cut_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
	cal_cutplane_action_->setIcon(icon4);


	///չ����άģ��
	start_unfold_action_ = new QAction(QString::fromLocal8Bit("չ��ģ��"));
	QIcon icon5;
	icon5.addFile(QString::fromUtf8("Icons/start_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
	start_unfold_action_->setIcon(icon5);
}

/**
	��ʼ���˵�
*/
void MainWindow::InitMenus() {
	///��ʼ���ļ��˵�
	file_menu_ = menuBar()->addMenu(QString::fromLocal8Bit("�ļ�"));
	file_menu_->addAction(import_action_);
	file_menu_->addAction(save_action_);
	file_menu_->addAction(combine_region_action_);
	file_menu_->addAction(cal_cutplane_action_);
	file_menu_->addAction(save_texture_action_);

	///��ʼ��ѡ��˵�
	options_menu_ = menuBar()->addMenu(QString::fromLocal8Bit("ѡ��"));
	options_menu_->addAction(start_unfold_action_);

	///��ʼ��������
	main_toolbar_ = addToolBar(QString::fromLocal8Bit("������"));
	main_toolbar_->setBaseSize(QSize(11, 11));
	main_toolbar_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	main_toolbar_->addAction(import_action_);
	main_toolbar_->addAction(save_action_);
	main_toolbar_->addAction(save_texture_action_);
	main_toolbar_->addSeparator();
	main_toolbar_->addAction(combine_region_action_);
	main_toolbar_->addAction(cal_cutplane_action_);
	main_toolbar_->addAction(start_unfold_action_);
}


/**
	��ʼ���Զ���ؼ�
*/
void MainWindow::InitWidgets() {
	central_widget_ = new QWidget();
	model_area_ = new ModelArea();
	texture_area_ = new TextureArea(this,model_area_);
}


/**
	��ʼ�����ڲ���
*/
void MainWindow::InitLayout() {
	central_layout_ = new QHBoxLayout();
	central_layout_->addWidget(model_area_);
	central_layout_->addWidget(texture_area_);
	central_widget_->setLayout(central_layout_);
	setCentralWidget(central_widget_);
}

/**
	��ʼ�����������źŲ�
*/
void MainWindow::InitConnect() {
	connect(import_action_, SIGNAL(triggered()), this, SLOT(OpenFile()));
	connect(save_action_, SIGNAL(triggered()), this, SLOT(SaveFiles()));
	connect(start_unfold_action_, SIGNAL(triggered()), this, SLOT(UnfoldModel()));
	connect(combine_region_action_, &QAction::changed, model_area_, &ModelArea::ResetCombine);
	connect(cal_cutplane_action_, SIGNAL(triggered()), model_area_, SLOT(ComputeBestCuts()));
	connect(save_texture_action_, SIGNAL(triggered()), texture_area_, SLOT(SaveTextureFile()));
}

/**
	���ļ�����
*/
void MainWindow::OpenFile() {
	QString file = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("���ļ�"), "", "*.obj");
	model_ = GLMReadOBJ(file.toLatin1().data());
	model_->CalFacetNormals();
	model_->Unitlize();
	model_->CalAxisNormal();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	combine_region_action_->setChecked(false);
	model_area_->LoadModel(model_);
	
	texture_area_->Reset();
}



/**
	���������ļ�����
*/
void MainWindow::SaveFiles() {
}


/**
	չ��ģ�Ͳ���
*/
void MainWindow::UnfoldModel() {
	texture_area_->LoadEnrolledSlices(model_area_->CutInConicalMode());		//ʹ��Բ̨չ��
	//texture_area_->LoadEnrolledSlices(model_area_->CutInCylinderMode());	//ʹ��Բ��չ��
}

