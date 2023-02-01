#pragma once
#include <BaseInclude.h>

#include <string>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>     
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include "../Asset/StaticMesh.h"

namespace Core
{
	class MeshImportor
	{
	private:
	public:
		MeshImportor();
		static ErrorCode LoadModel(const ANSICHAR * fileFullPathName, StaticMesh * pOutStaticMesh);
		~MeshImportor();
	};
}