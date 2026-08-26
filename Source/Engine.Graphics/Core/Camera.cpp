#include "Camera.h"
#include "imgui.h"
#include <algorithm>

namespace dx = DirectX;
constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;
Camera::Camera() noexcept
{
    pos = { 0.0f,7.5f,-18.0f };
    pitch = 0.0f;
    yaw = 0.0f;
}

float wrap_angle(float theta)
{
    const float modded = fmod(theta, 2.0 * PI_D);
    return (modded > PI_D) ?
        (modded - 2.0 * PI_D) :
        modded;
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    using namespace dx;

    const dx::XMVECTOR forwardBaseVector = XMVectorSet( 0.0f,0.0f,1.0f,0.0f );
    const auto lookVector = XMVector3Transform( forwardBaseVector,
        XMMatrixRotationRollPitchYaw( pitch,yaw,0.0f )
    );

    const auto camPosition = XMLoadFloat3( &pos );
    const auto camTarget = camPosition + lookVector;
    return XMMatrixLookAtLH( camPosition,camTarget,XMVectorSet( 0.0f,1.0f,0.0f,0.0f ) );
}

void Camera::Rotate( float dx,float dy ) noexcept
{
    yaw = wrap_angle( yaw + dx * rotationSpeed );
    pitch = std::clamp( pitch + dy * rotationSpeed,0.995f * -PI / 2.0f,0.995f * PI / 2.0f );
}

void Camera::Translate( DirectX::XMFLOAT3 translation ) noexcept
{
    const float yComp = translation.y;
    translation.y = 0.0f;
    dx::XMStoreFloat3( &translation,dx::XMVector3Transform(
        dx::XMLoadFloat3( &translation ),
        dx::XMMatrixRotationRollPitchYaw( pitch,yaw,0.0f ) *
        dx::XMMatrixScaling( travelSpeed,travelSpeed,travelSpeed )
    ) );
    pos = {
        pos.x + translation.x,
        pos.y + translation.y + (yComp * travelSpeed),
        pos.z + translation.z
    };
}
