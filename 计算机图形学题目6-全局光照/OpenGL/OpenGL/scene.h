
#ifndef __SCENE_H__
#define __SCENE_H__

#include "raytracer/world.h"
#include "raytracer/ray.h"
#include "raytracer/colour.h"
#include "raytracer/Material/material.h"
#include "raytracer/Geometry/rayplane.h"
#include "raytracer//Geometry/raysphere.h"
#include "raytracer/object.h"
#include "raytracer/light.h"

/*=====================================================================
������
=====================================================================*/
class Scene
{
public:
	
	//���캯����width��ȣ�height�߶�
	Scene(int width,int height);
	
	~Scene(void);

	
	//���������λ��
	void setCamPos(float x,float y,float z){m_campos=Vec3(x,y,z);}

	//���ø��ٴ���
	void setTraceNum(int traceNum){m_world->setTraceNum(traceNum);}
	
	//��������
	void insertObject(Object* ob);
	
	//�����Դ
	void insertLight(Light* light);
	
	//�������͹�Դ
	void clear(){m_world->clear();}

	
	//���Ƴ���
	void draw();
	
	//�����ͷŲ��õ��ڴ�ռ�
	void shutdown();

	
	unsigned char***m_screenBuffer;//��������

private:

	//���ؾ�������Ϊ��x��y�������ص����߷���(��λ��)
	Vec3 getUnitDirForImageCoords(int x, int y);

	//������Ϊ��x��y����������ɫ����Ϊcolour
	void drawPixel(int x,int y, Colour colour);

	
	int m_iWidth,m_iHeight;//�����������
	
	World*	m_world;//m_worldʵ�ֹ��߸���
	
	Vec3 m_campos;//�����λ��

};
#endif
