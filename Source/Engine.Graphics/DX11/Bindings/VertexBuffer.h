#pragma once
#include "Bindable.h"
#include "../GraphicsExceptionsMacros.h"
#include "../../Core/Vertex.h"

class VertexBuffer : public Bindable
{
public:
    VertexBuffer(Graphics& gfx, const std::string& tag, const std::vector<Vertex::Standard>& vertices);
    VertexBuffer(Graphics& gfx, const std::vector<Vertex::Standard>& vertices);
    void Bind(Graphics& gfx) noexcept override;
    static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag, const std::vector<Vertex::Standard>& vertices);
    template<typename...Ignore>
    static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
    {
        return GenerateUID_(tag);
    }
    std::string GetUID() const noexcept override;
private:
    static std::string GenerateUID_(const std::string& tag);
protected:
    std::string tag;
    UINT stride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};