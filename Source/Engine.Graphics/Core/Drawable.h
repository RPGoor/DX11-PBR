#pragma once
#include "../DX11/Graphics.h"
#include "../DX11/Bindings/Bindable.h"
#include <memory>
#include <vector>
#include "../DX11/Bindings/IndexBuffer.h"

class Drawable
{
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    virtual ~Drawable() = default;

public:
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    void Draw(Graphics& gfx) const conexcept;

protected:
    template<class T>
    T* QueryBindable() noexcept
    {
        for (auto& pb : binds)
        {
            if (auto pt = dynamic_cast<T*>(pb.get()))
            {
                return pt;
            }
        }
        return nullptr;
    }

    void AddBind(std::shared_ptr<Bindable> bind) conexcept;

private:
    const IndexBuffer* pIndexBuffer = nullptr;
    std::vector<std::shared_ptr<Bindable>> binds;

};