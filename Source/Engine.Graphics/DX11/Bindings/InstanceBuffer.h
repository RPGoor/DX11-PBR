#pragma once

#include "../../Core/Vertex.h"
#include "../Bindable.h"

class InstanceBuffer : public Bindable
{
  public:
    InstanceBuffer(Graphics& gfx, UINT maxCount);

    void Bind(Graphics& gfx) noexcept override;

    UINT GetCount() const noexcept;
    ID3D11UnorderedAccessView* GetUAV() const noexcept;

  private:
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pUnorderedAccessView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
    UINT stride = sizeof(Vertex::Instance);
    UINT count = 0u;
};
