#pragma once
#include <directxcollision.h>

class Camera
{
  public:
    Camera() noexcept;
    DirectX::XMFLOAT3 pos;

  public:
    void Rotate(float dx, float dy) noexcept;
    void Translate(DirectX::XMFLOAT3 translation) noexcept;
    DirectX::XMMATRIX GetMatrix() const noexcept;
    DirectX::BoundingFrustum GetFrustum() const;
    void SetProjection(DirectX::FXMMATRIX proj) noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;

  private:
    DirectX::XMMATRIX projection;
    float pitch;
    float yaw;
    static constexpr float travelSpeed = 12.0f;
    static constexpr float rotationSpeed = 0.004f;
};
