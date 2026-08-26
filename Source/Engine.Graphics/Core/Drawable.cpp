#include "Drawable.h"

Drawable::Drawable()
{}

void Drawable::Draw(Graphics& gfx) const
{
    mesh->Bind(gfx);
    pipeline->Bind(gfx);
    material->Bind(gfx);
    transform->Bind(gfx);

    DrawCall(gfx);
}

void Drawable::DrawCall(Graphics& gfx) const
{
    gfx.DrawIndexed(mesh->GetCount());
}