#pragma once
#include "../Bindable.h"

class IndirectArgsBuffer : public Bindable
{
  public:
    IndirectArgsBuffer(Graphics& gfx, UINT indexCount);
    ID3D11Buffer* GetBuffer() const noexcept;

    void Bind(Graphics&) noexcept override {}

  private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> indirectArgs;
};
