#include "MeshFactory.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "ModelException.h"
#include <string>

MeshData MeshFactory::Load(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals
    );

    if (!scene || !scene->HasMeshes())
    {
        throw ModelException(__LINE__, __FILE__, importer.GetErrorString());
    }

    const aiMesh& mesh = *scene->mMeshes[0];

    std::vector<Vertex::Standard> vertices;

    const bool hasColors = mesh.HasVertexColors(0);

    for (unsigned int i = 0; i < mesh.mNumVertices; i++)
    {
        DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
        if (hasColors)
        {
            color = *reinterpret_cast<DirectX::XMFLOAT4*>(&mesh.mColors[0][i]);
        }

        vertices.push_back(Vertex::Standard
        {
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
            *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i]),
            color
        });
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
    return MeshData{
        std::string("load#") + mesh.mName.C_Str(),
        std::move(vertices),
        std::move(indices)
    };
}

//MeshData MeshFactory::Grid(
//    uint32_t resolution,
//    float size
//)
//{
//    //MeshData data {std::string("load#"), nullptr, nullptr};
//
//    // generate vertices + indices
//
//    return;
//}