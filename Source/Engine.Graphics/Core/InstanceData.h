#pragma once

#include <DirectXMath.h>
#include "Vertex.h"

struct InstanceData
{
    DirectX::XMFLOAT4 row0;
    DirectX::XMFLOAT4 row1;
    DirectX::XMFLOAT4 row2;
    DirectX::XMFLOAT4 row3;
    DirectX::XMFLOAT4 row4;

    static Dvtx::VertexLayout GetLayout()
    {
        return Dvtx::VertexLayout{}
            .AppendInstance(
                Dvtx::VertexLayout::InstanceTransform0
            )
            .AppendInstance(
                Dvtx::VertexLayout::InstanceTransform1
            )
            .AppendInstance(
                Dvtx::VertexLayout::InstanceTransform2
            )
            .AppendInstance(
                Dvtx::VertexLayout::InstanceTransform3
            )
            .AppendInstance(
                Dvtx::VertexLayout::InstanceTransform4
            );;
    }
};