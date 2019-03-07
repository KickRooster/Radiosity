#include "Button.h"
#include "..\Math\Vector3.h"

namespace Core
{
	Button::Button()
		:m_pStaticMesh(std::make_unique<StaticMesh>()),
		m_pRenderableUnit(std::make_unique<GLRenderableUnit>())
	{
		m_pStaticMesh->pPositions = new Vector4[4];
		m_pStaticMesh->pUV0s = new Vector2[4];
		m_pStaticMesh->pColors = new Vector4[4];
		m_pStaticMesh->vertexCount = 4;

		m_pStaticMesh->pPositions[0].x = -1.0f;
		m_pStaticMesh->pPositions[0].y = -1.0f;
		m_pStaticMesh->pPositions[0].z = 0.9f;
		m_pStaticMesh->pUV0s[0].x = 0;
		m_pStaticMesh->pUV0s[0].y = 0;
		m_pStaticMesh->pColors[0].r = 1.0f;
		m_pStaticMesh->pColors[0].g = 0;
		m_pStaticMesh->pColors[0].b = 0;

		m_pStaticMesh->pPositions[1].x = 1.0f;
		m_pStaticMesh->pPositions[1].y = -1.0f;
		m_pStaticMesh->pPositions[1].z = 0.9f;
		m_pStaticMesh->pUV0s[1].x = 1.0f;
		m_pStaticMesh->pUV0s[1].y = 0;
		m_pStaticMesh->pColors[1].r = 0;
		m_pStaticMesh->pColors[1].g = 1.0f;
		m_pStaticMesh->pColors[1].b = 0;

		m_pStaticMesh->pPositions[2].x = -1.0f;
		m_pStaticMesh->pPositions[2].y = 1.0f;
		m_pStaticMesh->pPositions[2].z = 0.9f;
		m_pStaticMesh->pUV0s[2].x = 0;
		m_pStaticMesh->pUV0s[2].y = 1.0f;
		m_pStaticMesh->pColors[2].r = 0;
		m_pStaticMesh->pColors[2].g = 0;
		m_pStaticMesh->pColors[2].b = 1.0f;

		m_pStaticMesh->pPositions[3].x = 1.0f;
		m_pStaticMesh->pPositions[3].y = 1.0f;
		m_pStaticMesh->pPositions[3].z = 0.9f;
		m_pStaticMesh->pUV0s[3].x = 1.0f;
		m_pStaticMesh->pUV0s[3].y = 1.0f;
		m_pStaticMesh->pColors[3].r = 0;
		m_pStaticMesh->pColors[3].g = 0.5f;
		m_pStaticMesh->pColors[3].b = 1.0f;

		m_pStaticMesh->pIndices = new uint32[6];

		m_pStaticMesh->indexCount = 6;

		m_pStaticMesh->pIndices[0] = 0;
		m_pStaticMesh->pIndices[1] = 1;
		m_pStaticMesh->pIndices[2] = 2;
		m_pStaticMesh->pIndices[3] = 1;
		m_pStaticMesh->pIndices[4] = 3;
		m_pStaticMesh->pIndices[5] = 2;

		//m_pStaticMesh->UploadToGPU();
	}

	void Button::SetParent(IWidget * const pParent)
	{
		m_pParent.reset(pParent);
	}

	void Button::Update()
	{

	}

	void Button::Draw()
	{
		m_pStaticMesh->Activate();
		m_pStaticMesh->Inactivate();
	}

	Button::~Button()
	{

	}
}