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
	void Stitch(std::shared_ptr<Core::StaticMesh> mesh, std::shared_ptr<Core::Texture> lightmap, std::shared_ptr<Core::Texture> maskMap, Core::Vector4 lightmapPram, Core::int32 objectID);
}