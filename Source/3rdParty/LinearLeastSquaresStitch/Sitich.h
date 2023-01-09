#pragma once
#include <memory>
#include <assert.h>
#include <pplwin.h>
#include <ppl.h>
#include <vector>
#include <unordered_map>

#include "..\..\Asset\Texture.h"
#include "..\..\Asset\StaticMesh.h"
#include "..\..\Math\Vector4.h"

namespace llss
{
	void Stitch(std::shared_ptr<Core::StaticMesh> mesh, Core::int32 Width, Core::int32 Height, float* lightmapRawData, float* maskMapRawData);
}