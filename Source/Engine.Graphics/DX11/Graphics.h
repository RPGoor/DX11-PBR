#pragma once
#include <MinWindows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <ConditionalNoexcept.h>
#include "DxgiInfoManager.h"
#include <memory>

using namespace Microsoft::WRL;

class Graphics
{
    friend class Bindable;
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics& operator =(const Graphics&) = delete;
    ~Graphics();

    void DrawIndexed(UINT count) conexcept;
    void SetProjection(DirectX::FXMMATRIX proj) noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;
    void EndFrame();
    void BeginFrame(float r, float g, float b) noexcept;

    void SetCamera(DirectX::FXMMATRIX cam) noexcept;
    DirectX::XMMATRIX GetCamera() const noexcept;

    void EnableImGui() noexcept;
    void DisableImGui() noexcept;
    bool IsImGuiEnabled() const noexcept;

private:
    bool imGuiEnabled = true;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX camera;
#ifndef NDEBUG
    DxgiInfoManager infoManager;
#endif

    ComPtr<ID3D11Device> pDevice = nullptr;
    ComPtr<IDXGISwapChain> pSwap = nullptr;
    ComPtr<ID3D11DeviceContext> pContext = nullptr;
    ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
    ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

};