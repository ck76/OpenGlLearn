#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "transformation.h"
#include "common.h"
#include "line.h"

#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700								

//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

#define PI 3.1415926

//��ɫ
struct RGB{
	RGB(GLubyte nr=255,GLubyte ng=255,GLubyte nb=255):r(nr),g(ng),b(nb){}
	GLubyte r,g,b;
};
typedef RGB COLOR;

COLOR g_color=RGB(255,255,255);

float g_vertex[8][3]={{-50,-50,50},{50,-50,50},{50,50,50},{-50,50,50},{50,-50,-50},{50,50,-50},{-50,50,-50},{-50,-50,-50}};//�����嶥��
float g_pvertex[8][3];//�任��������嶥��
float Txyz[4][4],Rxyz[4][4],Sxyz[4][4],ComT[4][4]; //ƽ��,��ת,���ž��� 



//�Զ��庯��---------------------------------------------------------------------------

//��λ����
void SetIdentity(float m[4][4])
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			m[i][j]=(i==j?1:0);
}

//����任
void VerTransfer(float pv[][3],float v[][3],int n,float m[4][4])
{
	static float tpv[4],tv[4];

	for(int i=0;i<n;i++){
		tv[0]=v[i][0];
		tv[1]=v[i][1];
		tv[2]=v[i][2];
		tv[3]=1;
		MVMul(tpv,m,tv);
		pv[i][0]=tpv[0];
		pv[i][1]=tpv[1];
		pv[i][2]=tpv[2];
	}

}

//-------------------------------------------------------------------------------------

//��ʼ��
void MyInit()
{
	int i;
	float r[4][4];

	SetIdentity(Txyz);
	SetIdentity(Rxyz);
	SetIdentity(Sxyz);
	SetIdentity(ComT);
	SetIdentity(r);

	matTxyz(Txyz,0,0,0);
	matSxyz(Sxyz,1,1,1);

	matRx(r,PI/6);//��x����ת30��
	MatMul(Rxyz,r,Rxyz);
	matRy(r,PI/6);//��y����ת30��
	MatMul(Rxyz,r,Rxyz);
	matRz(r,PI/6);//��z����ת30��
	MatMul(Rxyz,r,Rxyz);

	//�󸴺ϱ任����
	MatMul(ComT,Rxyz,ComT);
	MatMul(ComT,Sxyz,ComT);
	MatMul(ComT,Txyz,ComT);

	for(i=0;i<8;i++)
	{
		g_pvertex[i][0]=g_vertex[i][0];
		g_pvertex[i][1]=g_vertex[i][1];
		g_pvertex[i][2]=g_vertex[i][2];
	}
	VerTransfer(g_pvertex,g_vertex,8,ComT);

}

//���ش���
inline void SetPixel(int x,int y)
{
	if(0<=x&&x<defSCREEN_WIDTH&&0<=y&&y<defSCREEN_HEIGHT){
		g_abyScreenImage[y][x][0]=g_color.r;
		g_abyScreenImage[y][x][1]=g_color.g;
		g_abyScreenImage[y][x][2]=g_color.b;
	}
}
inline void SetColor(GLubyte r,GLubyte g,GLubyte b)
{
	g_color.r=r;
	g_color.g=g;
	g_color.b=b;
}

//����
void Draw(float v[][3])
{
	int i;
	static int halWidth=defSCREEN_WIDTH/2,halHeight=defSCREEN_HEIGHT/2;
	static int index[12][2]={{0,1},{1,2},{2,3},{3,0},   {4,5},{5,6},{6,7},{7,4},    {1,4},{2,5},    {3,6},{0,7}};
	static int sx,sy,ex,ey;

	SetColor(100,100,100);
	//x��
	DDA(20,halHeight,defSCREEN_WIDTH-20,halHeight,SetPixel);
	DDA(defSCREEN_WIDTH-20,halHeight,defSCREEN_WIDTH-40,halHeight+20,SetPixel);
	DDA(defSCREEN_WIDTH-20,halHeight,defSCREEN_WIDTH-40,halHeight-20,SetPixel);

	//y��
	DDA(halWidth,20,halWidth,defSCREEN_HEIGHT-20,SetPixel);
	DDA(halWidth,defSCREEN_HEIGHT-20,halWidth-20,defSCREEN_HEIGHT-40,SetPixel);
	DDA(halWidth,defSCREEN_HEIGHT-20,halWidth+20,defSCREEN_HEIGHT-40,SetPixel);

	//��
	DDA(halWidth,halHeight,halWidth+300,halHeight+300,SetPixel);

	//������
	for(i=0;i<12;i++)
	{
		if(i<4)SetColor(255,0,0);
		else SetColor(255,255,255);
		sx=ROUND(g_pvertex[index[i][0]][0])+halWidth;
		sy=ROUND(g_pvertex[index[i][0]][1])+halHeight;
		ex=ROUND(g_pvertex[index[i][1]][0])+halWidth;
		ey=ROUND(g_pvertex[index[i][1]][1])+halHeight;
		DDA(sx,sy,ex,ey,SetPixel);
	}

}

//��ʼ����Ļ��������Ϊȫ��
void InitScreenImage()
{
	int iHeight,iWidth;
	int iColorRed,iColorGreen,iColorBlue;
	for(iHeight=0;iHeight<defSCREEN_HEIGHT;iHeight++){
		for(iWidth=0;iWidth<defSCREEN_WIDTH;iWidth++){
			iColorRed=0;
			iColorGreen=0;
			iColorBlue=0;
			g_abyScreenImage[iHeight][iWidth][0]=(GLubyte)iColorRed;
			g_abyScreenImage[iHeight][iWidth][1]=(GLubyte)iColorGreen;
			g_abyScreenImage[iHeight][iWidth][2]=(GLubyte)iColorBlue;
		}
	}
}

//�ı���Ļ��������
void ModifyScreenImage()
{
	Draw(g_pvertex);
}

//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	static float t[4][4];
	SetIdentity(t);
	if(uchKey == 27) {
		exit(0);
	}
	switch(uchKey){
		//ƽ��
		case 'd':
		case 'D'://x����
			matTxyz(t,4,0,0);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'a':
		case 'A'://x��С
			matTxyz(t,-4,0,0);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'w':
		case 'W'://y����
			matTxyz(t,0,4,0);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 's':
		case 'S'://y��С
			matTxyz(t,0,-4,0);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'q':
		case 'Q'://z����
			matTxyz(t,0,0,4);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'e':
		case 'E'://z��С
			matTxyz(t,0,0,-4);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;

		//����
		case 'z':
		case 'Z'://�Ŵ�
			matSxyz(t,1.05,1.05,1.05);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'x':
		case 'X'://��С
			matSxyz(t,0.95,0.95,0.95);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;

		//��ת
		case 'l':
		case 'L'://��x��
			matRx(t,0.1);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'i':
		case 'I'://��y��
			matRy(t,0.1);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'j':
		case 'J'://��z��
			matRz(t,0.1);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
		case 'k':
		case 'K'://��ָ����
			matR(t,0.1,1,1,0);
			MatMul(ComT,t,ComT);
			VerTransfer(g_pvertex,g_pvertex,8,t);
			break;
	}
	glutPostRedisplay();
}

//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos)
{
	
}

//��Ӧ��갴��
void Mouse(int iButton,int iState,int iXPos, int iYPos)
{
	
}

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos)
{
	
}

void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH,defSCREEN_HEIGHT);
}

//��ʼ��
void Init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	InitScreenImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

//���ƻص�����
void Display() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	InitScreenImage();
	ModifyScreenImage();
	glDrawPixels(defSCREEN_WIDTH,defSCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,g_abyScreenImage);
	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	MyInit();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("����任: ƽ��(w,a,s,d,q,e)  ��ת(l��x��,i��y��,j��z��,k��ָ����)  ����(z,x)");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
	return 0;
}

