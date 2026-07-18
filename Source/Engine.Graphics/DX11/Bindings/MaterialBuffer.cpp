#include "MaterialBuffer.h"
#include <imgui.h>

MaterialBuffer::MaterialBuffer(Graphics & gfx, MaterialConstants material)
    : cbuf(gfx, 2u), cbData(material)
{
}



void MaterialBuffer::Bind(Graphics& gfx) noexcept
{
    cbuf.Resolve(gfx);
}
