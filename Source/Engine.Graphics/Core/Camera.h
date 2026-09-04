#pragma once
#include "../DX11/Graphics.h"
#include <directxcollision.h>

class Camera
{
  public:
    Camera() noexcept;
    DirectX::XMMATRIX GetMatrix() const noexcept;
    void Rotate(float dx, float dy) noexcept;
    void Translate(DirectX::XMFLOAT3 translation) noexcept;

    DirectX::BoundingFrustum GetFrustum() const;

    void SetProjection(DirectX::FXMMATRIX proj) noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;

    DirectX::XMFLOAT3 pos;

  private:
    float pitch;
    float yaw;
    static constexpr float travelSpeed = 12.0f;
    static constexpr float rotationSpeed = 0.004f;

    DirectX::XMMATRIX projection;
};
