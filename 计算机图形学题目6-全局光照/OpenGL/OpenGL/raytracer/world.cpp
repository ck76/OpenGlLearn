

#include "world.h"


#include "object.h"
#include "colour.h"
#include "ray.h"
#include "light.h"
#include "Geometry/geometry.h"
#include <assert.h>
#include <iostream>
using namespace std;

const Colour ambient_lighting(0.3f, 0.3f, 0.3f);

World::World()
{
	m_traceNum=5;
}

World::~World()
{
	for(unsigned int i=0; i<m_objects.size(); ++i)
	{
		delete m_objects[i];
	}

	for(unsigned int z=0; z<m_lights.size(); ++z)
	{
		delete m_lights[z];
	}
}


void World::insertObject(Object* ob)
{
	m_objects.push_back(ob);
}



void World::insertLight(Light* light)
{
	m_lights.push_back(light);
}



//������ray�ཻ�������Object,����ray����뽻��ľ������dist����û��Object��ray�ཻ������NULL,distΪ����
Object* World::closestObject(const Ray& ray,float& dist)
{
	Object* obj=NULL;
	float smallest_dist = 1e9f; 

	for(unsigned int i=0; i<m_objects.size(); i++)
	{
		const float tracedist = m_objects[i]->getGeometry().getDistanceUntilHit(ray);
	
		if(tracedist >= 0.0f && tracedist < smallest_dist)
		{
			smallest_dist = tracedist;
			obj = m_objects[i];
		}
	}
	if(obj==NULL)dist=-666;
	else dist=smallest_dist* 0.999f;
	return obj;
}


void World::getColourForRay(const Ray& ray, Colour& colour_out,int traceNum)
{

	if (traceNum != 0)
	{
		
		Vec3 Ldir;
		float dist, bf = 0.5;
		//���������ray�ཻ����������壬������������dist��
		Object* obj = closestObject(ray, dist); 

		if (obj)
		{
			

			// ����ཻ�������
			Vec3 hitpos = ray.m_startPos;
			hitpos.addMult(ray.m_unitDir, dist);
			// �ཻ�㷨����
			Vec3 normal = obj->getGeometry().getNormalForPos(hitpos);
			normal = normal.normalise();
			//���������� 
			
			Vec3 R = ray.m_unitDir - 2 * ray.m_unitDir.dot(normal) * normal;

			//R = reflect(ray.m_unitDir, normal);
			Colour diffuse = Colour::black(), specular = Colour::black();
			//���ÿ����Դ�Խ��㴦���յĹ��� 
			for (int i = 0; i < m_lights.size(); i++)
			{
				//�����Ӱ���� 
				Vec3 L = m_lights[i]->getPos() - hitpos;
				L = L.normalise();
				//�����Ӱ����·�����Ƿ�����ڵ��� 
				if (!closestObject(Ray(hitpos, L), dist))
				{       
					//�������ڵ������ۼ��ϸù�Դ�Խ����ɢ��;����Ĺ��� 
					Colour lightcolour = m_lights[i]->getColour();
					Vec3 H = (L - ray.m_unitDir) / 2;
					H = H.normalise();
					diffuse += lightcolour * (L.dot(normal) > 0 ? L.dot(normal) : 0);
					specular += lightcolour * pow((H.dot(normal) > 0 ? H.dot(normal) : 0), obj->getMaterial().ns);
				}
			}
			Colour total1, total2;
			total1 = ambient_lighting * obj->getMaterial().ka + diffuse * obj->getMaterial().kd + specular * obj->getMaterial().ks;
			getColourForRay(Ray(hitpos, R), total2, traceNum - 1);//�ݹ�����¸�����Ĺ���
			colour_out = total1 + total2 * bf;//�ۼ� 
			return;
		}
	}
	colour_out = Colour::black();//����������غ�ɫ 
}

void World::setTraceNum(int traceNum)
{
	m_traceNum=traceNum;
}

void World::clear()
{
	int i=m_objects.size();
	for(int j=0;j<i;j++)
		delete m_objects[j];
	m_objects.clear();

	i=m_lights.size();
	for(int j=0;j<i;j++)
		delete m_lights[j];
	m_lights.clear();
}
