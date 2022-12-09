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
	}

	void Scene::BeginUse()
	{
	}
	
	void Scene::Reload()
	{
	}
	
	void Scene::BeforeSave()
	{
		for (vector<std::shared_ptr<Object>>::iterator iter = serializedObjects.begin();
			iter != serializedObjects.end();
			++iter)
		{
			iter->get()->BeforeSave();
		}
	}
	
	void Scene::AfterLoad()
	{
		for (vector<std::shared_ptr<Object>>::iterator iter = serializedObjects.begin();
			iter != serializedObjects.end();
			++iter)
		{
			iter->get()->AfterLoad();
		}
	}

	void Scene::Initialize(OpenGLDevice * pDevice, int32 width, int32 height)
	{
		m_camera->zNear = 1.0f;
		m_camera->zFar = 6000.0f;
		m_camera->ascept = width / (float)height;
		m_camera->fovY = 120.0f * Deg2Rad;

		m_camera->position = Vector3(0, 0, 50.0f);
		m_camera->lookAt = Vector3(0, 0, -1.0f);
		m_camera->eularAngle = Vector3(0, 0, 0);
		
		m_camera->UpdateMatrix();
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
	
	void Scene::Render(OpenGLDevice * pDevice, int32 width, int32 height)
	{
 		pDevice->BeforeRender(width, height);
		m_camera->UpdataGLParam(pDevice);

		//Vector3 lightDir = Normalize(m_dirLightRef.lock()->GetYDirection());
		//pDevice->UploadGlobalShaderData(GLShaderDataAlias_LightParam, sizeof(lightDir), &lightDir);

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

		m_areaLightRef = object; 

		if (needSerialization)
			serializedObjects.push_back(object);
	}

	Object* Scene::GetAreaLight() const
	{
		return m_areaLightRef.lock().get();
	}

	Object* Scene::GetBeingBakingObject() const
	{
		return m_beingBakingObjectRef.lock().get();	
	}
	
	Scene::~Scene()
	{

	}
}