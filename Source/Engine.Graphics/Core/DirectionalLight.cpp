#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Graphics& gfx, float radius)
    : cbuf(gfx, 3u),
      cbData({
          {0.0f, -1.0f, 0.5f},
          5.0f,
          {1.0f, 1.0f, 1.0f},
          0.0f,
      })
{
}

void DirectionalLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
    auto dataCopy = cbData;

    const auto direction = DirectX::XMLoadFloat3(&dataCopy.directionWS);

    DirectX::XMStoreFloat3(&dataCopy.directionWS, DirectX::XMVector3Normalize(direction));

    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
}
