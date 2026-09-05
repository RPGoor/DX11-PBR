#pragma once
#include "../Bindable.h"

class IndirectArgsBuffer : public Bindable
{
  public:
    IndirectArgsBuffer(Graphics& gfx, UINT indexCount);

    void Bind(Graphics&) noexcept override {}

    ID3D11Buffer* GetBuffer() const noexcept;

  private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> indirectArgs;
};
