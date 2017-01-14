#include "GLModel.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/GLAUX.H>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <direct.h>
#include <assert.h>
#include <io.h>

#include <QDebug>

/* enums */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


enum { X, Y, Z, W };

//////////////////////////////////////////////////////////////////////////
//@intro	构造器
//////////////////////////////////////////////////////////////////////////
GLModel::GLModel() {
	mAvePt[0] = mAvePt[1] = mAvePt[2] = 0.0;
}

//////////////////////////////////////////////////////////////////////////
//@intro	清除三维模型数据
//////////////////////////////////////////////////////////////////////////
void GLModel::ClearData() {
	mPathname = NULL;
	mMtllibname = NULL;

	mVertices.clear();
	mNormals.clear();
	mTexcoods.clear();
	mMaterials.clear();
	mTriangles.clear();
	mFacetNormals.clear();
	mPrincipleDir1.clear();
	mPrincipleDir2.clear();

	mAvePt[0] = mAvePt[1] = mAvePt[2] = 0.0;

	while (v_1ringTri.size()) {
		vector<int> teminci;
		(v_1ringTri.end() - 1)->swap(teminci);
		v_1ringTri.pop_back();
	}

	while (t_1ringTri.size()) {
		vector<int> temici;
		(t_1ringTri.end() - 1)->swap(temici);
		t_1ringTri.pop_back();
	}
}

//////////////////////////////////////////////////////////////////////////
//@intro	重载运算符=
//@param	model：三维模型
//////////////////////////////////////////////////////////////////////////
GLModel& GLModel::operator=(const GLModel & model) {
	mPathname = model.mPathname;
	mMtllibname = model.mMtllibname;

	mVertices = model.mVertices;
	mNormals = model.mNormals;
	mTexcoods = model.mTexcoods;
	mMaterials = model.mMaterials;
	mTriangles = model.mTriangles;
	mFacetNormals = model.mFacetNormals;
	mPrincipleDir1 = model.mPrincipleDir1;
	mPrincipleDir2 = model.mPrincipleDir2;
	v_1ringVer = model.v_1ringVer;
	v_1ringTri = model.v_1ringTri;
	t_1ringTri = model.t_1ringTri;
	return *this;
	//return GLModel(pathname,mtllibname,vertices,normals,texcoods,materials,triangles,facetnorms,groups,principleDir1,principleDir2,v_1ringVer,v_1ringTri,t_1ringTri);
}


//////////////////////////////////////////////////////////////////////////
//@intro	判断bounding box是否为unit box
//////////////////////////////////////////////////////////////////////////
bool GLModel::IsUnitBox() {
	CalAverage();

	float width[3], bbox_center[3];

	for (int i = 0; i< 3; i++) {
		width[i] = mMaxPt[i] - mMinPt[i];
		bbox_center[i] = (mMaxPt[i] + mMinPt[i]) / 2;
	}

	int maxW = MaxIndex(width, 3);
	if (fabs(mMaxPt[maxW] - 0.5) < 0.0001 &&fabs(mMinPt[maxW] + 0.5) <0.0001)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
//@intro	计算三维模型样本点的minPt,maxPt和avePt
//////////////////////////////////////////////////////////////////////////
void GLModel::CalAverage() {
	int verNum = mVertices.size();
	float sumx, sumy, sumz;
	if (verNum <1)
		return;
	sumx = mVertices[0].X();	sumy = mVertices[0].Y();	sumz = mVertices[0].Z();
	mMaxPt[0] = mVertices[0].X();	mMaxPt[1] = mVertices[0].Y();	mMaxPt[2] = mVertices[0].Z();
	mMinPt[0] = mVertices[0].X();	mMinPt[1] = mVertices[0].Y();	mMinPt[2] = mVertices[0].Z();

	for (int i = 1; i< verNum; i++) {
		sumx += mVertices[i].X();
		sumy += mVertices[i].Y();
		sumz += mVertices[i].Z();
		mMaxPt[0] = mVertices[i].X() > mMaxPt[0] ? mVertices[i].X() : mMaxPt[0];
		mMaxPt[1] = mVertices[i].Y() > mMaxPt[1] ? mVertices[i].Y() : mMaxPt[1];
		mMaxPt[2] = mVertices[i].Z() > mMaxPt[2] ? mVertices[i].Z() : mMaxPt[2];

		mMinPt[0] = mVertices[i].X() < mMinPt[0] ? mVertices[i].X() : mMinPt[0];
		mMinPt[1] = mVertices[i].Y() < mMinPt[1] ? mVertices[i].Y() : mMinPt[1];
		mMinPt[2] = mVertices[i].Z() < mMinPt[2] ? mVertices[i].Z() : mMinPt[2];
	}

	mAvePt[0] = (mMinPt[0] + mMaxPt[0]) / 2;
	mAvePt[1] = (mMinPt[1] + mMaxPt[1]) / 2;
	mAvePt[2] = (mMinPt[2] + mMaxPt[2]) / 2;
}

//////////////////////////////////////////////////////////////////////////
//@intro	计算三维模型的min和max之间的距离
//////////////////////////////////////////////////////////////////////////
float calLength(float max, float min) {
	float length;
	if (max >= 0 && min >= 0) {
		length = max - min;
	}
	else if (max >= 0 && min <= 0) {
		length = max - min;
	}
	else if (max <= 0 && min <= 0) {
		length = fabs(max) - fabs(min);
	}
	return length;
}


//////////////////////////////////////////////////////////////////////////
//@intro	对三维模型进行单位化
//////////////////////////////////////////////////////////////////////////
void GLModel::Unitlize() {
	float scale;
	float w, h, d;
	CalAverage();
	//w = calLength(mMaxPt[0], mMinPt[0]);
	h = calLength(mMaxPt[1], mMinPt[1]);
	//d = calLength(mMaxPt[2], mMinPt[2]);

	//scale = 2.0 / (getmax(w, h, d));
	scale = 2.0/h;
	for (int i = 0; i < mVertices.size(); i++) {
		float vx = mVertices[i].X();
		float vy = mVertices[i].Y();
		float vz = mVertices[i].Z();
		mVertices[i].SetX((vx - mAvePt[0]) * scale);
		mVertices[i].SetY((vy - mAvePt[1]) * scale);
		mVertices[i].SetZ((vz - mAvePt[2]) * scale);
	}
	CalAverage();
}


//////////////////////////////////////////////////////////////////////////
//@intro	计算三维模型上所有三角面片的法向量
//////////////////////////////////////////////////////////////////////////
void GLModel::CalFacetNormals() {
	for (int i = 0; i < mTriangles.size(); i++) {
		int* vindices = mTriangles[i].Vindices();
		Vertex v1 = mVertices.at(vindices[0]);
		Vertex v2 = mVertices.at(vindices[1]);
		Vertex v3 = mVertices.at(vindices[2]);

		mFacetNormals.push_back(Normal(v1, v2, v3));
		mFacetNormals[i].Normalize();
		mTriangles[i].SetFindex(i);
	}
	//calPrinciple();
}


//////////////////////////////////////////////////////////////////////////
//@intro	将wide char转换为string
//////////////////////////////////////////////////////////////////////////
std::string ws2s(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}

//////////////////////////////////////////////////////////////////////////
//@intro	计算三维模型的主方向
//////////////////////////////////////////////////////////////////////////
void GLModel::CalPrincipal() {
	cout << "Now compute the principal" << endl;
	char buf[MAX_PATH];
	getcwd(buf, MAX_PATH);
	string path = buf;
	path = path + "\\" + mPathname;
	string objFile = path;
	path.resize(path.size() - 4);
	string output = path + ".pdir";

	if (_access(output.data(), 06) != 0) {
		char exepath[MAX_PATH];
		string strdir, tmpdir;
		memset(exepath, 0, MAX_PATH);
		//GetModuleFileName(NULL,exepath,MAX_PATH);
		getcwd(exepath, MAX_PATH);
		tmpdir = exepath;
		strdir += "curvature.exe";

		string strCommand = strdir + " " + objFile + " " + output;
		//string command1 = output;
		std::wstring stemp = std::wstring(strCommand.begin(), strCommand.end());
		LPWSTR sw = (LPWSTR)stemp.c_str();

		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		BOOL bRet = CreateProcess(NULL, sw, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (bRet) {
			if (!WaitForSingleObject(pi.hProcess, INFINITE)) {
				Read_pdirFile(output);
			}
		}
	}
	else {
		Read_pdirFile(output);
	}
	GenIncidentTable();
}

//////////////////////////////////////////////////////////////////////////
//@intro	读取主方向文件
//////////////////////////////////////////////////////////////////////////
bool GLModel::Read_pdirFile(string strfile) {
	FILE* file;
	char buf[128];
	float f1, f2, f3, f4, f5, f6;
	int num, d1;
	file = fopen(strfile.c_str(), "r");

	if (!file) {
		cout << "Can't open the principal direction file" << endl;
		system("pause");
	}

	if (fscanf(file, "%d", &d1) != EOF) {
		num = d1;
		cout << num << endl;
		if (num != mVertices.size()) {
			cout << "The number is not congruent with the current mesh" << endl;
			system("pause");
		}
	}
	while (fscanf(file, "%f %f %f %f %f %f", &f1, &f2, &f3, &f4, &f5, &f6) == 6)
	{
		mPrincipleDir1.push_back(Normal(f1, f2, f3));
		mPrincipleDir2.push_back(Normal(f4, f5, f6));
	}
	cout << "The file readed completely" << endl;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//@intro	生成索引表
//////////////////////////////////////////////////////////////////////////
void GLModel::GenIncidentTable() {
	vector<int> teminci;
	int numVer = mVertices.size();
	int numTri = mTriangles.size();

	vector<Triangle>::iterator f_iter = mTriangles.begin();
	//v_1ringTri.assign(numVer, numTri);//顶点的1 ring邻接face
	v_1ringTri.assign(numVer, vector<int>(numTri));
	f_iter = mTriangles.begin();

	for (int i = 0; f_iter != mTriangles.end() && i < numTri; i++, f_iter++) {
		int pa, pb, pc;
		int* vindices = f_iter->Vindices();
		pa = vindices[0];
		pb = vindices[1];
		pc = vindices[2];
		v_1ringTri.at(pa).push_back(i);
		v_1ringTri.at(pb).push_back(i);
		v_1ringTri.at(pc).push_back(i);
	}

	t_1ringTri.assign(numTri, teminci);
	vector<vector<int>>::iterator iter_incitable = t_1ringTri.begin();
	f_iter = mTriangles.begin();
	CMyBinaryTree mybtree(0);

	for (int i = 0; i < numTri; i++, f_iter++, iter_incitable++) {
		int* vindices = f_iter->Vindices();
		for (int j = 0; j < 3; j++) {
			int pp = vindices[j];
			vector<int> &p_face = v_1ringTri[pp];
			vector<int>::iterator iter_index = p_face.begin();
			if (j == 0) {
				mybtree.resetroot(*iter_index);
			}
			for (int k = 0; iter_index != p_face.end(); k++, iter_index++) {
				mybtree.InsertElement(*iter_index);
			}
		}
		mybtree.TravelTree(*iter_incitable);
	}

	v_1ringVer.assign(numVer, teminci);
	vector<vector<int>>::iterator iter_inciVer = v_1ringVer.begin();
	vector<vector<int>>::iterator iter_neigFace = v_1ringTri.begin();
	for (int i = 0; i < numVer; i++, iter_inciVer++, iter_neigFace++) {
		bool flag = false;
		vector<int>::iterator iterf = iter_neigFace->begin();
		for (; iterf != iter_neigFace->end(); iterf++) {
			f_iter = mTriangles.begin() + *iterf;
			int* vindices = f_iter->Vindices();
			for (int j = 0; j < 3; j++) {
				int pv = vindices[j];
				if (pv == i)
					continue;
				if (!flag) {
					mybtree.resetroot(pv);
					flag = true;
					continue;
				}
				mybtree.InsertElement(pv);
			}
		}
		mybtree.TravelTree(*iter_inciVer);
	}

}

//////////////////////////////////////////////////////////////////////////
//@intro	删除三维模型的纹理
//////////////////////////////////////////////////////////////////////////
void GLModel::ClearTextures() {
	mTextureArray = new GLuint[100];
	for (int i = 0; i<100; i++) {
		mTextureArray[i] = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
//@intro	初始化数组值
//@param	v,n,t:需要初始化的int数组
//////////////////////////////////////////////////////////////////////////
void initVNT(int v[], int n[], int t[]) {
	v[0] = v[1] = v[2] = -1;
	n[0] = n[1] = n[2] = -1;
	t[0] = t[1] = t[2] = -1;
}

/* strdup is actually not a standard ANSI C or POSIX routine
so implement a private one.  OpenVMS does not have a strdup; Linux's
standard libc doesn't declare strdup by default (unless BSD or SVID
interfaces are requested). */
//复制一个string指向的字符串
static char* stralloc(const char *string)
{
	char *copy;
	int a = strlen(string) + 1;
	copy = (char*)malloc(strlen(string) + 1);
	if (copy == NULL)
		return NULL;
	strcpy(copy, string);
	return copy;
}


//////////////////////////////////////////////////////////////////////////
//@intro	读取Obj文件
//@param	filename:obj文件名
//////////////////////////////////////////////////////////////////////////
GLModel* GLMReadOBJ(const char* filename) {
	GLModel* model = new GLModel();
	FILE* file;
	char buf[128];
	float f1, f2, f3;
	int v = 0, n = 0, t = 0;
	int *t_vindices, *t_nindices, *t_tindices;
	int vindices[3], nindices[3], tindices[3];
	int curIndex = 0;
	int materialNum = 0;

	model->ClearTextures();

	//model = (GLModel*)malloc(sizeof(GLModel));
	cout << "The size of model: " << sizeof(model) << endl;
	model->mPathname = stralloc(filename);

	file = fopen(filename, "r");
	cout << "file name:" << filename << endl;
	if (!file) {
		fprintf(stderr, "glmReadOBJ() failed:can't open data file %s\".\n", filename);
		system("pause");
	}

	Group tempGroup = Group();
	model->mGroups.push_back(tempGroup);

	int faceCount = 0;
	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case '#':
			fgets(buf, sizeof(buf), file);
			break;
		case 'v':
			switch (buf[1]) {
			case '\0':
				fscanf(file, "%f %f %f", &f1, &f2, &f3);
				model->mVertices.push_back(Vertex(f1, f2, f3));
				break;
			case 'n':
				fscanf(file, "%f %f %f", &f1, &f2, &f3);
				model->mNormals.push_back(Normal(f1, f2, f3));
				break;
			case 't':
				fscanf(file, "%f %f", &f1, &f2);
				model->mTexcoods.push_back(Texture(f1, f2));
				break;
			}
			break;
		case 'm':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);

			//model->mtllibname = stralloc(buf);
			model->mMtllibname = stralloc(buf);

			GLMReadMTL(model, buf);
			break;
		case 'u':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);

			materialNum = GLMFindMaterial(model, stralloc(buf));
			model->mGroups.at(model->mGroups.size() - 1).SetMaterialno(materialNum);
			//model->groups.at(model->groups.size()-1).materialno = materialNum;
			break;
		case 'g':		//去掉设置group的部分
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s", buf);
			if (model->mGroups.size() == 1 && strcmp(model->mGroups[0].Name(), "") == 0) {
				//model.groups[0].setName(stralloc(buf));
				model->mGroups.pop_back();
				model->mGroups.push_back(Group(stralloc(buf), 0));
			}
			else {
				//tempGroup = Group(stralloc(buf));
				// 				tempGroup.setMaterialno(materialNum);
				// 				tempGroup.setName(stralloc(buf));
				model->mGroups.push_back(Group(stralloc(buf), materialNum));
			}
			break;


		case 'f':
			v = n = t = 0;

			initVNT(vindices, nindices, tindices);

			fscanf(file, "%s", buf);
			/* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
			if (strstr(buf, "//")) {

				// v//n
				sscanf(buf, "%d//%d", &v, &n);
				vindices[0] = v - 1;
				nindices[0] = n - 1;
				fscanf(file, "%d//%d", &v, &n);
				vindices[1] = v - 1;
				nindices[1] = n - 1;
				fscanf(file, "%d//%d", &v, &n);
				vindices[2] = v - 1;
				nindices[2] = n - 1;
				model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
				faceCount++;
				model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				//model->groups.at(model->groups.size()-1).triangles.push_back(model->triangles.size()-1);
				while (fscanf(file, "%d//%d", &v, &n)>0) {

					initVNT(vindices, nindices, tindices);
					curIndex = model->mTriangles.size() - 1;
					t_vindices = model->mTriangles[curIndex].Vindices();
					t_nindices = model->mTriangles[curIndex].Nindices();

					vindices[0] = t_vindices[0]; vindices[1] = t_vindices[2]; vindices[2] = v - 1;
					nindices[0] = t_nindices[0]; nindices[1] = t_nindices[2]; nindices[2] = n - 1;

					model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
					faceCount++;
					model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				}

			}
			else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
				/* v/t/n */
				initVNT(vindices, nindices, tindices);

				vindices[0] = v - 1; nindices[0] = n - 1; tindices[0] = t - 1;
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				vindices[1] = v - 1; nindices[1] = n - 1; tindices[1] = t - 1;
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				vindices[2] = v - 1; nindices[2] = n - 1; tindices[2] = t - 1;

				model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
				faceCount++;
				model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				//model.groups.at(model.groups.size()-1)triangles.push_back(model.triangles.size()-1);
				while (fscanf(file, "%d/%d/%d", &v, &t, &n)>0) {
					initVNT(vindices, nindices, tindices);

					curIndex = model->mTriangles.size() - 1;
					t_vindices = model->mTriangles[curIndex].Vindices();
					t_nindices = model->mTriangles[curIndex].Nindices();
					t_tindices = model->mTriangles[curIndex].Tindices();

					vindices[0] = t_vindices[0]; vindices[1] = t_vindices[2]; vindices[2] = v - 1;
					nindices[0] = t_nindices[0]; nindices[1] = t_nindices[2]; nindices[2] = n - 1;
					tindices[0] = t_tindices[0]; tindices[1] = t_tindices[2]; tindices[2] = t - 1;

					model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
					faceCount++;
					model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				}
			}
			else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
				initVNT(vindices, nindices, tindices);
				vindices[0] = v - 1; tindices[0] = t - 1;

				fscanf(file, "%d/%d", &v, &t);
				vindices[1] = v - 1; tindices[1] = t - 1;
				fscanf(file, "%d/%d", &v, &t);
				vindices[2] = v - 1; tindices[2] = t - 1;
				model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
				faceCount++;
				model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				while (fscanf(file, "%d/%d", &v, &t)>0) {
					initVNT(vindices, nindices, tindices);

					curIndex = model->mTriangles.size() - 1;
					t_vindices = model->mTriangles[curIndex].Vindices();
					t_tindices = model->mTriangles[curIndex].Tindices();

					vindices[0] = t_vindices[0]; vindices[1] = t_vindices[2]; vindices[2] = v - 1;
					tindices[0] = t_tindices[0]; tindices[1] = t_tindices[2]; tindices[2] = t - 1;

					model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
					faceCount++;
					model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				}
			}
			else {
				initVNT(vindices, nindices, tindices);
				sscanf(buf, "%d", &v);
				vindices[0] = v - 1;
				fscanf(file, "%d", &v);
				vindices[1] = v - 1;
				fscanf(file, "%d", &v);
				vindices[2] = v - 1;
				model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
				faceCount++;
				model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				while (fscanf(file, "%d", &v)>0) {
					initVNT(vindices, nindices, tindices);

					curIndex = model->mTriangles.size() - 1;
					t_vindices = model->mTriangles[curIndex].Vindices();
					vindices[0] = t_vindices[0]; vindices[1] = t_vindices[2]; vindices[2] = v - 1;

					model->mTriangles.push_back(Triangle(vindices, nindices, tindices, faceCount));
					faceCount++;
					model->mGroups.at(model->mGroups.size() - 1).AddTriangle(model->mTriangles.size() - 1);
				}
			}
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	printf("Memory: %d bytes\n", model->mVertices.size() * sizeof(Vertex) + model->mNormals.size() * sizeof(Normal) +
		model->mTexcoods.size() * sizeof(Texture) + model->mTriangles.size() * sizeof(Triangle));
	fclose(file);
	return model;
}


//////////////////////////////////////////////////////////////////////////
//@intro	查找是否存在material
//@param	model:三维模型
//@param	name:material名
//@return	material的索引
//////////////////////////////////////////////////////////////////////////
int GLMFindMaterial(GLModel* model, char* name) {
	int i;
	for (i = 0; i < model->mMaterials.size(); i++) {
		if (!strcmp(model->mMaterials[i].Name(), name))
			return i;
	}
	printf("_glmFindMaterial(),can't find material %s\".\n", name);
	i = 0;

	return i;
}

//////////////////////////////////////////////////////////////////////////
//@intro	读取MTL文件
//@param	model:三维模型
//@param	name:MTL文件名
//////////////////////////////////////////////////////////////////////////
void GLMReadMTL(GLModel* model, char* name) {
	FILE* file;
	char* dir;
	char* filename;
	char buf[128];
	int numMaterials, i;
	char* name1;
	char* map_file;
	float shiness;
	float diffuse[4], ambient[4], specular[4], emmissive[4];

	dir = GLMDirName(model->mPathname);
	filename = (char*)malloc(sizeof(char)*(strlen(dir) + strlen(name) + 1));
	strcpy(filename, dir);  //将dir复制到filenam开始位置
	strcat(filename, name);	//连接两个字符串
	cout << "mtl file name: " << filename << endl;
	free(dir);
	file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "_glmReadMTL() failed: can't open material file \"%s\".\n", filename);
		//exit(1);
	}
	free(filename);

	/*model->materials[0].name = stralloc("default");*/

	name1 = stralloc("default");
	shiness = 0.0;
	diffuse[0] = 0.8; diffuse[1] = 0.8; diffuse[2] = 0.8; diffuse[3] = 1.0;
	ambient[0] = 0.2; ambient[1] = 0.2; ambient[2] = 0.2; ambient[3] = 1.0;
	specular[0] = 0.0; specular[1] = 0.0; specular[2] = 0.0; specular[3] = 1.0;
	emmissive[0] = 0.0; emmissive[1] = 0.0; emmissive[2] = 0.0; emmissive[3] = 0.0;
	map_file = NULL;

	Material ma = Material(name1, diffuse, ambient, specular, emmissive, shiness, map_file);
	model->mMaterials.push_back(ma);

	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case '#':
			fgets(buf, sizeof(buf), file);
			break;
		case 'n':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);
			model->mMaterials.push_back(Material(stralloc(buf)));
			break;
		case 'N':
			fscanf(file, "%f", &shiness);
			shiness /= 1000.0;
			shiness *= 128.0;
			break;
		case  'm':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);

			map_file = stralloc(buf);
			std::cout << "map_file name: " << map_file << endl;
			model->mMaterials.at(model->mMaterials.size() - 1).SetMapFile(map_file);
			//_glmReadBMP(textureArray,stralloc(buf),model->materials.size()-1);
			break;
		case 'K':
			switch (buf[1]) {
			case 'd':
				fscanf(file, "%f %f %f", &diffuse[0], &diffuse[1], &diffuse[2]);
				model->mMaterials.at(model->mMaterials.size() - 1).SetDiffuse(diffuse);

				break;
			case 'f':
				fscanf(file, "%f %f %f", &specular[0], &specular[1], &specular[2]);
				model->mMaterials.at(model->mMaterials.size() - 1).SetSpecular(specular);
				break;
			case 's':
				fscanf(file, "%f %f %f", &specular[0], &specular[1], &specular[2]);
				model->mMaterials.at(model->mMaterials.size() - 1).SetSpecular(specular);
				break;
			case 'a':
				fscanf(file, "%f %f %f", &ambient[0], &ambient[1], &ambient[2]);
				model->mMaterials.at(model->mMaterials.size() - 1).SetAmbient(ambient);
				break;
			default:
				fgets(buf, sizeof(buf), file);
				break;
			}
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//@intro	获取三维模型的路径
//@param	path:路径名
//////////////////////////////////////////////////////////////////////////
char* GLMDirName(char* path) {
	char* dir;
	char* s;
	dir = stralloc(path);

	s = strrchr(dir, '/');
	if (s)
		s[1] = '\0';
	else
		dir[0] = '\0';
	return dir;
}

//////////////////////////////////////////////////////////////////////////
//读取BMP图片生成纹理，并绑定到
//////////////////////////////////////////////////////////////////////////
int GLMReadBMP(GLuint textureArray[], char* strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	cout << "The image file name is :" << strFileName << endl;


	if (!strFileName) {
		printf("Cannot find the BMP texture!!\n");     // 如果无此文件，则直接返回
		return 0;
	}
	//pBitmap = auxDIBImageLoadA(strFileName);				// 装入位图，并保存数据
	pBitmap = auxDIBImageLoadA(strFileName);
	if (pBitmap == NULL) {								// 如果装入位图失败，则退出
		printf("Cannot open the BMP texture!!\n");
		return 0;
	}


	glGenTextures(1, &textureArray[textureID]);//根据纹理参数返回n个纹理索引

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// 设置像素对齐格式

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]); //绑定纹理

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);// 生成纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	printf("texture %d :: %s is loaded\n", textureArray[textureID], strFileName);
	if (pBitmap)										// 释放位图占用的资源
	{
		if (pBitmap->data)
		{
			free(pBitmap->data);
		}
		free(pBitmap);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//@intro	绘制三维模型
//@param	model:三维模型
//@param	mode:绘制三维模型的模式
//////////////////////////////////////////////////////////////////////////
void GLMDraw(GLModel* model, GLuint mode) {
	int i, j;
	Group group;
	int *nindices, *vindices, *tindices;
	assert(model->mVertices.size());

	/* do a bit of warning */
	if (mode & GLM_FLAT && !model->mFacetNormals.size()) {
		printf("glmDraw() warning: flat render mode requested "
			"with no facet normals defined.\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_SMOOTH && !model->mNormals.size()) {
		printf("glmDraw() warning: smooth render mode requested "
			"with no normals defined.\n");
		mode &= ~GLM_SMOOTH;
	}
	if (mode & GLM_TEXTURE && !model->mTexcoods.size()) {
		printf("glmDraw() warning: texture render mode requested "
			"with no texture coordinates defined.\n");
		mode &= ~GLM_TEXTURE;
	}
	if (mode & GLM_FLAT && mode & GLM_SMOOTH) {
		printf("glmDraw() warning: flat render mode requested "
			"and smooth render mode requested (using smooth).\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_COLOR && !model->mMaterials.size()) {
		printf("glmDraw() warning: color render mode requested "
			"with no materials defined.\n");
		mode &= ~GLM_COLOR;
	}
	if (mode & GLM_MATERIAL && !model->mMaterials.size()) {
		printf("glmDraw() warning: material render mode requested "
			"with no materials defined.\n");
		mode &= ~GLM_MATERIAL;
	}
	if (mode & GLM_COLOR && mode & GLM_MATERIAL) {
		printf("glmDraw() warning: color and material render mode requested "
			"using only material mode\n");
		mode &= ~GLM_COLOR;
	}
	if (mode & GLM_COLOR)
		glEnable(GL_COLOR_MATERIAL);
	if (mode & GLM_MATERIAL)
		glDisable(GL_COLOR_MATERIAL);

	// glPushMatrix();
	//glTranslatef(model->position[0], model->position[1], model->position[2]);//改变位置
	for (unsigned int i = 0; i < model->mGroups.size(); i++) {
		group = model->mGroups.at(i);
		Material material = model->mMaterials.at(group.MaterialNo());
		if (mode & GLM_MATERIAL) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.Ambient());
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.Diffuse());
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.Specular());
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.Shiness());
		}

		if (mode & GLM_COLOR) {
			glColor3fv(model->mMaterials[0].Diffuse());
		}

		/* add code here to bind texture for each model */
		if (mode & GLM_TEXTURE) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, model->mTextureArray[group.MaterialNo()]);
		}

		glBegin(GL_TRIANGLES);
		for (unsigned int j = 0; j< group.Triangles().size(); j++) {
			Triangle triangle = model->mTriangles.at(group.Triangles().at(j));
			vindices = triangle.Vindices();
			nindices = triangle.Nindices();
			tindices = triangle.Tindices();

			if (mode & GLM_FLAT)
				glNormal3d(model->mFacetNormals[triangle.Findex()].X(),
					model->mFacetNormals[triangle.Findex()].Y(), 
					model->mFacetNormals[triangle.Findex()].Z());
			if (mode * GLM_SMOOTH)
			{
				glNormal3d(model->mNormals.at(nindices[0]).X(),
					model->mNormals.at(nindices[0]).Y(), 
					model->mNormals.at(nindices[0]).Z());
			}
			if (mode & GLM_TEXTURE) {
				glTexCoord2d(model->mTexcoods.at(tindices[0]).X(),
					model->mTexcoods.at(tindices[0]).Y());
			}

			glVertex3d(model->mVertices.at(vindices[0]).X(),
				model->mVertices.at(vindices[0]).Y(), 
				model->mVertices.at(vindices[0]).Z());

			if (mode * GLM_SMOOTH)
			{
				glNormal3d(model->mNormals.at(nindices[1]).X(),
					model->mNormals.at(nindices[1]).Y(),
					model->mNormals.at(nindices[1]).Z());
			}
			if (mode & GLM_TEXTURE) {
				glTexCoord2d(model->mTexcoods.at(tindices[1]).X(),
					model->mTexcoods.at(tindices[1]).Y());
			}

			glVertex3d(model->mVertices.at(vindices[1]).X(),
				model->mVertices.at(vindices[1]).Y(),
				model->mVertices.at(vindices[1]).Z());

			if (mode * GLM_SMOOTH)
			{
				glNormal3d(model->mNormals.at(nindices[2]).X(),
					model->mNormals.at(nindices[2]).Y(),
					model->mNormals.at(nindices[2]).Z());
			}
			if (mode & GLM_TEXTURE) {
				glTexCoord2d(model->mTexcoods.at(tindices[2]).X(),
					model->mTexcoods.at(tindices[2]).Y());
			}

			glVertex3d(model->mVertices.at(vindices[2]).X(),
				model->mVertices.at(vindices[2]).Y(),
				model->mVertices.at(vindices[2]).Z());

		}
		glEnd();

	}

}


void GLMWriteMTL(GLModel* model, char* modelpath, char* mtllibname) {
	FILE* file;
	char* dir;
	char* filename;
	float* ambient, *diffuse, *specular;
	dir = GLMDirName(modelpath);
	filename = (char*)malloc(sizeof(char)*(strlen(dir) + strlen(mtllibname)));
	strcpy(filename, dir);
	strcat(filename, mtllibname);
	free(dir);

	file = fopen(filename, "w");
	if (!file) {
		fprintf(stderr, "_glmWriteMTL() failed: can't open file %s\".\n", filename);
		exit(1);
	}
	free(filename);

	fprintf(file, "# \n");
	fprintf(file, "# MTL generated by GLM library");
	fprintf(file, "# \n");
	fprintf(file, "# \n\n");

	for (unsigned int i = 0; i< model->mMaterials.size(); i++) {
		Material material = model->mMaterials.at(i);
		ambient = material.Ambient();
		diffuse = material.Diffuse();
		specular = material.Specular();

		fprintf(file, "newmtl %s\n", material.Name());
		fprintf(file, "Ka %f %f %f\n", ambient[0], ambient[1], ambient[2]);
		fprintf(file, "Kd %f %f %f\n", diffuse[0], diffuse[1], diffuse[2]);
		fprintf(file, "Ks %f %f %f\n", specular[0], specular[1], specular[2]);
		fprintf(file, "Ns %f\n", material.Shiness());
		fprintf(file, "\n");
	}
}

/* glmWriteOBJ: Writes a model description in Wavefront .OBJ format to
* a file.
*
* model    - initialized GLMmodel structure
* filename - name of the file to write the Wavefront .OBJ format data to
* mode     - a bitwise or of values describing what is written to the file
*            GLM_NONE     -  render with only vertices
*            GLM_FLAT     -  render with facet normals
*            GLM_SMOOTH   -  render with vertex normals
*            GLM_TEXTURE  -  render with texture coords
*            GLM_COLOR    -  render with colors (color material)
*            GLM_MATERIAL -  render with materials
*            GLM_COLOR and GLM_MATERIAL should not both be specified.
*            GLM_FLAT and GLM_SMOOTH should not both be specified.
*/
void GLMWriteOBJ(GLModel* model, char* filename, GLuint mode) {
	FILE* file;
	Group group;
	Vertex vertex;
	Normal normal;
	Texture texture;
	Triangle triangle;
	int* vindices, *nindices, *tindices;
	int findex;

	if (mode & GLM_FLAT && model->mFacetNormals.size() == 0) {
		printf("glmWriteOBJ() warning: flat normal output requested with no facet normals defined.\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_SMOOTH && model->mNormals.size() == 0) {
		printf("glmWriteOBJ() warning: smooth normal output requested with no normals defined.\n");
		mode &= ~GLM_SMOOTH;
	}
	if (mode & GLM_TEXTURE && model->mTexcoods.size() == 0) {
		printf("glmWriteOBJ() warning: texture coordinate output requested with no texture coordinates defined.\n");
		mode &= ~GLM_TEXTURE;
	}
	if (mode & GLM_FLAT && mode & GLM_SMOOTH) {
		printf("glmWriteOBJ() warning: flat normal output requested and smooth normal requested(using smooth).\n");
		mode &= !GLM_FLAT;
	}

	file = fopen(filename, "w");
	if (!file) {
		fprintf(stderr, "glmWriteOBJ() failed: can't open file %s to write.\n", filename);
		exit(1);
	}

	fprintf(file, "# \n");
	fprintf(file, "OBJ generated by GLM library\n");
	fprintf(file, "# \n");

	if (mode & GLM_MATERIAL && model->mMtllibname) {
		fprintf(file, "\n mtllib %s\n\n", model->mMtllibname);
		GLMWriteMTL(model, filename, model->mMtllibname);
	}

	fprintf(file, "\n");
	fprintf(file, "#%d vertices\n", model->mVertices.size());

	for (unsigned int i = 0; i< model->mVertices.size(); i++) {
		vertex = model->mVertices[i];
		fprintf(file, "v %f %f %f\n", vertex.X(), vertex.Y(), vertex.Z());
	}

	if (mode & GLM_SMOOTH) {
		fprintf(file, "\n");
		fprintf(file, "# %d normals\n", model->mNormals.size());
		for (unsigned int i = 0; i < model->mNormals.size(); i++) {
			normal = model->mNormals[i];
			fprintf(file, "vn %f %f %f", normal.X(), normal.Y(), normal.Z());
		}
	}
	else if (mode & GLM_FLAT) {
		fprintf(file, "\n");
		fprintf(file, "# %d normals\n", model->mFacetNormals.size());
		for (unsigned int i = 0; i < model->mFacetNormals.size(); i++) {
			normal = model->mFacetNormals[i];
			fprintf(file, "vn %f %f %f", normal.X(), normal.Y(), normal.Z());
		}
	}

	if (mode & GLM_TEXTURE) {
		fprintf(file, "\n");
		fprintf(file, "# %d texcoords\n", model->mTexcoods.size());
		for (unsigned int i = 0; i< model->mTexcoods.size(); i++) {
			texture = model->mTexcoods[i];
			fprintf(file, "vt %f %f", texture.X(), texture.Y());
		}
	}

	fprintf(file, "\n");
	fprintf(file, "# %d groups\n", model->mGroups.size());
	fprintf(file, "#d faces(triangles)\n", model->mTriangles.size());
	fprintf(file, "\n");

	for (unsigned i = 0; i< model->mGroups.size(); i++) {
		group = model->mGroups[i];

		fprintf(file, "g %s \n", group.Name());
		if (mode & GLM_MATERIAL) {
			fprintf(file, "usemtl %s\n", model->mMaterials.at(group.MaterialNo()).Name());
			for (unsigned int j = 0; j < group.Triangles().size(); j++) {
				if (mode & GLM_SMOOTH && mode && GLM_TEXTURE) {
					triangle = model->mTriangles.at(group.Triangles().at(j));
					vindices = triangle.Vindices();
					nindices = triangle.Nindices();
					tindices = triangle.Tindices();
					fprintf(file, "f  %d/%d/%d  %d/%d/%d  %d/%d/%d\n",
						vindices[0], tindices[0], nindices[0],
						vindices[1], tindices[1], nindices[1],
						vindices[2], tindices[2], nindices[2]);
				}
				else if (mode & GLM_FLAT && mode & GLM_TEXTURE) {
					vindices = triangle.Vindices();
					findex = triangle.Findex();
					fprintf(file, "f  %d/%d  %d/%d  %d/%d\n",
						vindices[0], findex,
						vindices[1], findex,
						vindices[2], findex);
				}
				else if (mode & GLM_TEXTURE) {
					vindices = triangle.Vindices();
					tindices = triangle.Tindices();
					fprintf(file, "f  %d/%d  %d/%d  %d/%d\n",
						vindices[0], tindices[0],
						vindices[1], tindices[1],
						vindices[2], tindices[2]);
				}
				else if (mode & GLM_SMOOTH) {
					vindices = triangle.Vindices();
					nindices = triangle.Nindices();
					fprintf(file, "f  %d//%d  %d//%d  %d//%d\n",
						vindices[0], nindices[0],
						vindices[1], nindices[1],
						vindices[2], nindices[2]);
				}
				else if (mode & GLM_FLAT) {
					vindices = triangle.Vindices();
					findex = triangle.Findex();
					fprintf(file, "f  %d//%d  %d//%d  %d//%d\n",
						vindices[0], findex,
						vindices[1], findex,
						vindices[2], findex);
				}
				else {
					vindices = triangle.Vindices();
					fprintf(file, "f  %d  %d  %d\n", vindices[0], vindices[1], vindices[2]);
				}
			}
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

//////////////////////////////////////////////////////////////////////////
//使用SVD方法直接计算三维模型的对称轴方向
//
//返回值：三维模型的对称轴方向
//////////////////////////////////////////////////////////////////////////
void GLModel::CalAxisNormal() {
	//float sum_x=0,sum_x2=0,sum_y=0,sum_y2=0,sum_z=0,sum_z2=0,sum_xy=0,sum_xz=0,sum_yz = 0;
	////float M[9] = {0.0};
	//MatrixXf M(3,3);
	//M.setZero();
	//vector<Normal> verticeNormals = normals;
	//int verticeNormalNum = verticeNormals.size();

	////计算3*3矩阵，按照Andrea Tagliasacchi.2009提出的方法
	//for(int i=0; i < verticeNormalNum;i++){
	//	Normal tempNormal = verticeNormals[i];
	//	sum_x += tempNormal.getX();	sum_x2 += tempNormal.getX()* tempNormal.getX();
	//	sum_y += tempNormal.getY();	sum_y2 += tempNormal.getY()* tempNormal.getY();
	//	sum_z += tempNormal.getZ();	sum_z2 += tempNormal.getZ() * tempNormal.getZ();
	//	sum_xy += tempNormal.getX() * tempNormal.getY();
	//	sum_xz += tempNormal.getX() * tempNormal.getZ();
	//	sum_yz += tempNormal.getY() * tempNormal.getZ();
	//}
	//M(0,0) = sum_x2 / verticeNormalNum - sum_x*sum_x/(verticeNormalNum * verticeNormalNum);
	//M(0,1) = 2 / verticeNormalNum * sum_xy - 2 / (verticeNormalNum*verticeNormalNum) * sum_x * sum_y;
	//M(0,2) = 2 / verticeNormalNum * sum_xz - 2 / (verticeNormalNum*verticeNormalNum) * sum_x * sum_z;
	//M(1,0) = M(0,1);
	//M(1,1) = sum_y2 / verticeNormalNum - sum_y * sum_y /(verticeNormalNum * verticeNormalNum);
	//M(1,2) = 2 / verticeNormalNum * sum_yz - 2 / (verticeNormalNum * verticeNormalNum)*sum_y * sum_z;
	//M(2,0) = M(0,2);
	//M(2,1) = M(1,2);
	//M(2,2) = sum_z2 / verticeNormalNum - sum_z * sum_z / (verticeNormalNum * verticeNormalNum);

	////对矩阵M进行SVD分解
	//JacobiSVD<MatrixXf> svd(M,ComputeFullU|ComputeFullV);
	//Matrix3f U = svd.matrixU();
	//Vector3f S = svd.singularValues();
	//int minIndex = S(0)<S(1)?0:1;
	//minIndex = S(minIndex) < S(2)?minIndex:2;
	////cout<<"The cutting Normal:"<<U(minIndex)<<"  "<<U(minIndex+3)<<" "<<U(minIndex+6)<<endl;
	//axisNormal =  Normal(U(minIndex),U(minIndex+3),U(minIndex+6));
	mAxisNormal = Normal(0, 1, 0);
}
