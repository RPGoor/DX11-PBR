#include "Model.h"
#include <ConditionalNoexcept.h>
#include "imgui.h"
#include <unordered_map>
#include "ModelException.h"
#include "../DX11/Bindings/VertexBuffer.h"
#include "../DX11/Bindings/VertexShader.h"
#include "../DX11/Bindings/PixelShader.h"
#include "../DX11/Bindings/InputLayout.h"
#include "../DX11/Bindings/ConstantBuffers.h"
#include "../DX11/Bindings/Texture.h"
#include "../DX11/Bindings/Sampler.h"
#include "Vertex.h"
#include <random>

class ModelWindow
{
public:
    void Show(const char* windowName, const Node& root) noexcept
    {
        // window name defaults to "Model"
        windowName = windowName ? windowName : "Model";
        // need an ints to track node indices and selected node
        int nodeIndexTracker = 0;
        if (ImGui::Begin(windowName))
        {
            ImGui::Columns(2, nullptr, true);
            root.ShowTree(pSelectedNode);

            ImGui::NextColumn();
            if (pSelectedNode != nullptr)
            {
                auto& transform = transforms[pSelectedNode->GetId()];
                ImGui::Text("Orientation");
                ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
                ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
                ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
                ImGui::Text("Position");
                ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
                ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
                ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
            }
        }
        ImGui::End();
    }
    DirectX::XMMATRIX GetTransform() const noexcept
    {
        assert(pSelectedNode != nullptr);
        const auto& transform = transforms.at(pSelectedNode->GetId());
        return
            DirectX::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
            DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
    }
    Node* GetSelectedNode() const noexcept
    {
        return pSelectedNode;
    }
private:
    Node* pSelectedNode;
    struct TransformParameters
    {
        float roll = 0.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };
    std::unordered_map<int, TransformParameters> transforms;
};

Model::Model(Graphics& gfx, const std::string fileName)
    :
    pWindow(std::make_unique<ModelWindow>())
{
    Assimp::Importer imp;
    const auto pScene = imp.ReadFile(fileName.c_str(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_GenNormals
    );

    if (pScene == nullptr)
    {
        throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
    }

    for (size_t i = 0; i < pScene->mNumMeshes; i++)
    {
        meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
    }

    int nextId = 0;
    pRoot = ParseNode(nextId, *pScene->mRootNode);
}

void Model::Draw(Graphics& gfx, DirectX::XMMATRIX position) const conexcept
{
    if (auto node = pWindow->GetSelectedNode())
    {
        node->SetAppliedTransform(pWindow->GetTransform());
    }

    pRoot->Draw(gfx, position);
    //pRoot->Draw(gfx, DirectX::XMMatrixIdentity());
}

void Model::ShowWindow(const char* windowName) noexcept
{
    pWindow->Show(windowName, *pRoot);
}

Model::~Model() noexcept
{}

std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
    using Dvtx::VertexLayout;

    Dvtx::VertexBuffer vbuf(std::move(
        VertexLayout{}
        .Append(VertexLayout::Position3D)
        .Append(VertexLayout::Normal)
        .Append(VertexLayout::Texture2D)
    ));

    for (unsigned int i = 0; i < mesh.mNumVertices; i++)
    {
        vbuf.EmplaceBack(
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
            *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
        );
    }

    std::vector<unsigned short> indices;
    indices.reserve(mesh.mNumFaces * 3);
    for (unsigned int i = 0; i < mesh.mNumFaces; i++)
    {
        const auto& face = mesh.mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    std::vector<std::shared_ptr<Bindable>> bindablePtrs;

    using namespace std::string_literals;
    const auto base = "..\\..\\Assets\\Models\\nano_textured\\"s;

    bool hasSpecularMap = false;
    float shininess = 35.0f;
    if (mesh.mMaterialIndex > 0)
    {
        auto& material = *pMaterials[mesh.mMaterialIndex];

        aiString texFileName;

        material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
        //bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str()));

        if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
        {
            bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str(), 1));
            hasSpecularMap = true;
        }
        else
        {
            material.Get(AI_MATKEY_SHININESS, shininess);
        }

        bindablePtrs.push_back(Sampler::Resolve(gfx));
    }

    auto meshTag = base + "%" + mesh.mName.C_Str();

    bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

    bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

    auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
    auto pvsbc = pvs->GetBytecode();
    bindablePtrs.push_back(std::move(pvs));

    bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

    if (hasSpecularMap)
    {
        bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSSpecMap.cso"));
    }
    else
    {
        bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS.cso"));

        struct PSMaterialConstant
        {
            float specularIntensity = 0.8f;
            float specularPower;
            float padding[2];
        } pmc;
        pmc.specularPower = shininess;
        // this is CLEARLY an issue... all meshes will share same mat const, but may have different
        // Ns (specular power) specified for each in the material properties... bad conflict
        bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));
    }

    return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node) noexcept
{
    namespace dx = DirectX;
    const auto transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(
        reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation)
    ));

    std::vector<Mesh*> curMeshPtrs;
    curMeshPtrs.reserve(node.mNumMeshes);
    for (size_t i = 0; i < node.mNumMeshes; i++)
    {
        const auto meshIdx = node.mMeshes[i];
        curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
    }

    auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
    for (size_t i = 0; i < node.mNumChildren; i++)
    {
        pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
    }

    return pNode;
}