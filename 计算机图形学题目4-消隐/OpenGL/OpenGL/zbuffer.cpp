#include "common.h"
#include "math.h"
#include "zbuffer.h"
#include <iostream>
using namespace std;

//��ɨ����Y=y���AB�Ľ����x���꣬����޽��㣬����-1
int linejoin(int A[2],int B[2],int y)
{
	float t;
	if (B[1]==A[1])
		return -1;

	t = (y-A[1]) / float(B[1]-A[1]);
	if (t>=0.0 && t<=1.0){
		return ROUND(A[0] + (B[0]-A[0])*t);
	}else
		return -1;
}

//�����õ��ĺ���
void SetPixel(int x,int y);
float GetDepth(int x,int y);
void SetDepth(int x,int y,float depth);


void getX(int X[2],int y, int coord[][2], int n)
{
	X[0] = 100000, X[1] = -100000;
	for (int i = 0 ;i < n; i++)
	{
		int cur[2], nex[2];
		cur[0] = coord[i][0], cur[1] = coord[i][1];
		nex[0] = coord[(i + 1) % n][0], nex[1] = coord[(i + 1) % n][1];
		int res = linejoin(cur, nex, y);

		if (X[0] >= res && res > 0) X[0] = res;
		if (X[1] <= res && res > 0) X[1] = res;
	}
}
//ɨ��������㷨
//coord �������� ,coord[0],...coord[n-1]������������һ�����������
//depth ���ض�Ӧ�����
//n ���ظ���
void linefill(int coord[][2],float depth[],int n)
{	
	int miny,maxy;
	int X[10];
	float A[3],B[3],C[3],D[3],E[3],R[3];

	R[2] = 1;

	// �ҳ�y����������С�Ķ��� 
	miny=coord[0][1];maxy=coord[0][1];
	for (int i=0;i<n;i++){
		if (coord[i][1]<miny)
			miny=coord[i][1];
		if (coord[i][1]>maxy)
			maxy=coord[i][1];
	}

	// ABC ���ƽ�� ����AΪ�㣬BCΪ����
	A[0] = coord[0][0]; 
	A[1] = coord[0][1]; 
	A[2] = depth[0];
	
	B[0] = coord[1][0] - coord[0][0]; 
	B[1] = coord[1][1] - coord[0][1]; 
	B[2] = depth[1] - depth[0];

	C[0] = coord[3][0] - coord[0][0]; 
	C[1] = coord[3][1] - coord[0][1]; 
	C[2] = depth[3] - depth[0];

	E[0] = 0.0; E[1] = 0.0; E[2] = 1.0;	

	// ʹɨ���ߴ�y������С�ĵ���1��y���Ķ��� 
	for (int y=miny;y<=maxy;y++)
	{
		int X[2];
		// ��ȡɨ���������ε��ཻ��
		getX(X, y, coord, n);
		for (int x = X[0]; x <= X[1]; x++)
		{
			D[0] = x; D[1] = y; D[2] = 0;	
			facejoin(R, A, B, C, D, E);
			float oldDepth = GetDepth(x, y);
			float newDepth = R[2];
			// �Ƚϻ������ȣ�ԽС��Խ�����۲���
			if (newDepth <= oldDepth)
			{
				SetPixel(x, y);
				SetDepth(x, y, newDepth);
			}
		}
	}
}
