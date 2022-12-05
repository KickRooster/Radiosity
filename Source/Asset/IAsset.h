#pragma once
#include <BaseInclude.h>
#include <../3rdParty/cereal/archives/xml.hpp>
//	������Դ��Ļ���.
//	��Դ��Ļ���Ĵ���,��Ҫ��Ϊ�˷���ͳ����Դ�ڴ�ռ��.
//	���Կ����ṩ�ڴ�����,Ҳ������Դ.�������ܹ��������еĶ��ڴ�.
//	���еĻ����������ù�ϵ,ͬʱ�����һ�������ԭ���ڴ����,ʹ���滻��
//	malloc�ķ�����ʵ��.

#include STRING_INCLUDE_PATH

namespace Core
{
	enum FileType
	{
		FileType_Scene,
		FileType_Prefab,
		FileType_GLSL_Vertex,
		FileType_GLSL_Fragment,
		FileType_TextureInfo,
		FileType_Texture,
		FileType_Material,
		FileType_StaticMesh,
		FileType_FBX,
	};

	class IAsset
	{
	protected:
		Bool m_beingUsed;

	public:
		ctd::string fullPathName;
		ctd::string fileName;
		ctd::string fileNameWithExt;		//	�ļ���,����չ��
		FileType type;

		IAsset() : m_beingUsed(False) {}

		Bool BeingUsed()
		{
			return m_beingUsed;
		}

		virtual void BeginUse() = 0;
		virtual void Reload() = 0;
		virtual void BeforeSave() = 0;
		virtual void AfterLoad() = 0;
		virtual ~IAsset() {}
	};
}