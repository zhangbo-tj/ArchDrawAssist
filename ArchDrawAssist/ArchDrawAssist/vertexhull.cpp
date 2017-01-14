#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
#include"vertexHull.h"
using namespace std;

//�ж�������(������)�Ƿ����
bool operator==(const pPOINT &pt1, const pPOINT &pt2) {
    return (pt1.x == pt2.x && pt1.y == pt2.y);
}
// �Ƚ���������pt1��pt2�ֱ���x������(1, 0)�ļн�
bool CompareVector(const pPOINT &pt1, const pPOINT &pt2) {
    //��������ģ
    float m1 = sqrt((float)(pt1.x * pt1.x + pt1.y * pt1.y));
    float m2 = sqrt((float)(pt2.x * pt2.x + pt2.y * pt2.y));
    //���������ֱ���(1, 0)���ڻ�
    float v1 = pt1.x / m1, v2 = pt2.x / m2;
    return (v1 > v2 || (v1 == v2 && m1 < m2));
}
//����͹��
void CalcConvexHull(vector<pPOINT> &vecSrc) {
    //�㼯������Ӧ��3���㣬���ܹ��ɶ����
    if (vecSrc.size() < 3) {
        return;
    }
    //���һ���
    pPOINT ptBase = vecSrc.front(); //����1����Ԥ��Ϊ��С��
    for (vector<pPOINT>::iterator i = vecSrc.begin() + 1; i != vecSrc.end(); ++i) {
        //�����ǰ���yֵС����С�㣬��yֵ��ȣ�xֵ��С
        if (i->y < ptBase.y || (i->y == ptBase.y && i->x > ptBase.x)) {
            //����ǰ����Ϊ��С��
            ptBase = *i;
        }
    }
    //�������������㹹�ɵ�����
    for (vector<pPOINT>::iterator i = vecSrc.begin(); i != vecSrc.end();) {
        //�ų��������ͬ�ĵ㣬����������������г��ֳ�0����
        if (*i == ptBase) {
            i = vecSrc.erase(i);
        }
        else {
            //�����ɻ��㵽Ŀ���
            i->x -= ptBase.x, i->y -= ptBase.y;
            ++i;
        }
    }
    //���������������֮��ļн�����
    sort(vecSrc.begin(), vecSrc.end(), &CompareVector);
    //ɾ����ͬ������
    vecSrc.erase(unique(vecSrc.begin(), vecSrc.end()), vecSrc.end());
    //����õ���β��������������
    for (vector<pPOINT>::reverse_iterator ri = vecSrc.rbegin();
        ri != vecSrc.rend() - 1; ++ri) {
        vector<pPOINT>::reverse_iterator riNext = ri + 1;
        //���������μ��㹫ʽ
        ri->x -= riNext->x, ri->y -= riNext->y;
    }
    //����ɾ������͹���ϵ�����
    for (vector<pPOINT>::iterator i = vecSrc.begin() + 1; i != vecSrc.end(); ++i) {
        //����ɾ����ת�����෴��������ʹ������ж���ת����
        for (vector<pPOINT>::iterator iLast = i - 1; iLast != vecSrc.begin();) {
            float v1 = i->x * iLast->y, v2 = i->y * iLast->x;
            //������С��0������û��������ת
            //����������0�������жϷ����Ƿ�����
            if (v1 < v2 || (v1 == v2 && i->x * iLast->x > 0 &&
                i->y * iLast->y > 0)) {
                    break;
            }
            //ɾ��ǰһ������������µ�ǰ��������ǰ���������β����
            //���������μ��㹫ʽ
            i->x += iLast->x, i->y += iLast->y;
            iLast = (i = vecSrc.erase(iLast)) - 1;
        }
    }
    //��������β���������������ۼӣ����������
    vecSrc.front().x += ptBase.x, vecSrc.front().y += ptBase.y;
    for (vector<pPOINT>::iterator i = vecSrc.begin() + 1; i != vecSrc.end(); ++i) {
        i->x += (i - 1)->x, i->y += (i - 1)->y;
    }
    //��ӻ��㣬ȫ����͹���������
    vecSrc.push_back(ptBase);
}
 
//int main(void) {
//    int nPtCnt = 100; //���ɵ��������
//    vector<POINT> vecSrc, vecCH;
//    for (int i = 0; i < nPtCnt; ++i) {
//        POINT ptIn = { rand() % 20, rand() % 20 };
//        vecSrc.push_back(ptIn);
//        cout << ptIn.x << ", " << ptIn.y << endl;
//    }
//    CalcConvexHull(vecSrc);
//    cout << "\nConvex Hull:\n";
//    for (vector<POINT>::iterator i = vecSrc.begin(); i != vecSrc.end(); ++i) {
//        cout << i->x << ", " << i->y << endl;
//    }
//    return 0;
//}
