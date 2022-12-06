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
		m_camera->Initialize(pDevice, width, height);
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

	void Scene::BeginBaking(OpenGLDevice * pGLDevice, int32 width, int32 height)
	{
		m_objectCursor = 0;
		pGLDevice->BeforeRender(width, height, 0);

		m_camera->UpdataGLParam(pGLDevice);
	}

	Core::Bool Scene::BakingFinished() const
	{
		return m_objectCursor == objects.size();
	}

	Core::Object * Scene::PoolBakingObject() const
	{
		return objects[m_objectCursor].get();
	}

	void Scene::SkipOneObject()
	{
		++m_objectCursor;
	}

	Bool Scene::BakeOneObject(OpenGLDevice * pDevice)
	{
		objects[m_objectCursor]->Bake(pDevice);

		++m_objectCursor;

		return True;
	}

	void Scene::EndBaking(OpenGLDevice * pDevice)
	{
		pDevice->AfterRender();
	}
	
	Core::Camera * Scene::GetCamera()
	{
		return m_camera.get();
	}

	void Scene::AddObject(std::shared_ptr<Object> object, Bool needSerialization)
	{
		objects.push_back(object);

		if (needSerialization)
			serializedObjects.push_back(object);
	}

	void Scene::AddLight(std::shared_ptr<Object> object, Bool needSerialization)
	{
		objects.push_back(object);

		m_dirLightRef = object; 

		if (needSerialization)
			serializedObjects.push_back(object);
	}

	Scene::~Scene()
	{

	}
}