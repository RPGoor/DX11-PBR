#pragma once 

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "../DX11/Graphics.h"
#include <ConditionalNoexcept.h>
#include "Mesh.h"
#include "Node.h"
#include <memory>

class Model
{
public:
    Model(Graphics& gfx, const std::string fileName, bool isInstanced = 0);
    void Draw(Graphics& gfx, DirectX::XMMATRIX position) const conexcept;
    void DrawInstanced(Graphics& gfx, DirectX::XMMATRIX position, UINT instanceCount) const conexcept;

    void ShowWindow(const char* windowName = nullptr) noexcept;
    ~Model() noexcept;
private:
    std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
    std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node) noexcept;
private:
    bool isInstanced;

    std::unique_ptr<Node> pRoot;
    std::vector<std::unique_ptr<Mesh>> meshPtrs;
    std::unique_ptr<class ModelWindow> pWindow;
};