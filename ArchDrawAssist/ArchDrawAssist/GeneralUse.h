#pragma once

#include "KDTree\matrix.h"
#include <random>

struct RotMatrix3
{
	double r[9];

	RotMatrix3()
	{
		reset();
	}

	RotMatrix3 &operator = (const RotMatrix3 & rot)
	{
		memcpy(r, rot.r, sizeof(double) * 9);
		return *this;
	}

	void reset()
	{
		memset(r, 0, sizeof(double) * 9);
		r[0] = r[4] = r[8] = 1.0;
	}
};

/************************************************************************/
/* 获取数组中最大值所在索引                                                                     */
/************************************************************************/
template<typename T>
int MaxIndex(T* x, int num, T* err = NULL) {
	int maxindex = 0;
	T maxx;
	int start = 0;

	if (err == NULL) {
		start = 0;
	}
	else {
		for (int i = 0; i<num; i++) {
			if (x[i] != *err) {
				start = i;
				break;
			}
		}
	}

	maxx = x[start];
	maxindex = start;
	for (int i = start + 1; i<num; i++) {
		if ((err != NULL) && (x[i] == *err)) {
			continue;
		}
		if (x[i] >maxx) {
			maxx = x[i];
			maxindex = i;
		}
	}
	if (err != NULL &&x[maxindex] == *err) {
		return -1;
	}
	else
	{
		return maxindex;
	}
}

/************************************************************************/
/* 计算数组中最小值的索引                                                                     */
/************************************************************************/
template<typename T>
int MinIndex(T* x, int num, T *err = NULL) {
	int minindex = 0;
	T minx;
	int start = 0;
	if (err == NULL) {
		start = 0;
	}
	else {
		for (int i = 0; i< num; i++) {
			if (x[i] != *err) {
				start = i;
				break;
			}
		}
	}

	minx = x[start];
	minindex = start;
	for (int i = start + 1; i < num; i++) {
		if (err != NULL&&x[i] == *err) {
			continue;
		}
		if (x[i]<minx) {
			minx = x[i];
			minindex = i;
		}
	}
	if (err != NULL && x[minindex] == *err)
		return -1;
	else
		return minindex;
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

/************************************************************************/
/* 生成0-1之间的随机数                                                                     */
/************************************************************************/
float RandomFloat()
{
	return static_cast<float>(dis(gen));
}

float Distance(float A1[], float A2[], int dimention)
{
	float sum = 0;
	for (int i = 0; i<dimention; i++)
		sum += (A1[i] - A2[i])*(A1[i] - A2[i]);

	return sqrt(sum);
}
float Distance(double A1[], double A2[], int dimention)
{
	double sum = 0;
	for (int i = 0; i<dimention; i++)
		sum += (A1[i] - A2[i])*(A1[i] - A2[i]);

	return sqrt(sum);
}

/************************************************************************/
/* 计算数组的方差
输入:
X:数组指针
n:数组大小
bYangben:是否为样本点集
输出：
数组的方差
/************************************************************************/
double Variance(double* X, int n, bool bYangben);

//判断两个向量是否平行
bool IsParallel(VECTOR3 &v1, VECTOR3 &v2, double err);
//判断两个向量是否正交
bool IsOrthogonal(VECTOR3 &v1, VECTOR3 &v2, double err);
//线段与平面相交，线段lsgPo,lsgP1,  平面：pP with pV
bool PlaneLineSegmentIntersectPt(double pV[3], double pP[3], double lsgP0[3], double lsgP1[3], double interPt[3]);

void RotMatrixBetween2Vector(VECTOR3 v_current, VECTOR3 v_dest, double r[9]);

void LimitTo(double &x, double lower_limit, double upper_limit);
void LimitTo(int &x, int lower_limit, int upper_limit);

class MatrixCalculate
{
public:
	MatrixCalculate();
	virtual ~MatrixCalculate();
public:
	bool InverseMatrix(double *m1, int n);
	bool ResoveLineEquations(double *a, double *x, double *b, int m, int n);
	void TransposeMatrix(double *a, double *at, int m, int n);
	void MultiplyMatrix(double *a, double *b, double *c, int m, int n, int k);
	void MinusMatrix(double *a, double *b, double *c, int m, int n);
	void AddMatrix(double *a, double *b, double *c, int m, int n);
	double GetStandardDeviation(double *d, int Rows);

	//带权最小二乘
	bool ResoveLineEquations(double *a, double *x, double *b, double *p, int m, int n);
	bool ResoveLineEquations_diaQ(double *a, double *x, double *b, double *p, int m, int n);

	//2011-4-19
	bool ResoveEquations(double *a, double *x, double *b, int n);
};

typedef struct MYBINARYTREENODE
{
	int m_value;
	MYBINARYTREENODE* pleft;
	MYBINARYTREENODE* pright;

	MYBINARYTREENODE()
	{
		pleft = NULL;
		pright = NULL;
	}
}MyBTNode;

class CMyBinaryTree
{
public:
	CMyBinaryTree(int root) { m_root.m_value = root; }
	~CMyBinaryTree() { MyDeleteTree(&m_root); }

	void resetroot(int root);
	bool InsertElement(int a);
	int CountTree();
	void TravelTree(std::vector<int> &v_tree);

	void MyConstructBinaryTree(std::vector<int> &v_values);

private:
	MyBTNode m_root;
	bool MyInsertElement(int a, MyBTNode *root);
	void MyCountTree(MyBTNode* root, int &n);
	void MyTravelTree(std::vector<int> &v_tree, MyBTNode *root);
	void MyDeleteTree(MyBTNode* root);
};

float getmin(float a, float b, float c);
float getmax(float a, float b, float c);