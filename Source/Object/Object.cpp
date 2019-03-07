#include "Object.h"
#include "..\Helper\MeshImportor.h"
#include "..\Math\MathGlobal.h"
#include "..\Helper\TextureOperator.h"
#include "..\3rdParty\DearUI\ImGuizmo.h"

namespace Core
{
	int32 Object::idSeed = 1;

	Object::Object()
		:
		m_object2WorldMatrix(Matrix4x4Identify),
		m_world2ObjectMatrix(Matrix4x4Identify),
		m_object2WorldITMatrix(Matrix4x4Identify),
		position(Zero),
		eulerAngle(Zero),
		scale(Vector3(1.0f, 1.0f, 1.0f)),
		stitched(False)
	{
		id = idSeed;
		++idSeed;
	}

	void Object::Initialize(OpenGLDevice * pDevice, RLDevice * pRLDevice)
	{
		float matrixTranslation[3];
		float matrixRotation[3];
		float matrixScale[3];

		matrixTranslation[0] = position.x; 
		matrixTranslation[1] = position.y;
		matrixTranslation[2] = position.z;

		matrixRotation[0] = eulerAngle.x;
		matrixRotation[1] = eulerAngle.y;
		matrixRotation[2] = eulerAngle.z;

		matrixScale[0] = scale.x;
		matrixScale[1] = scale.y;
		matrixScale[2] = scale.z;

		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, (float*)(&m_object2WorldMatrix));

		m_object2WorldMatrix *= glRenderableUnit->staticMesh.lock()->GetLocal2World();

		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		
		m_world2ObjectMatrix = Inverse(m_object2WorldMatrix);
		m_object2WorldITMatrix = Transpose(m_world2ObjectMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);

		pDevice->UploadGlobalShaderData(GLShaderDataAlias_LightmapUVParam, sizeof(Vector4Dummy), &glRenderableUnit->material.lock()->lightmapUVParam);

		glRenderableUnit->BeginUse();

		rlRenderableUnit->Commit(id);
	}

	void Object::Tick(float deltaTime, OpenGLDevice * pDevice, RLDevice * pRLDevice)
	{
		m_world2ObjectMatrix = Inverse(m_object2WorldMatrix);
		m_object2WorldITMatrix = Transpose(m_world2ObjectMatrix);

		float matrixTranslation[3];
		float matrixRotation[3];
		float matrixScale[3];

		ImGuizmo::DecomposeMatrixToComponents((float*)(&m_object2WorldMatrix), matrixTranslation, matrixRotation, matrixScale);

		position.x = matrixTranslation[0];
		position.y = matrixTranslation[1];
		position.z = matrixTranslation[2];

		eulerAngle.x = matrixRotation[0];
		eulerAngle.y = matrixRotation[1];
		eulerAngle.z = matrixRotation[2];

		scale.x = matrixScale[0];
		scale.y = matrixScale[1];
		scale.z = matrixScale[2];
	}

	void Object::Render(OpenGLDevice * pDevice)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_LightmapUVParam, sizeof(Vector4Dummy), &glRenderableUnit->material.lock()->lightmapUVParam);

		glRenderableUnit->Activate();
		glRenderableUnit->material.lock()->SetAlbedoColor();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh.lock()->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->Inactivate();
	}
	
	void Object::RenderAsPostprocess(OpenGLDevice * pDevice, Vector4 & postprocessParam)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_PostprocessParam, sizeof(Vector4Dummy), &postprocessParam);

		glRenderableUnit->Activate();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh.lock()->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->Inactivate();
	}

	void Object::Bake(OpenGLDevice * pDevice)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);

		glRenderableUnit->ActivateBaking();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh.lock()->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->InactivateBaking();
	}

	Matrix4x4 * Object::GetObject2WorldMatrix()
	{
		return &m_object2WorldMatrix;
	}

	Core::Vector3 Object::GetZDirection()
	{
		return GetZAxis(m_object2WorldMatrix);
	}

	Core::Vector3 Object::GetYDirection()
	{
		return GetYAxis(m_object2WorldMatrix);
	}

	float Object::GetXScale() const
	{
		Vector3 xAxis = Vector3(m_object2WorldMatrix[0][0], m_object2WorldMatrix[0][1], m_object2WorldMatrix[0][2]);
		
		return Length(xAxis);
	}

	float Object::GetYScale() const
	{
		Vector3 yAxis = Vector3(m_object2WorldMatrix[1][0], m_object2WorldMatrix[1][1], m_object2WorldMatrix[1][2]);

		return Length(yAxis);
	}

	float Object::GetZScale() const
	{
		Vector3 zAxis = Vector3(m_object2WorldMatrix[2][0], m_object2WorldMatrix[2][1], m_object2WorldMatrix[2][2]);

		return Length(zAxis);
	}

	Object::~Object()
	{

	}
}