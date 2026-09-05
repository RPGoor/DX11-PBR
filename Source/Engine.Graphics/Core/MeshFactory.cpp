#include "MeshFactory.h"
#include "ModelException.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

MeshData MeshFactory::Load(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals
    );

    if (!scene || !scene->HasMeshes())
    {
        throw ModelException(__LINE__, __FILE__, importer.GetErrorString());
    }

    const aiMesh& mesh = *scene->mMeshes[0];

    std::vector<Vertex::Standard> vertices;

    for (unsigned int i = 0; i < mesh.mNumVertices; i++)
    {
        vertices.push_back(
            Vertex::Standard{
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
                *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
            }
        );
    }

    std::vector<unsigned int> indices;
    indices.reserve(mesh.mNumFaces * 3);

    for (unsigned int i = 0; i < mesh.mNumFaces; i++)
    {
        const aiFace& face = mesh.mFaces[i];

        assert(face.mNumIndices == 3);

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
    return MeshData{std::string("load#") + mesh.mName.C_Str(), std::move(vertices), std::move(indices)};
}

MeshData MeshFactory::Grid(UINT resolution, float size)
{
    std::vector<Vertex::Standard> vertices;
    vertices.reserve(resolution * resolution);

    float spacing = size / (resolution - 1u);
    float halfSize = size * 0.5f;
    for (UINT z = 0; z < resolution; z++)
    {
        for (UINT x = 0; x < resolution; x++)
        {
            vertices.push_back(Vertex::Standard{{-halfSize + x * spacing, 0.0f, -halfSize + z * spacing}});
        }
    }

    std::vector<unsigned int> indices;
    indices.reserve((resolution - 1) * (resolution - 1) * 6);

    for (UINT z = 0; z < resolution - 1; z++)
    {
        for (UINT x = 0; x < resolution - 1; x++)
        {
            UINT topLeft = z * resolution + x;
            UINT topRight = topLeft + 1;
            UINT bottomLeft = (z + 1) * resolution + x;
            UINT bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    return MeshData{
        std::string("generateGrid#") + std::to_string(resolution) + "#" + std::to_string(size),
        std::move(vertices),
        std::move(indices)
    };
}
