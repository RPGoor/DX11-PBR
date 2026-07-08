#include "Drawable.h"


void Drawable::Draw(Graphics& gfx) const conexcept
{
    for (auto& b : binds)
    {
        b->Bind(gfx);
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind) conexcept
{
    if (typeid(*bind) == typeid(IndexBuffer))
    {
        assert("Binding multiple index buffers it not allowed" && pIndexBuffer == nullptr);
        pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
    }
    binds.push_back(std::move(bind));
}

