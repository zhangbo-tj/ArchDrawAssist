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
#define GLM_FLAT     (1 << 0)    /* render with facet normals */       // 0000 0001��1����0λ,��0000 0001
#define GLM_SMOOTH   (1 << 1)    /* render with vertex normals */      // 0000 0001��1����1λ����0000 0010
#define GLM_TEXTURE  (1 << 2)    /* render with texture coords */      //0000 0001��1����2λ����0000 0100
#define GLM_COLOR    (1 << 3)    /* render with colors */              //0000 0001��1����3λ����0000 1000
#define GLM_MATERIAL (1 << 4)    /* render with materials */           //0000 0001��1����4λ����0001 0000




/**
	��άģ�͵��࣬��Ҫ������άģ�͵Ķ�ȡ����ؼ���
	������Ҫ�Ƕ�GLM�е�GLMmodel���и�д�������
*/	
class GLModel {
public:
	char* mPathname;		//��άģ�͵�·��
	char* mMtllibname;	//Material������

	vector<Vertex> mVertices;		//ģ���ж��������
	vector<Normal> mNormals;		//ģ���ж��㷨��������
	vector<Texture> mTexcoods;		//ģ���ж�����������ֵ������
	vector<Material> mMaterials;	//ģ��material������
	vector<Triangle> mTriangles;	//ģ����������Ƭ������
	vector<Normal> mFacetNormals;	//ģ����������Ƭ������
	vector<Group> mGroups;			//ģ����Group����
	vector<Normal> mPrincipleDir1, mPrincipleDir2;  //curvature ������

	vector<vector<int>> v_1ringVer;	//�����1 ring �ڽӶ���
	vector<vector<int>> v_1ringTri;	//�����1 ring�ڽ�������
	vector<vector<int>> t_1ringTri;	//�����ε�1 ring �ڽ�������

	float mAvePt[3];		//��άģ�͵�����ƽ��ֵ
	float mMaxPt[3], mMinPt[3];	//��άģ�͵���������±߽�
	Normal mAxisNormal;		//��άģ�͵ĶԳ���
	GLuint* mTextureArray;	//��άģ�͵���������

public:
	//�������
	void ClearTextures();

	//���캯��
	GLModel();

	//�����ǰ����
	void ClearData();

	//���ظ�ֵ������
	GLModel& operator=(const GLModel & model);

	//�ж�bounding box�Ƿ�Ϊunit box
	bool IsUnitBox();

	//����ģ������ƽ��ֵ(�м�ֵ)�����ֵ����Сֵ
	void CalAverage();

	//����������
	void CalPrincipal();

	//��ȡ�������ļ�
	bool Read_pdirFile(string strfile);

	//��λ��ģ��
	void Unitlize();

	//����ģ����ķ�����
	void CalFacetNormals();

	//����������
	void GenIncidentTable();

	//����Գ��᷽������
	void CalAxisNormal();
};

//��ȡobj�ļ���ȡ��άģ��
GLModel* GLMReadOBJ(const char* filename);

//����άģ��д��obj�ļ�
void GLMWriteOBJ(GLModel* model, char* filename, GLuint mode);

//������Ӧ�Ĳ���
int GLMFindMaterial(GLModel* model, char* name);

//��ȡMTL�ļ�
void GLMReadMTL(GLModel* model, char* buf);

//д��MTL�ļ�
void GLMWriteMTL(GLModel model, char* modelpath, char* mtllibname);

//��ȡBMP�����ļ�
int GLMReadBMP(GLuint textureArray[], char* strFileName, int textureID);

//��ȡ��άģ�����ڵ�·��
char* GLMDirName(char* path);

//������άģ��
void GLMDraw(GLModel* model, GLuint mode);