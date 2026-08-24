#pragma once

#include "Vertex.h"
#include <string>

struct MeshData
{
    std::string tag;
    Dvtx::VertexBuffer vertices;
    std::vector<unsigned int> indices;

    MeshData(std::string tag, Dvtx::VertexBuffer vertices, std::vector<unsigned int> indices)
        : tag(std::move(tag)), vertices(std::move(vertices)), indices(std::move(indices))
    {}
};