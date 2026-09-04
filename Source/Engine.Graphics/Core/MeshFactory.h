#pragma once
#include "MeshData.h"
#include <string>

namespace MeshFactory
{
    MeshData Load(const std::string& path);

    MeshData Grid(UINT64 resolution, float size);
} // namespace MeshFactory
