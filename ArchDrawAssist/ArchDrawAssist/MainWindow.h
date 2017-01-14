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
	应用程序的主窗口，用于显示菜单栏、工具栏、以及模型及切分和展开效果
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

	///初始化程序的相关控件
	void InitActions();	//初始化操作
	void InitMenus();	//初始化菜单
	void InitWidgets();	//初始化控件
	void InitLayout();	//初始化布局
	void InitConnect();	//连接控件及其操作

public slots:
	void OpenFile();	//打开文件
	void SaveFiles();	//保存所有文件
	void UnfoldModel();	//展开模型

private:
	QAction* import_action_;	//读取模型文件操作
	QAction* save_action_;	//保存所有文件操作，实际没有实现其功能
	QAction* save_texture_action_;	//保存为纹理截图操作
	QAction* start_unfold_action_;	//展开模型操作

	QMenu* file_menu_;	//文件菜单
	QMenu* options_menu_;	//选项菜单
	QMenu* help_menu_;	//帮助菜单
	QToolBar* main_toolbar_;	//工具栏

	QWidget* central_widget_;	//窗口主控件
	QHBoxLayout* central_layout_;	//窗口主布局

	ModelArea* model_area_;	//显示三维模型的自定义控件
	TextureArea* texture_area_;	//显示展开后的纹理的自定义控件

	GLModel* model_;	//三维模型
};
