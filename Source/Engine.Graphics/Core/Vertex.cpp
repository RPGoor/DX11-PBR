#include "Vertex.h"

namespace Dvtx
{
    // VertexLayout
    const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t i) const conexcept
    {
        return elements[i];
    }
    VertexLayout& VertexLayout::Append(ElementType type) conexcept
    {
        elements.emplace_back(type, Size(), 0u,
            D3D11_INPUT_PER_VERTEX_DATA,
            0u);
        return *this;
    }

    VertexLayout& VertexLayout::AppendInstance(ElementType type) conexcept
    {
        elements.emplace_back(
            type,
            Size(),
            1u,
            D3D11_INPUT_PER_INSTANCE_DATA,
            1u
        );

        return *this;
    }

    size_t VertexLayout::Size() const conexcept
    {
        return elements.empty() ? 0u : elements.back().GetOffsetAfter();
    }
    size_t VertexLayout::GetElementCount() const noexcept
    {
        return elements.size();
    }
    std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const conexcept
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
        desc.reserve(GetElementCount());
        for (const auto& e : elements)
        {
            desc.push_back(e.GetDesc());
        }
        return desc;
    }
    std::string VertexLayout::GetCode() const conexcept
    {
        std::string code;
        for (const auto& e : elements)
        {
            code += e.GetCode();
        }
        return code;
    }


    // VertexLayout::Element
    VertexLayout::Element::Element(ElementType type, size_t offset,
        UINT inputSlot,
        D3D11_INPUT_CLASSIFICATION classification,
        UINT instanceStepRate)
        :
        type(type),
        offset(offset),
        inputSlot(inputSlot),
        classification(classification),
        instanceStepRate(instanceStepRate)
    {}
    size_t VertexLayout::Element::GetOffsetAfter() const conexcept
    {
        return offset + Size();
    }
    size_t VertexLayout::Element::GetOffset() const
    {
        return offset;
    }
    size_t VertexLayout::Element::Size() const conexcept
    {
        return SizeOf(type);
    }
    constexpr size_t VertexLayout::Element::SizeOf(ElementType type) conexcept
    {
        switch (type)
        {
        case Position2D:
            return sizeof(Map<Position2D>::SysType);
        case Position3D:
            return sizeof(Map<Position3D>::SysType);
        case Texture2D:
            return sizeof(Map<Texture2D>::SysType);
        case Normal:
            return sizeof(Map<Normal>::SysType);
        case Float3Color:
            return sizeof(Map<Float3Color>::SysType);
        case Float4Color:
            return sizeof(Map<Float4Color>::SysType);
        case BGRAColor:
            return sizeof(Map<BGRAColor>::SysType);
        case InstanceTransform0:
            return sizeof(Map<InstanceTransform0>::SysType);

        case InstanceTransform1:
            return sizeof(Map<InstanceTransform1>::SysType);

        case InstanceTransform2:
            return sizeof(Map<InstanceTransform2>::SysType);

        case InstanceTransform3:
            return sizeof(Map<InstanceTransform3>::SysType);

        }
        assert("Invalid element type" && false);
        return 0u;
    }
    VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
    {
        return type;
    }
    const char* Dvtx::VertexLayout::Element::GetCode() const noexcept
    {
        switch (type)
        {
        case Position2D:
            return Map<Position2D>::code;
        case Position3D:
            return Map<Position3D>::code;
        case Texture2D:
            return Map<Texture2D>::code;
        case Normal:
            return Map<Normal>::code;
        case Float3Color:
            return Map<Float3Color>::code;
        case Float4Color:
            return Map<Float4Color>::code;
        case BGRAColor:
            return Map<BGRAColor>::code;
        case InstanceTransform0:
            return Map<InstanceTransform0>::code;

        case InstanceTransform1:
            return Map<InstanceTransform1>::code;

        case InstanceTransform2:
            return Map<InstanceTransform2>::code;

        case InstanceTransform3:
            return Map<InstanceTransform3>::code;
        }
        assert("Invalid element type" && false);
        return "Invalid";
    }
    D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const conexcept
    {
        switch (type)
        {
        case Position2D:
            return GenerateDesc<Position2D>();
        case Position3D:
            return GenerateDesc<Position3D>();
        case Texture2D:
            return GenerateDesc<Texture2D>();
        case Normal:
            return GenerateDesc<Normal>();
        case Float3Color:
            return GenerateDesc<Float3Color>();
        case Float4Color:
            return GenerateDesc<Float4Color>();
        case BGRAColor:
            return GenerateDesc<BGRAColor>();
        case InstanceTransform0:
            return GenerateDesc<InstanceTransform0>();

        case InstanceTransform1:
            return GenerateDesc<InstanceTransform1>();

        case InstanceTransform2:
            return GenerateDesc<InstanceTransform2>();

        case InstanceTransform3:
            return GenerateDesc<InstanceTransform3>();
        }
        assert("Invalid element type" && false);
        return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
    }


    // Vertex
    Vertex::Vertex(char* pData, const VertexLayout& layout) conexcept
        :
    pData(pData),
        layout(layout)
    {
        assert(pData != nullptr);
    }
    ConstVertex::ConstVertex(const Vertex& v) conexcept
        :
    vertex(v)
    {}


    // VertexBuffer
    VertexBuffer::VertexBuffer(VertexLayout layout) conexcept
        :
    layout(std::move(layout))
    {}
    const char* VertexBuffer::GetData() const conexcept
    {
        return buffer.data();
    }
    const VertexLayout& VertexBuffer::GetLayout() const noexcept
    {
        return layout;
    }
    size_t VertexBuffer::Size() const conexcept
    {
        return buffer.size() / layout.Size();
    }
    size_t VertexBuffer::SizeBytes() const conexcept
    {
        return buffer.size();
    }
    Vertex VertexBuffer::Back() conexcept
    {
        assert(buffer.size() != 0u);
        return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
    }
    Vertex VertexBuffer::Front() conexcept
    {
        assert(buffer.size() != 0u);
        return Vertex{ buffer.data(),layout };
    }
    Vertex VertexBuffer::operator[](size_t i) conexcept
    {
        assert(i < Size());
        return Vertex{ buffer.data() + layout.Size() * i,layout };
    }
    ConstVertex VertexBuffer::Back() const conexcept
    {
        return const_cast<VertexBuffer*>(this)->Back();
    }
    ConstVertex VertexBuffer::Front() const conexcept
    {
        return const_cast<VertexBuffer*>(this)->Front();
    }
    ConstVertex VertexBuffer::operator[](size_t i) const conexcept
    {
        return const_cast<VertexBuffer&>(*this)[i];
    }
}