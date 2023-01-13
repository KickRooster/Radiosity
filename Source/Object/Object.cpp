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
		IsLight(False),
		Color{1.0f, 1.0f, 1.0f},
		Intensity(1.0),
		Energy{1.0f, 1.0f, 1.0f},
		LightmapResolution(LightmapResolution_Invalid),
		ResolutionString(Null),
		LightPrecision(LightPrecision_Invalid),
		PrecisionString(Null),
		XLength(1),
		YLength(1),
		MaxXLength(1000),
		MaxYLength(1000)
	{
		id = idSeed;
		++idSeed;
	}

	void Object::Initialize(OpenGLDevice * pDevice,  Bool SerializedRTSAsLocal2World)
	{
		if (SerializedRTSAsLocal2World)
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
		}
		else
		{
			m_object2WorldMatrix = glRenderableUnit->staticMesh->GetLocal2World();
		}
		
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		
		m_world2ObjectMatrix = Inverse(m_object2WorldMatrix);
		m_object2WorldITMatrix = Transpose(m_world2ObjectMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);
		
		glRenderableUnit->BeginUse();

		if (LightmapResolution != LightmapResolution_Invalid)
		{
			ResolutionString = LightmapResolutionItems[LightmapResolution];
		}

		if (LightPrecision != LightPrecision_Invalid)
		{
			PrecisionString = LightPrecisionItems[LightPrecision];
		}
	}

	void Object::UpdateTransformMatrix()
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

		m_world2ObjectMatrix = Inverse(m_object2WorldMatrix);
		m_object2WorldITMatrix = Transpose(m_world2ObjectMatrix);
	}
	
	void Object::Tick(float deltaTime, OpenGLDevice * pDevice)
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
		
		glRenderableUnit->Activate();
		glRenderableUnit->material.lock()->albedoColor[0] = Color[0];
		glRenderableUnit->material.lock()->albedoColor[1] = Color[1];
		glRenderableUnit->material.lock()->albedoColor[2] = Color[2];
		glRenderableUnit->material.lock()->SetAlbedoColor();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh->indexCount,
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
			glRenderableUnit->staticMesh->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->Inactivate();
	}

	void Object::BeforeBaking()
	{
		if (IsLight)
		{
			glRenderableUnit->staticMesh->BeforeBaking(m_object2WorldMatrix, LightmapResolution_Invalid);
		}
		else
		{
			glRenderableUnit->staticMesh->BeforeBaking(m_object2WorldMatrix, LightmapResolution);
		}
		
		if (rlRenderableUnit)
		{
			rlRenderableUnit->Commit();
		}
	}

	void Object::UpdateRLMatrix(RLDevice * pDevice)
	{
		if (rlRenderableUnit)
		{
			rlRenderableUnit->Activate();
			pDevice->SetTransformMatrix(rlRenderableUnit->GetProgram(), &m_object2WorldMatrix);
			rlRenderableUnit->Inactivate();
		}
	}

	void Object::DrawGBuffer(OpenGLDevice * pDevice)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);

		glRenderableUnit->ActiveDrawGBuffer();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->InactiveDrawGBuffer();
	}
	
	void Object::DrawID(OpenGLDevice * pDevice)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);

		glRenderableUnit->ActivateDrawingID();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->InactivateDrawingID();
	}
	
	void Object::ComputeFormFactor(OpenGLDevice* pDevice)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);

		glRenderableUnit->ActiveComputingFormFactor();
		pDevice->DrawElements(
			GLTopology_Triangles,
			glRenderableUnit->staticMesh->indexCount,
			GLDataType_UnsignedInt,
			Null);
		glRenderableUnit->InactiveComputingFormFactor();
	}

	void Object::BeforeComputeFormFactor(OpenGLDevice* pDevice)
	{
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatrices, sizeof(m_object2WorldMatrix), &m_object2WorldMatrix);
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_ObjectMatricesIT, sizeof(m_object2WorldITMatrix), &m_object2WorldITMatrix);

		glRenderableUnit->ActiveComputingFormFactor();
	}

	void Object::DrawPrimitive(OpenGLDevice* pDevice, int32 StartPrimitive, int32 PrimitiveCount)
	{
		pDevice->DrawArrays(GLTopology_Triangles, StartPrimitive * 3, PrimitiveCount * 3);
	}
	
	void Object::AfterComputeFormFactor()
	{
		glRenderableUnit->InactiveComputingFormFactor();
	}
	
	void Object::SetShootingPrimitive(float PrimitiveID)
	{
		rlRenderableUnit->Activate();
		rlRenderableUnit->SetShootingPrimitive(PrimitiveID);
		rlRenderableUnit->Inactivate();
	}
	
	Matrix4x4 * Object::GetObject2WorldMatrix()
	{
		return &m_object2WorldMatrix;
	}

	Vector3 Object::GetZDirection()
	{
		return GetZAxis(m_object2WorldMatrix);
	}

	Vector3 Object::GetYDirection()
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