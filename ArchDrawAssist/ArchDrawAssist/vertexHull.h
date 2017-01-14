#pragma once
#include <vector>
#include <iostream>
using namespace std;
//二维点(或向量)结构体定义

struct pPOINT { float x; float y; };


void CalcConvexHull(vector<pPOINT> &vecSrc);