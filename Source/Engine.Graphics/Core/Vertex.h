#pragma once

#include <DirectXMath.h>
#include <cstddef>
#include <d3d11.h>
#include <vector>

namespace Vertex
{
    struct Standard
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texcoord;
    };

    struct Instance
    {
        DirectX::XMFLOAT2 position;
        float rotation;
        float scale;
    };

    inline std::vector<D3D11_INPUT_ELEMENT_DESC>
    CombineLayouts(std::vector<D3D11_INPUT_ELEMENT_DESC> a, std::vector<D3D11_INPUT_ELEMENT_DESC> b)
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> result;
        result.reserve(a.size() + b.size());

        result.insert(result.end(), a.begin(), a.end());
        result.insert(result.end(), b.begin(), b.end());

        return result;
    }
} // namespace Vertex

template <typename T> struct VertexLayout;

template <> struct VertexLayout<Vertex::Standard>
{
    inline static const std::vector<D3D11_INPUT_ELEMENT_DESC> elements = {
        { "POSITION",
            0u,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0u,
            static_cast<UINT>(offsetof(Vertex::Standard, position)),
            D3D11_INPUT_PER_VERTEX_DATA,
            0u },
        { "NORMAL",
            0u,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0u,
            static_cast<UINT>(offsetof(Vertex::Standard, normal)),
            D3D11_INPUT_PER_VERTEX_DATA,
            0u },
        { "TEXCOORD",
            0u,
            DXGI_FORMAT_R32G32_FLOAT,
            0u,
            static_cast<UINT>(offsetof(Vertex::Standard, texcoord)),
            D3D11_INPUT_PER_VERTEX_DATA,
            0u }
    };
};

template <> struct VertexLayout<Vertex::Instance>
{
    inline static const std::vector<D3D11_INPUT_ELEMENT_DESC> elements = {
        { "INSTANCE_POSITION",
            0u,
            DXGI_FORMAT_R32G32_FLOAT,
            1u,
            static_cast<UINT>(offsetof(Vertex::Instance, position)),
            D3D11_INPUT_PER_INSTANCE_DATA,
            1u },
        { "INSTANCE_ROTATION",
            0u,
            DXGI_FORMAT_R32_FLOAT,
            1u,
            static_cast<UINT>(offsetof(Vertex::Instance, rotation)),
            D3D11_INPUT_PER_INSTANCE_DATA,
            1u },
        { "INSTANCE_SCALE",
            0u,
            DXGI_FORMAT_R32_FLOAT,
            1u,
            static_cast<UINT>(offsetof(Vertex::Instance, scale)),
            D3D11_INPUT_PER_INSTANCE_DATA,
            1u }
    };
};
