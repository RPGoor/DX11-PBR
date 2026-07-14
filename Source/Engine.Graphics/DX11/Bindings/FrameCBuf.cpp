#include "FrameCBuf.h"

FrameCbuf::FrameCbuf(Graphics& gfx)
    : bufferVS(gfx, 0u), bufferPS(gfx, 0u)
{}

void FrameCbuf::Update(Graphics & gfx, DirectX::FXMMATRIX view, DirectX::FXMMATRIX projection, const DirectX::XMFLOAT3 & cameraPosition, float time)
{
    FrameConstants data{};

    data.view = view;
    data.projection = projection;
    data.viewProjection = view * projection;
    data.cameraPositionWS = cameraPosition;
    data.time = time;

    bufferVS.Update(gfx, data);
    bufferPS.Update(gfx, data);

}

void FrameCbuf::Bind(Graphics & gfx) noexcept
{
    bufferVS.Bind(gfx);
    bufferPS.Bind(gfx);
}
