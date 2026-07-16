#pragma once

#include "Bindable.h"
#include "../../Core/InstanceData.h"


class InstanceBuffer : public Bindable
{
public:


    InstanceBuffer(
        Graphics& gfx,
        const std::vector<InstanceData>& instances
    );

    void Bind(Graphics& gfx) noexcept override;

    UINT GetCount() const noexcept;

private:


    Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
    UINT stride = sizeof(InstanceData);
    UINT count = 0u;
};