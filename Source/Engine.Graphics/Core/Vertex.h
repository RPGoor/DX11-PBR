#pragma once
#include <vector>
#include <type_traits>
#include "../DX11/Graphics.h"
#include "Color.h"
#include <ConditionalNoexcept.h>

namespace Dvtx
{
    class VertexLayout
    {
    public:
        enum ElementType
        {
            Position2D,
            Position3D,
            Texture2D,
            Normal,
            Float3Color,
            Float4Color,
            BGRAColor,
            InstanceTransform0,
            InstanceTransform1,
            InstanceTransform2,
            InstanceTransform3,
            Count,
        };
        template<ElementType> struct Map;
        template<> struct Map<Position2D>
        {
            using SysType = DirectX::XMFLOAT2;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
            static constexpr const char* semantic = "Position";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "P2";
        };
        template<> struct Map<Position3D>
        {
            using SysType = DirectX::XMFLOAT3;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            static constexpr const char* semantic = "Position";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "P3";
        };
        template<> struct Map<Texture2D>
        {
            using SysType = DirectX::XMFLOAT2;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
            static constexpr const char* semantic = "Texcoord";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "T2";
        };
        template<> struct Map<Normal>
        {
            using SysType = DirectX::XMFLOAT3;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            static constexpr const char* semantic = "Normal";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "N";
        };
        template<> struct Map<Float3Color>
        {
            using SysType = DirectX::XMFLOAT3;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            static constexpr const char* semantic = "Color";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "C3";
        };
        template<> struct Map<Float4Color>
        {
            using SysType = DirectX::XMFLOAT4;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
            static constexpr const char* semantic = "Color";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "C4";
        };
        template<> struct Map<BGRAColor>
        {
            using SysType = ::BGRAColor;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            static constexpr const char* semantic = "Color";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "C8";
        };
        template<>
        struct Map<InstanceTransform0>
        {
            using SysType = DirectX::XMFLOAT4;
            static constexpr DXGI_FORMAT dxgiFormat =
                DXGI_FORMAT_R32G32B32A32_FLOAT;
            static constexpr const char* semantic = "InstanceTransform";
            static constexpr UINT semanticIndex = 0u;
            static constexpr const char* code = "IT0";
        };

        template<>
        struct Map<InstanceTransform1>
        {
            using SysType = DirectX::XMFLOAT4;
            static constexpr DXGI_FORMAT dxgiFormat =
                DXGI_FORMAT_R32G32B32A32_FLOAT;
            static constexpr const char* semantic = "InstanceTransform";
            static constexpr UINT semanticIndex = 1u;
            static constexpr const char* code = "IT1";
        };

        template<>
        struct Map<InstanceTransform2>
        {
            using SysType = DirectX::XMFLOAT4;
            static constexpr DXGI_FORMAT dxgiFormat =
                DXGI_FORMAT_R32G32B32A32_FLOAT;
            static constexpr const char* semantic = "InstanceTransform";
            static constexpr UINT semanticIndex = 2u;
            static constexpr const char* code = "IT2";
        };

        template<>
        struct Map<InstanceTransform3>
        {
            using SysType = DirectX::XMFLOAT4;
            static constexpr DXGI_FORMAT dxgiFormat =
                DXGI_FORMAT_R32G32B32A32_FLOAT;
            static constexpr const char* semantic = "InstanceTransform";
            static constexpr UINT semanticIndex = 3u;
            static constexpr const char* code = "IT3";
        };
        class Element
        {
        public:
            Element(ElementType type, size_t offset, UINT inputSlot,
                D3D11_INPUT_CLASSIFICATION classification,
                UINT instanceStepRate);
            size_t GetOffsetAfter() const conexcept;
            size_t GetOffset() const;
            size_t Size() const conexcept;
            static constexpr size_t SizeOf(ElementType type) conexcept;
            ElementType GetType() const noexcept;
            D3D11_INPUT_ELEMENT_DESC GetDesc() const conexcept;
            const char* GetCode() const noexcept;
        private:
            template<ElementType type>
            D3D11_INPUT_ELEMENT_DESC GenerateDesc() const noexcept
            {
                return D3D11_INPUT_ELEMENT_DESC{
                    Map<type>::semantic,
                    Map<type>::semanticIndex,
                    Map<type>::dxgiFormat,
                    inputSlot,
                    static_cast<UINT>(offset),
                    classification,
                    instanceStepRate
                };
            }
        private:
            ElementType type;
            size_t offset;
            UINT inputSlot;
            D3D11_INPUT_CLASSIFICATION classification;
            UINT instanceStepRate;
        };
    public:
        template<ElementType Type>
        const Element& Resolve() const conexcept
        {
            for (auto& e : elements)
            {
                if (e.GetType() == Type)
                {
                    return e;
                }
            }
            assert("Could not resolve element type" && false);
            return elements.front();
        }
        const Element& ResolveByIndex(size_t i) const conexcept;
        VertexLayout& Append(ElementType type) conexcept;
        VertexLayout& AppendInstance(ElementType type) conexcept;

        size_t Size() const conexcept;
        size_t GetElementCount() const noexcept;
        std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const conexcept;
        std::string GetCode() const conexcept;
    private:
        std::vector<Element> elements;
    };

    class Vertex
    {
        friend class VertexBuffer;
    public:
        template<VertexLayout::ElementType Type>
        auto& Attr() conexcept
        {
            auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
            return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
        }
        template<typename T>
        void SetAttributeByIndex(size_t i, T&& val) conexcept
        {
            const auto& element = layout.ResolveByIndex(i);
            auto pAttribute = pData + element.GetOffset();
            switch (element.GetType())
            {
            case VertexLayout::Position2D:
                SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
                break;
            case VertexLayout::Position3D:
                SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
                break;
            case VertexLayout::Texture2D:
                SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
                break;
            case VertexLayout::Normal:
                SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
                break;
            case VertexLayout::Float3Color:
                SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
                break;
            case VertexLayout::Float4Color:
                SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
                break;
            case VertexLayout::BGRAColor:
                SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
                break;
            default:
                assert("Bad element type" && false);
            }
        }
    protected:
        Vertex(char* pData, const VertexLayout& layout) conexcept;
    private:
        // enables parameter pack setting of multiple parameters by element index
        template<typename First, typename ...Rest>
        void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) conexcept
        {
            SetAttributeByIndex(i, std::forward<First>(first));
            SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
        }
        // helper to reduce code duplication in SetAttributeByIndex
        template<VertexLayout::ElementType DestLayoutType, typename SrcType>
        void SetAttribute(char* pAttribute, SrcType&& val) conexcept
        {
            using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
            if constexpr (std::is_assignable<Dest, SrcType>::value)
            {
                *reinterpret_cast<Dest*>(pAttribute) = val;
            }
            else
            {
                assert("Parameter attribute type mismatch" && false);
            }
        }
    private:
        char* pData = nullptr;
        const VertexLayout& layout;
    };

    class ConstVertex
    {
    public:
        ConstVertex(const Vertex& v) conexcept;
        template<VertexLayout::ElementType Type>
        const auto& Attr() const conexcept
        {
            return const_cast<Vertex&>(vertex).Attr<Type>();
        }
    private:
        Vertex vertex;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer(VertexLayout layout) conexcept;
        const char* GetData() const conexcept;
        const VertexLayout& GetLayout() const noexcept;
        size_t Size() const conexcept;
        size_t SizeBytes() const conexcept;
        template<typename ...Params>
        void EmplaceBack(Params&&... params) conexcept
        {
            assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
            buffer.resize(buffer.size() + layout.Size());
            Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
        }
        Vertex Back() conexcept;
        Vertex Front() conexcept;
        Vertex operator[](size_t i) conexcept;
        ConstVertex Back() const conexcept;
        ConstVertex Front() const conexcept;
        ConstVertex operator[](size_t i) const conexcept;
    private:
        std::vector<char> buffer;
        VertexLayout layout;
    };
}