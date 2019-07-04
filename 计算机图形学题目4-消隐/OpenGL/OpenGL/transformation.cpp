#include <math.h>
#include <FLOAT.H>
#include <stdio.h>
#include "transformation.h"
// ƽ��(tx,ty,tz)�ľ���Txyz 
void matTxyz(float Txyz[4][4],float tx,float ty,float tz)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			Txyz[i][j]=0;
	Txyz[0][0]=1; Txyz[1][1]=1;  Txyz[2][2]=1;  Txyz[3][3]=1;
	Txyz[0][3]=tx; Txyz[1][3]=ty; Txyz[2][3]=tz; 
}

// ��x����תtheta�ǵľ���Rx 
void matRx(float Rx[4][4],float theta)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			Rx[i][j]=0;
	Rx[0][0]=1; Rx[3][3]=1;
	Rx[1][1]=cos(theta);
	Rx[1][2]=-sin(theta);
	Rx[2][1]=sin(theta);
	Rx[2][2]=cos(theta);
}

// ��y����תtheta�ǵľ���Ry 
void matRy(float Ry[4][4],float theta)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			Ry[i][j]=0;
	Ry[1][1]=1;  Ry[3][3]=1;
	Ry[0][0]=cos(theta);
	Ry[0][2]=sin(theta);
	Ry[2][0]=-sin(theta);
	Ry[2][2]=cos(theta);
}

// ��z����תtheta�ǵľ���Rz 
void matRz(float Rz[4][4],float theta)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			Rz[i][j]=0;
	Rz[2][2]=1;  Rz[3][3]=1;
	Rz[0][0]=cos(theta);
	Rz[0][1]=-sin(theta);
	Rz[1][0]=sin(theta);
	Rz[1][1]=cos(theta);
}

//��ָ����(���(0,0,0),�յ�(x,y,z))��תtheta�ǵľ���R
void matR(float R[4][4],float theta,float x,float y,float z)
{
	float c=cos(theta),s=sin(theta),onec=1-c;
	float len=sqrt(x*x+y*y+z*z);
	if(len<=0)return;
	float ux=x/len,uy=y/len,uz=z/len;

	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			R[i][j]=0;
	R[3][3]=1;

	R[0][0]=ux*ux*onec+c;
	R[0][1]=ux*uy*onec-uz*s;
	R[0][2]=ux*uz*onec+uy*s;
	R[1][0]=uy*ux*onec+uz*s;
	R[1][1]=uy*uy*onec+c;
	R[1][2]=uy*uz*onec-ux*s;
	R[2][0]=uz*ux*onec-uy*s;
	R[2][1]=uz*uy*onec+ux*s;
	R[2][2]=uz*uz*onec+c;
}

// ��������Sxyz
void matSxyz(float Sxyz[4][4],float sx,float sy,float sz)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			Sxyz[i][j]=0;
	Sxyz[3][3]=1;
	Sxyz[0][0]=sx;
	Sxyz[1][1]=sy;
	Sxyz[2][2]=sz;
}

//��λ����
void SetIdentity(float M[4][4])
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			M[i][j]=(i==j?1:0);
}


//���󿽱�,MD=MS
void Copy(float MD[4][4],float MS[4][4])
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			MD[i][j]=MS[i][j];
}

//����ת��
void Transpose(float MD[4][4],float MS[4][4])
{
	float TM[4][4];
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			TM[i][j]=MS[i][j];
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			MD[i][j]=TM[j][i];

}
// 4�׷������ R=A*B 
void MatMul(float R[4][4],float A[4][4],float B[4][4])
{
	int i,j,k;
	float TR[4][4];

	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			TR[i][j] = 0; 
			for(k=0;k<4;k++)
				TR[i][j] = TR[i][j]+ A[i][k]*B[k][j];
		}
	}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			R[i][j]=TR[i][j];
}

// 4�׷�����4ά������� PV=Mat*Vector 
void MVMul(float PV[4],float Mat[4][4],float Vector[4])
{
	int i,j;
	for(i=0;i<4;i++) {
		PV[i] = 0;
		for(j=0;j<4;j++) {
			PV[i] = PV[i] + Mat[i][j]*Vector[j];
		}
	}
}
