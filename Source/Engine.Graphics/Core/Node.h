#pragma once

#include "../DX11/Graphics.h"
#include <ConditionalNoexcept.h>
#include <optional>
#include "Mesh.h"

class Node
{
    friend class Model;
public:
    Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) conexcept;
    void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const conexcept;
    void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
    int GetId() const noexcept;
    void ShowTree(Node*& pSelectedNode) const noexcept;
private:
    void AddChild(std::unique_ptr<Node> pChild) conexcept;
private:
    std::string name;
    int id;
    std::vector<std::unique_ptr<Node>> childPtrs;
    std::vector<Mesh*> meshPtrs;
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMFLOAT4X4 appliedTransform;
};