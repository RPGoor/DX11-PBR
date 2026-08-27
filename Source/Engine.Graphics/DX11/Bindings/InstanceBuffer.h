#pragma once

#include "Bindable.h"
#include "../../Core/Vertex.h"

class InstanceBuffer : public Bindable
{
public:
    InstanceBuffer(Graphics& gfx, const std::vector<Vertex::Instance>& instances);

    void Bind(Graphics& gfx) noexcept override;

    UINT GetCount() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
    UINT stride = sizeof(Vertex::Instance);
    UINT count = 0u;
};