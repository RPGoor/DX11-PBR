#pragma once

#include "Vertex.h"
#include <string>
#include <vector>

struct MeshData
{
    std::string tag;
    std::vector<Vertex::Standard> vertices;
    std::vector<unsigned int> indices;

    MeshData(std::string tag, std::vector<Vertex::Standard> vertices, std::vector<unsigned int> indices)
        : tag(std::move(tag)),
          vertices(std::move(vertices)),
          indices(std::move(indices))
    {
    }
};
