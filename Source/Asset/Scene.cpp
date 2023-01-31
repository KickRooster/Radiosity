#include "Scene.h"
#include "..\Helper\Helper.h"
#include <algorithm>

using namespace std;
using namespace ctd;

namespace Core
{
	Scene::Scene()
		:m_camera(std::make_unique<Camera>())
	{
		//	Camera default parameters.
		m_camera->zNear = 1.0;
		m_camera->zFar =  6000.0f;
		//	Mark it created just now.
		m_camera->ascept = 0.0;
		m_camera->fovY = 90.0f * Deg2Rad;

		m_camera->position = Vector3(0, 0, 0);
		m_camera->lookAtDir = Vector3(0, 0, -1.0f);
		m_camera->eularAngle = Vector3(0, 0, 0);
	}

	void Scene::BeginUse()
	{
	}
	
	void Scene::Reload()
	{
	}
	
	void Scene::BeforeSave()
	{
		m_camera_z_near = m_camera->zNear;
		m_camera_z_far = m_camera->zFar;
		m_camera_fov = m_camera->fovY;
		m_camera_position_x = m_camera->position.x;
		m_camera_position_y = m_camera->position.y;
		m_camera_position_z = m_camera->position.z;
		m_camera_look_at_x = m_camera->lookAtDir.x;
		m_camera_look_at_y = m_camera->lookAtDir.y;
		m_camera_look_at_z = m_camera->lookAtDir.z;
		m_camera_eulerAngle_x = m_camera->eularAngle.x;
		m_camera_eulerAngle_y = m_camera->eularAngle.y;
		m_camera_eulerAngle_z = m_camera->eularAngle.z;
		
		for (vector<std::shared_ptr<Object>>::iterator iter = serializedObjects.begin();
			iter != serializedObjects.end();
			++iter)
		{
			iter->get()->BeforeSave();
		}
	}
	
	void Scene::AfterLoad()
	{
		m_camera->zNear = m_camera_z_near;
		m_camera->zFar = m_camera_z_far ;
		m_camera->fovY = m_camera_fov ;
		m_camera->position.x = m_camera_position_x;
		m_camera->position.y = m_camera_position_y;
		m_camera->position.z = m_camera_position_z;
		m_camera->lookAtDir.x = m_camera_look_at_x;
		m_camera->lookAtDir.y = m_camera_look_at_y;
		m_camera->lookAtDir.z = m_camera_look_at_z;
		m_camera->eularAngle.x = m_camera_eulerAngle_x;
		m_camera->eularAngle.y = m_camera_eulerAngle_y;
		m_camera->eularAngle.z = m_camera_eulerAngle_z;
		
		for (vector<std::shared_ptr<Object>>::iterator iter = serializedObjects.begin();
			iter != serializedObjects.end();
			++iter)
		{
			iter->get()->AfterLoad();
		}
	}

	void Scene::Tick(float deltaTime, OpenGLDevice * pDevice, const InputState & inputState)
	{
		for (ctd::vector<std::shared_ptr<Object>> ::iterator iter = objects.begin();
			iter != objects.end();
			++iter)
		{
			iter->get()->Tick(deltaTime, pDevice);
		}

		m_camera->Tick(deltaTime, inputState);
	}
	
	void Scene::Render(OpenGLDevice * pDevice, int32 width, int32 height, Bool LightmapEncodingInRGBM)
	{
 		pDevice->BeforeRender(width, height);
		m_camera->LightmapEncodingInRGBM = LightmapEncodingInRGBM;
		m_camera->UpdataGLParam(pDevice);

		for (ctd::vector<std::shared_ptr<Object>> ::iterator iter = objects.begin();
			iter != objects.end();
			++iter)
		{
			iter->get()->Render(pDevice);
		}
		pDevice->AfterRender();
	}

	Camera * Scene::GetCamera()
	{
		return m_camera.get();
	}

	void Scene::AddObject(std::shared_ptr<Object> object, Bool needSerialization)
	{
		objects.push_back(object);

		m_beingBakingObjectRef = object;

		if (needSerialization)
			serializedObjects.push_back(object);
	}

	void Scene::AddLight(std::shared_ptr<Object> object, Bool needSerialization)
	{
		objects.push_back(object);
		m_LightRefs.push_back(object);

		if (needSerialization)
			serializedObjects.push_back(object);
	}

	void Scene::RemoveLight(Object* Light)
	{
		for (std::vector<std::weak_ptr<Object>>::iterator iter = m_LightRefs.begin();
			iter != m_LightRefs.end();
			++iter)
		{
			if (iter->lock().get() == Light)
			{
				m_LightRefs.erase(iter);
				break ;
			}
		}
		
		for (ctd::vector<std::shared_ptr<Object>> ::iterator iter = objects.begin();
					iter != objects.end();
					++iter)
		{
			if (iter->get() == Light)
			{
				objects.erase(iter);
				break ;
			}
		}

		for (ctd::vector<std::shared_ptr<Object>> ::iterator iter = serializedObjects.begin();
					iter != serializedObjects.end();
					++iter)
		{
			if (iter->get() == Light)
			{
				serializedObjects.erase(iter);
				break ;
			}
		}
	}

	void Scene::Clear()
	{
		//	Destory RLPrimitive manually.
		for (ctd::vector<std::shared_ptr<Object>> ::iterator iter = objects.begin();
			iter != objects.end();
			++iter)
		{
			if ((*iter)->rlRenderableUnit)
			{
				(*iter)->rlRenderableUnit->material.lock()->rlVertexShader.lock()->Detach((*iter)->rlRenderableUnit.get());
				(*iter)->rlRenderableUnit->material.lock()->rlRayShader.lock()->Detach((*iter)->rlRenderableUnit.get());
				(*iter)->rlRenderableUnit = Null;
			}
		}
		
		objects.clear();
		m_LightRefs.clear();
	}

	int32 Scene::GetLightCount() const
	{
		return m_LightRefs.size();
	}

	int32 Scene::GetMaxLightIndex() const
	{
		int32 MaxIndex = -1;
		
		for (std::vector<std::weak_ptr<Object>>::const_iterator iter = m_LightRefs.begin();
			iter != m_LightRefs.end();
			++iter)
		{
			size_t Offset = iter->lock()->name.find_last_of('t');
			size_t NameLength = iter->lock()->name.length();
			string IndexStr = iter->lock()->name.substr(Offset + 1, NameLength - Offset);
			int32 Index = std::stoi(IndexStr);

			if (Index > MaxIndex)
			{
				MaxIndex = Index;
			}
		}

		return MaxIndex;
	}
	
	Object* Scene::GetAreaLight(int32 Index) const
	{
		return m_LightRefs[Index].lock().get();
	}

	Object* Scene::GetBeingBakingObject() const
	{
		return m_beingBakingObjectRef.lock().get();	
	}
	
	Scene::~Scene()
	{

	}
}