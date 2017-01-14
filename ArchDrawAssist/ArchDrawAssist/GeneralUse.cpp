
#include "GeneralUse.h"

/************************************************************************/
/* 计算数组的方差
输入:
X:数组指针
n:数组大小
bYangben:是否为样本点集
输出：
数组的方差
/************************************************************************/
double Variance(double* X, int n, bool bYangben) {
	if (n == 1) {
		return 0;
	}
	int i;
	double ave = 0;
	double sum = 0;
	for (i = 0; i < n; i++) {
		sum += X[i];
	}
	ave = sum / n;
	sum = 0;
	for (i = 0; i< n; i++) {
		sum += (X[i] - ave)*(X[i] - ave);
	}

	if (bYangben) {
		ave = sum / (n - 1);
	}
	else {
		ave = sum / n;
	}
	return sqrt(ave);
}

/************************************************************************/
/* 判断两个向量是否平行
输入：
v1,v2: 两个向量
err:误差
输出：
返回值：是否平行
/************************************************************************/
bool IsParallel(VECTOR3 &v1, VECTOR3 &v2, double err) {
	VECTOR3 v3 = cross(v1, v2);
	double sina = v3.Magnitude();
	if (sina < err) {
		return true;
	}
	else {
		return false;
	}
}


/************************************************************************/
/*判断两个向量是否正交
输入：
v1,v2:三维向量
err:误差
输出：
返回值：正交则返回true,不正交则返回false
/************************************************************************/
bool IsOrthogonal(VECTOR3 &v1, VECTOR3 &v2, double err) {
	double cosa = fabs(dot(v1, v2));
	if (cosa < err) {
		return true;
	}
	else {
		return false;
	}
}

//线段与平面相交，线段lsgPo,lsgP1,  平面：pP with pV
// bool PlaneLineSegmentIntersecPt(double pV[3],double pP[3],double lsgP0[3],double lsgP1[3],double interPt[3]){
// 	//先计算点到平面的有向距离，只有距离之积<0才可能相交
// 	double lV[3];	//lsgP0 -> lsgP1
// 	lV[0] = lsgP0[0]-lsgP1[0];	lV[1] = lsgP0[1] - lsgP1[1];	lV[2] = lsgP0[2] - lsgP1[2];
// 	double tempLength  = sqrt(lV[0]*lV[0]+lV[1]*lV[1]+lV[2]*lV[2]);
// 	if(tempLength < 1.0e-14){
// 		return false;
// 	}
// 	lV[0] /= tempLength;	lV[1] /= tempLength;	lV[2] /=tempLength;
// 
// 	double vpt = lV[0]*pV[0]+lV[1]*pV[1]+lV[2]*pV[2];
// 	if(fabs(vpt)<1.0e-8){
// 		return false;
// 	}
// 	
// 	//length为线段端点到平面上一点连线在直线投影长度
// 	double length = ((pP[0]-lsgP0[0])*pV[0] + (pP[1]-lsgP0[1])*pV[1]+(pP[2]-lsgP0[2])*pV[2])/vpt;
// 	if(length <0 || length >tempLength){
// 		return false;
// 	}
// 
// 	interPt[0] = lsgP0[0] + lV[0]*length;
// 	interPt[1] = lsgP0[1] + lV[1]*length;
// 	interPt[2] = lsgP0[2] + lV[2]*length;
// 	return true;
// }

bool PlaneLineSegmentIntersectPt(double pV[3], double pP[3], double lsgP0[3], double lsgP1[3], double interPt[3])
{
	//先计算点到平面的有向距离，只有距离之积 <= 0 才可能相交
	double lV[3]; // lsgP0 -> lsgP1
	lV[0] = lsgP1[0] - lsgP0[0]; lV[1] = lsgP1[1] - lsgP0[1]; lV[2] = lsgP1[2] - lsgP0[2];
	double n = sqrt(lV[0] * lV[0] + lV[1] * lV[1] + lV[2] * lV[2]);
	if (n<1.0e-14) //向量模为0
		return false;
	lV[0] /= n; lV[1] /= n; lV[2] /= n;

	double vpt = lV[0] * pV[0] + lV[1] * pV[1] + lV[2] * pV[2]; //点积判断是否平行
	if (fabs(vpt) < 1.0e-8) //线段与平面平行
		return false;

	// t为线段端点与平面上一点连线在直线投影长度
	double t = ((pP[0] - lsgP0[0]) * pV[0] + (pP[1] - lsgP0[1]) * pV[1] + (pP[2] - lsgP0[2]) * pV[2]) / vpt;
	if (t<0 || t>n) //超出线段范围
		return false;

	interPt[0] = lsgP0[0] + lV[0] * t;
	interPt[1] = lsgP0[1] + lV[1] * t;
	interPt[2] = lsgP0[2] + lV[2] * t;
	return true;
}



void RotMatrixBetween2Vector(VECTOR3 v_current, VECTOR3 v_dest, double r[9]) {

	VECTOR3 k = cross(v_current, v_dest);
	k.Normalize();

	double cosa = dot(v_current, v_dest);
	LimitTo(cosa, -1.0, 1.0);

	double sina = sqrt(1 - cosa * cosa);
	double crossK[9], crossK2[9];
	crossK[0] = crossK[4] = crossK[8] = 0;
	crossK[1] = -k.vec[2];	crossK[3] = k.vec[2];
	crossK[2] = k.vec[1];	crossK[6] = -k.vec[1];
	crossK[5] = -k.vec[0];	crossK[7] = k.vec[0];

	MatrixCalculate mc;
	mc.MultiplyMatrix(crossK, crossK, crossK2, 3, 3, 3);
	double identityM[9] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
	for (int i = 0; i < 9; i++) {
		r[i] = identityM[i] + sina * crossK[i] + (1 - cosa)*crossK2[i];
	}

	cosa = r[2] * v_dest[0] + r[5] * v_dest[1] * r[8] * v_dest[2];
	if (cosa < 0.9) {
		for (int i = 0; i < 9; i++) {
			r[i] *= -1;
		}
	}

}


void LimitTo(double &x, double lower_limit, double upper_limit) {
	if (x < lower_limit)
		x = lower_limit;
	else if (x >= upper_limit)
		x = upper_limit;
}
void LimitTo(int &x, int lower_limit, int upper_limit) {
	if (x < lower_limit)
		x = lower_limit;
	else if (x >= upper_limit)
		x = upper_limit;
}


MatrixCalculate::MatrixCalculate()
{

}

MatrixCalculate::~MatrixCalculate()
{

}
//////////////////////////////////////////////////////////////////////////
//功  能：	矩阵求逆
//输  入：	矩阵a，阶数n。
//输  出：	返回1（成功），0（失败），经求逆后，a矩阵为逆矩阵
bool MatrixCalculate::InverseMatrix(double *m1, int n)
{
	int *is, *js;
	int i, j, k, l, u, v;
	double temp, max_v;
	is = (int *)malloc(n * sizeof(int));
	js = (int *)malloc(n * sizeof(int));
	if (is == NULL || js == NULL) {
		printf("out of memory!\n");
		return(0);
	}
	for (k = 0; k<n; k++) {
		max_v = 0.0;
		for (i = k; i<n; i++)
			for (j = k; j<n; j++) {
				temp = fabs(m1[i*n + j]);
				if (temp>max_v) {
					max_v = temp; is[k] = i; js[k] = j;
				}
			}
		if (max_v == 0.0) {
			free(is); free(js);
			printf("invers is not availble!\n");
			return(0);
		}
		if (is[k] != k)
			for (j = 0; j<n; j++) {
				u = k*n + j; v = is[k] * n + j;
				temp = m1[u]; m1[u] = m1[v]; m1[v] = temp;
			}
		if (js[k] != k)
			for (i = 0; i<n; i++) {
				u = i*n + k; v = i*n + js[k];
				temp = m1[u]; m1[u] = m1[v]; m1[v] = temp;
			}
		l = k*n + k;
		m1[l] = 1.0 / m1[l];
		for (j = 0; j<n; j++)
			if (j != k) {
				u = k*n + j;
				m1[u] *= m1[l];
			}
		for (i = 0; i<n; i++)
			if (i != k)
				for (j = 0; j<n; j++)
					if (j != k) {
						u = i*n + j;
						m1[u] -= m1[i*n + k] * m1[k*n + j];
					}
		for (i = 0; i<n; i++)
			if (i != k) {
				u = i*n + k;
				m1[u] *= -m1[l];
			}
	}
	for (k = n - 1; k >= 0; k--) {
		if (js[k] != k)
			for (j = 0; j<n; j++) {
				u = k*n + j; v = js[k] * n + j;
				temp = m1[u]; m1[u] = m1[v]; m1[v] = temp;
			}
		if (is[k] != k)
			for (i = 0; i<n; i++) {
				u = i*n + k; v = i*n + is[k];
				temp = m1[u]; m1[u] = m1[v]; m1[v] = temp;
			}
	}
	free(is); free(js);
	return(1);
}

//////////////////////////////////////////////////////////////////////////
//功  能：	矩阵转置
//输  入：	矩阵a（mxn阶）转置后存在矩阵at（nxm阶）中
//输  出：	void，结果在矩阵at中
void MatrixCalculate::TransposeMatrix(double *a, double *at, int m, int n)
{
	int i, j;
	for (i = 0; i<n; i++)
		for (j = 0; j<m; j++)
			*(at + i*m + j) = *(a + j*n + i);
}

//////////////////////////////////////////////////////////////////////////
//功  能：	矩阵相乘
//输  入：	矩阵a（mxn阶）乘以矩阵b（nxk阶）等于矩阵c（mxk阶）。
//输  出：	void，结果在矩阵c中
void MatrixCalculate::MultiplyMatrix(double *a, double *b, double *c, int m, int n, int k)
{
	int i, j, ii;
	for (i = 0; i<m; i++)
	{
		for (j = 0; j<k; j++)
		{
			*(c + i*k + j) = 0;
			for (ii = 0; ii<n; ii++)
				*(c + i*k + j) = *(c + i*k + j) + *(a + i*n + ii)**(b + ii*k + j);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//功  能：	矩阵加减
//输  入：	矩阵a（mxn阶）减or 加矩阵b（mxn阶）等于矩阵c（mxn阶）。
//输  出：	void，结果在矩阵c中
void MatrixCalculate::MinusMatrix(double *a, double *b, double *c, int m, int n)
{
	for (int i = 0; i<m; i++)
		for (int j = 0; j<n; j++) {
			*(c + i*n + j) = *(a + i*n + j) - *(b + i*n + j);
		}

}
void MatrixCalculate::AddMatrix(double *a, double *b, double *c, int m, int n)
{
	for (int i = 0; i<m; i++)
		for (int j = 0; j<n; j++) {
			*(c + i*n + j) = *(a + i*n + j) + *(b + i*n + j);
		}

}

//////////////////////////////////////////////////////////////////////////
//功  能：	解线性方程组m个方程解n个未知数（m>n）方程形式ax=b
//输  入：	方程系数矩阵a(m行n列)，未知数指针(内存大小n)，方程常数项矩阵b(m行1列)，方程个数m，未知数个数n
//输  出：	void，结果在矩阵x中
bool MatrixCalculate::ResoveLineEquations(double *a, double *x, double *b, int m, int n)
{
	double *at = (double *)malloc(sizeof(double)*m*n);
	double *ata = (double *)malloc(sizeof(double)*n*n);
	double *atb = (double *)malloc(sizeof(double)*n);
	TransposeMatrix(a, at, m, n);
	MultiplyMatrix(at, a, ata, n, m, n);
	MultiplyMatrix(at, b, atb, n, m, 1);
	bool bsucess = InverseMatrix(ata, n);
	if (bsucess)
		MultiplyMatrix(ata, atb, x, n, n, 1);

	free(at);
	free(ata);
	free(atb);

	return bsucess;
}

bool MatrixCalculate::ResoveEquations(double *a, double *x, double *b, int n)
{
	int m = n;
	double *at = (double *)malloc(sizeof(double)*m*n);
	double *ata = (double *)malloc(sizeof(double)*n*n);
	double *atb = (double *)malloc(sizeof(double)*n);
	TransposeMatrix(a, at, m, n);
	MultiplyMatrix(at, a, ata, n, m, n);
	MultiplyMatrix(at, b, atb, n, m, 1);
	bool bsucess = InverseMatrix(ata, n);
	if (bsucess)
		MultiplyMatrix(ata, atb, x, n, n, 1);

	free(at);
	free(ata);
	free(atb);

	return bsucess;
}

bool MatrixCalculate::ResoveLineEquations_diaQ(double *a, double *x, double *b, double *p, int m, int n)
{
	double *atp = (double *)malloc(sizeof(double)*m*n);
	double *atpa = (double *)malloc(sizeof(double)*n*n);
	double *atpb = (double *)malloc(sizeof(double)*n);
	//atp
	TransposeMatrix(a, atp, m, n);
	for (int i = 0; i<n; i++) {
		for (int j = 0; j<m; j++) {
			atp[i*m + j] *= p[j];
		}
	}
	MultiplyMatrix(atp, a, atpa, n, m, n);
	MultiplyMatrix(atp, b, atpb, n, m, 1);

	bool bsucess = InverseMatrix(atpa, n);
	if (bsucess)
		MultiplyMatrix(atpa, atpb, x, n, n, 1);

	free(atp);
	free(atpa);
	free(atpb);

	return bsucess;
}

bool MatrixCalculate::ResoveLineEquations(double *a, double *x, double *b, double *p, int m, int n)
{
	double *at = (double *)malloc(sizeof(double)*m*n);
	double *atp = (double *)malloc(sizeof(double)*n*m);
	double *atpa = (double *)malloc(sizeof(double)*n*n);
	double *atpb = (double *)malloc(sizeof(double)*n);
	TransposeMatrix(a, at, m, n);
	MultiplyMatrix(at, p, atp, n, m, m);
	MultiplyMatrix(atp, a, atpa, n, m, n);
	MultiplyMatrix(atp, b, atpb, n, m, 1);

	bool bsucess = InverseMatrix(atpa, n);
	if (bsucess)
		MultiplyMatrix(atpa, atpb, x, n, n, 1);

	free(at);
	free(atp);
	free(atpa);
	free(atpb);

	return bsucess;
}
//////////////////////////////////////////////////////////////////////////
//
//
//
double MatrixCalculate::GetStandardDeviation(double *d, int Rows) {
	double sum = 0.0, tmp = 0.0, average, deviation, standardDeviation;
	int i;
	for (i = 0; i<Rows; i++) {
		sum += *(d + i);
	}
	average = sum / Rows;
	for (i = 0; i<Rows; i++) {
		tmp += pow((*(d + i) - average), 2);
	}
	deviation = tmp / Rows;
	standardDeviation = sqrt(deviation);
	return standardDeviation;
}

bool CMyBinaryTree::MyInsertElement(int a, MyBTNode *root)
{
	if (a < root->m_value) {
		if (root->pleft != NULL)
			return MyInsertElement(a, root->pleft);
		else {
			root->pleft = new MyBTNode;
			root->pleft->m_value = a;
			return true;
		}
	}
	else if (a > root->m_value)
	{
		if (root->pright != NULL)
			return MyInsertElement(a, root->pright);
		else {
			root->pright = new MyBTNode;
			root->pright->m_value = a;
			return true;
		}
	}

	return false;
}

void CMyBinaryTree::MyCountTree(MyBTNode* root, int &n)
{
	if (root->pleft != NULL) {
		n++;
		MyCountTree(root->pleft, n);
	}

	if (root->pright != NULL) {
		n++;
		MyCountTree(root->pright, n);
	}
}

void CMyBinaryTree::MyTravelTree(std::vector<int> &v_tree, MyBTNode *root)
{
	v_tree.push_back(root->m_value);

	if (root->pleft != NULL)
		MyTravelTree(v_tree, root->pleft);

	if (root->pright != NULL)
		MyTravelTree(v_tree, root->pright);
}

void CMyBinaryTree::MyDeleteTree(MyBTNode* root)
{
	if (root->pleft != NULL) {
		MyDeleteTree(root->pleft);
		delete root->pleft;
		root->pleft = NULL;
	}

	if (root->pright != NULL) {
		MyDeleteTree(root->pright);
		delete root->pright;
		root->pright = NULL;
	}
}

void CMyBinaryTree::MyConstructBinaryTree(std::vector<int> &v_values)
{
	MyBTNode root;
	std::vector<int>::iterator iter_v = v_values.begin();

	for (root.m_value = *iter_v; iter_v != v_values.end(); iter_v++)
		MyInsertElement(*iter_v, &root);
}

void CMyBinaryTree::resetroot(int root)
{
	MyDeleteTree(&m_root);
	m_root.m_value = root;
	m_root.pleft = NULL;
	m_root.pright = NULL;
}

bool CMyBinaryTree::InsertElement(int a)
{
	return MyInsertElement(a, &m_root);
}

int CMyBinaryTree::CountTree()
{
	int n = 0;
	MyCountTree(&m_root, n);
	return n;
}

void CMyBinaryTree::TravelTree(std::vector<int> &v_tree)
{
	MyTravelTree(v_tree, &m_root);
}
//////////////////////////////////////////////////////////////////////////
//获取三个点中的最小值
//@param	a,b,c:三个需要进行比较的值
//@return	三个数值中的最小值
//////////////////////////////////////////////////////////////////////////
float getmin(float a, float b, float c) {
	float min = a < b ? a : b;
	min = min < c ? min : c;
	return min;
}

//////////////////////////////////////////////////////////////////////////
//计算三个点中的最大值
//@param	a,b,c:三个需要进行比较的数值
//@return	三个数值中的最大值
//////////////////////////////////////////////////////////////////////////
float getmax(float a, float b, float c) {
	float max = a > b ? a : b;
	max = max > c ? max : c;
	return max;
}

