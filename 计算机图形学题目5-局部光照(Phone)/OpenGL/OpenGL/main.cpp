#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "raytracer/colour.h"
#include "raytracer/Material/material.h"
#include "raytracer/Geometry/rayplane.h"
#include "raytracer/Geometry/raysphere.h"
#include "raytracer/object.h"
#include "raytracer/light.h"
#include "scene.h"

#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700		

float lx = 50;
float ly = 50;
float lz = 50;

//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

Scene s(defSCREEN_WIDTH,defSCREEN_HEIGHT);

void createScene(){
	//�������͹�Դ
	s.clear();
	s.setCamPos(0,3,40);
	Object*sphere;
	Light*light;
	float planedist=-10;

	//����
	{

		Material mat(Colour(0.0f,0.0f,0.0f),Colour(0.6f,0.6f,0.6f),Colour(0.6f,0.6f,0.6f),100.0f);
			
		RaySphere* geom = new RaySphere(Vec3(0,planedist+9,-35), 3);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}

	//����
	{

		Material mat(Colour(0.0f,0.0f,0.0f),Colour(0.0f,0.0f,0.6f),Colour(0.5f,0.5f,0.6f),100.0f);
			
		RaySphere* geom = new RaySphere(Vec3(-7,planedist+14,-30), 4);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}
	
	//����
	{

		Material mat(Colour(0.0f,0.0f,0.0f),Colour(0.6f,0.0f,0.0f),Colour(0.6f,0.5,0.5f),100.0f);
			
		RaySphere* geom = new RaySphere(Vec3(10,planedist+12,-40),4);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}
	
	//����
	{

		Material mat(Colour(0.0f,0.0f,0.0f),Colour(0.0f,0.6f,0.0f),Colour(0.5f,0.6f,0.5f),100.0f);
			
		RaySphere* geom = new RaySphere(Vec3(5,planedist+7,-32), 2);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}

	//��Դ
	light = new Light(Vec3(lx,ly,lz), Colour(1, 1, 1) );
	s.insertLight(light);
}


void myInit(){
	s.setTraceNum(4);
	createScene();
	s.draw();
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
	for(int i=0;i<defSCREEN_HEIGHT;i++){
		for(int j=0;j<defSCREEN_WIDTH;j++){
			g_abyScreenImage[i][j][0]=s.m_screenBuffer[i][j][0];
			g_abyScreenImage[i][j][1]=s.m_screenBuffer[i][j][1];
			g_abyScreenImage[i][j][2]=s.m_screenBuffer[i][j][2];
		}
	}
}

//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if(uchKey == 27) {
		exit(0);
	}
	else if(uchKey=='a'||uchKey=='A'){
		s.shutdown();
		lx=-50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
	else if(uchKey=='d'||uchKey=='D'){
		s.shutdown();
		lx=50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
	else if (uchKey == 'w' || uchKey == 'W') {
		s.shutdown();
		ly = 50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
	else if (uchKey == 's' || uchKey == 'S') {
		s.shutdown();
		ly = -50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
	else if (uchKey == 'q' || uchKey == 'Q') {
		s.shutdown();
		lz = 50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
	else if (uchKey == 'e' || uchKey == 'E') {
		s.shutdown();
		lz = -50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
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
	createScene();
}

//���ƻص�����
void Display() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	//InitScreenImage();
	ModifyScreenImage();
	glDrawPixels(defSCREEN_WIDTH,defSCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,g_abyScreenImage);
	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	myInit();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("Phone���գ�a,d���ƹ�Դ����λ�ã�w,s���ƹ�Դ����λ�ã�q,e���ƹ�Դǰ��λ�ã�");
	Init();
	glutDisplayFunc(Display);
	//glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
	return 0;
}

