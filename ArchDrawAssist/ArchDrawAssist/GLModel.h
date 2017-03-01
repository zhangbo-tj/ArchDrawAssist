#pragma once
#include "Vertex.h"
#include "Normal.h"
#include "Texture.h"
#include "Material.h"
#include "Triangle.h"
#include "Group.h"
#include "GeneralUse.h"
#include <gl/glut.h>
#include <vector>

using namespace std;
/* defines */
#define GLM_NONE     (0)    /* render with only vertices */
#define GLM_FLAT     (1 << 0)    /* render with facet normals */       // 0000 0001将1左移0位,即0000 0001
#define GLM_SMOOTH   (1 << 1)    /* render with vertex normals */      // 0000 0001将1左移1位，即0000 0010
#define GLM_TEXTURE  (1 << 2)    /* render with texture coords */      //0000 0001将1左移2位，即0000 0100
#define GLM_COLOR    (1 << 3)    /* render with colors */              //0000 0001将1左移3位，即0000 1000
#define GLM_MATERIAL (1 << 4)    /* render with materials */           //0000 0001将1左移4位，即0001 0000




/**
	三维模型的类，主要用于三维模型的读取和相关计算
	该类主要是对GLM中的GLMmodel进行改写后的内容
*/	
class GLModel {
public:
	char* mPathname;		//三维模型的路径
	char* mMtllibname;	//Material的名称

	vector<Vertex> mVertices;		//模型中顶点的数组
	vector<Normal> mNormals;		//模型中顶点法向量数组
	vector<Texture> mTexcoods;		//模型中顶点纹理坐标值的数组
	vector<Material> mMaterials;	//模型material的数组
	vector<Triangle> mTriangles;	//模型中三角面片的数组
	vector<Normal> mFacetNormals;	//模型中三角面片的数组
	vector<Group> mGroups;			//模型中Group数组
	vector<Normal> mPrincipleDir1, mPrincipleDir2;  //curvature 主方向

	vector<vector<int>> v_1ringVer;	//顶点的1 ring 邻接顶点
	vector<vector<int>> v_1ringTri;	//顶点的1 ring邻接三角形
	vector<vector<int>> t_1ringTri;	//三角形的1 ring 邻接三角形

	float mAvePt[3];		//三维模型的坐标平均值
	float mMaxPt[3], mMinPt[3];	//三维模型的坐标的上下边界
	Normal mAxisNormal;		//三维模型的对称轴
	GLuint* mTextureArray;	//三维模型的纹理数组

public:
	//清除纹理
	void ClearTextures();

	//构造函数
	GLModel();

	//清除当前数据
	void ClearData();

	//重载赋值操作符
	GLModel& operator=(const GLModel & model);

	//判断bounding box是否为unit box
	bool IsUnitBox();

	//计算模型坐标平均值(中间值)和最大值、最小值
	void CalAverage();

	//计算主方向
	void CalPrincipal();

	//读取主方向文件
	bool Read_pdirFile(string strfile);

	//单位化模型
	void Unitlize();

	//计算模型面的法向量
	void CalFacetNormals();

	//生成索引表
	void GenIncidentTable();

	//计算对称轴方向向量
	void CalAxisNormal();
};

//读取obj文件获取三维模型
GLModel* GLMReadOBJ(const char* filename);

//将三维模型写入obj文件
void GLMWriteOBJ(GLModel* model, char* filename, GLuint mode);

//查找相应的材质
int GLMFindMaterial(GLModel* model, char* name);

//读取MTL文件
void GLMReadMTL(GLModel* model, char* buf);

//写入MTL文件
void GLMWriteMTL(GLModel model, char* modelpath, char* mtllibname);

//读取BMP纹理文件
int GLMReadBMP(GLuint textureArray[], char* strFileName, int textureID);

//获取三维模型所在的路径
char* GLMDirName(char* path);

//绘制三维模型
void GLMDraw(GLModel* model, GLuint mode);