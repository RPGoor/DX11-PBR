#include "Node.h"
#include "imgui.h"

Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) conexcept
    :
id(id),
meshPtrs(std::move(meshPtrs)),
name(name)
{
    DirectX::XMStoreFloat4x4(&transform, transform_in);
    DirectX::XMStoreFloat4x4(&appliedTransform, DirectX::XMMatrixIdentity());
}

void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const conexcept
{
    const auto built =
        DirectX::XMLoadFloat4x4(&appliedTransform) *
        DirectX::XMLoadFloat4x4(&transform) *
        accumulatedTransform;
    for (const auto pm : meshPtrs)
    {
        pm->Draw(gfx, built);
    }
    for (const auto& pc : childPtrs)
    {
        pc->Draw(gfx, built);
    }
}

void Node::DrawInstanced(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform, UINT instanceCount) const conexcept
{
    const auto built =
        DirectX::XMLoadFloat4x4(&appliedTransform) *
        DirectX::XMLoadFloat4x4(&transform) *
        accumulatedTransform;
    for (const auto pm : meshPtrs)
    {
        pm->DrawInstanced(gfx, built, instanceCount);
    }
    for (const auto& pc : childPtrs)
    {
        pc->DrawInstanced(gfx, built, instanceCount);
    }
}

void Node::AddChild(std::unique_ptr<Node> pChild) conexcept
{
    assert(pChild);
    childPtrs.push_back(std::move(pChild));
}

void Node::ShowTree(Node*& pSelectedNode) const noexcept
{
    // if there is no selected node, set selectedId to an impossible value
    const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
    // build up flags for current node
    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
        | ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
    // render this node
    const auto expanded = ImGui::TreeNodeEx(
        (void*)(intptr_t)GetId(), node_flags, name.c_str()
    );
    // processing for selecting node
    if (ImGui::IsItemClicked())
    {
        pSelectedNode = const_cast<Node*>(this);
    }
    // recursive rendering of open node's children
    if (expanded)
    {
        for (const auto& pChild : childPtrs)
        {
            pChild->ShowTree(pSelectedNode);
        }
        ImGui::TreePop();
    }
}

void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
    DirectX::XMStoreFloat4x4(&appliedTransform, transform);
}

int Node::GetId() const noexcept
{
    return id;
}