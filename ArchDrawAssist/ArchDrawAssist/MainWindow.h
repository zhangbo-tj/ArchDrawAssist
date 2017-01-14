#pragma once

#include "ModelArea.h"
#include "TextureArea.h"
#include "GLModel.h"

#include <QtWidgets/QMainWindow>

class QAction;
class QStatusBar;
class QToolBar;
class QMenu;
class QSplitter;
class QWidget;
class QHBoxLayout;


/**
	Ӧ�ó���������ڣ�������ʾ�˵��������������Լ�ģ�ͼ��зֺ�չ��Ч��
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

	///��ʼ���������ؿؼ�
	void InitActions();	//��ʼ������
	void InitMenus();	//��ʼ���˵�
	void InitWidgets();	//��ʼ���ؼ�
	void InitLayout();	//��ʼ������
	void InitConnect();	//���ӿؼ��������

public slots:
	void OpenFile();	//���ļ�
	void SaveFiles();	//���������ļ�
	void UnfoldModel();	//չ��ģ��

private:
	QAction* import_action_;	//��ȡģ���ļ�����
	QAction* save_action_;	//���������ļ�������ʵ��û��ʵ���书��
	QAction* save_texture_action_;	//����Ϊ�����ͼ����
	QAction* start_unfold_action_;	//չ��ģ�Ͳ���

	QMenu* file_menu_;	//�ļ��˵�
	QMenu* options_menu_;	//ѡ��˵�
	QMenu* help_menu_;	//�����˵�
	QToolBar* main_toolbar_;	//������

	QWidget* central_widget_;	//�������ؼ�
	QHBoxLayout* central_layout_;	//����������

	ModelArea* model_area_;	//��ʾ��άģ�͵��Զ���ؼ�
	TextureArea* texture_area_;	//��ʾչ�����������Զ���ؼ�

	GLModel* model_;	//��άģ��
};
