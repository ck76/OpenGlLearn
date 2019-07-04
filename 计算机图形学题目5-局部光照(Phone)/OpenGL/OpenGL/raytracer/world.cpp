

#include "world.h"


#include "object.h"
#include "colour.h"
#include "ray.h"
#include "light.h"
#include "Geometry/geometry.h"
#include <assert.h>


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

//��ȡ����ray����ɫ������colour_out��������traceNum��
void World::getColourForRay(const Ray& ray, Colour& colour_out,int traceNum)
{

	if(traceNum<1){
		colour_out=Colour::black();
		return;
	}

	float minDistance;
	Object* obj = closestObject(ray, minDistance);

	if (obj != NULL && minDistance >= 0)
	{
		Material material = obj->getMaterial();
		float reflectiveness = 0.25f;
		Vec3 pos = ray.m_startPos + ray.m_unitDir * minDistance;
		Vec3 normal = obj->getGeometry().getNormalForPos(pos);
		colour_out = material.sample(ray, pos, normal, m_lights[0]);
		return;
	}
	colour_out = Colour::black();
	return;

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
