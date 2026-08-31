#include "Drawable.h"

Drawable::Drawable()
{}

void Drawable::Bind(Graphics& gfx) const
{
    mesh->Bind(gfx);
    pipeline->Bind(gfx);
    material->Bind(gfx);
    transform->Bind(gfx);
}

void Drawable::Draw(Graphics& gfx) const
{
    Bind(gfx);
    DrawCall(gfx);
}

void Drawable::Draw(Graphics& gfx, UINT instanceCount) const
{
    Bind(gfx);
    DrawCall(gfx, instanceCount);
}

void Drawable::DrawCall(Graphics& gfx) const
{
    gfx.DrawIndexed(mesh->GetCount());
}

void Drawable::DrawCall(Graphics& gfx, UINT instanceCount) const
{
    gfx.DrawIndexed(mesh->GetCount(), instanceCount);
}
