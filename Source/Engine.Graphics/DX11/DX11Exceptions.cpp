#include "DX11Exceptions.h"
#include <sstream>

DX11HrException::DX11HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    : DX11Exception(line, file), hr(hr)
{
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* DX11HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }

    oss << GetOriginString();
    WhatBuffer = oss.str();
    return WhatBuffer.c_str();
}

const char* DX11HrException::GetType() const noexcept
{
    return "Graphics Exception";
}

HRESULT DX11HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string DX11HrException::GetErrorString() const noexcept
{
    std::ostringstream oss;
    oss << "0x"
        << std::hex
        << std::uppercase
        << static_cast<unsigned long>(hr);
    return oss.str();
}

std::string DX11HrException::GetErrorDescription() const noexcept
{
    char* pMsgBuf = nullptr;

    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf),
        0,
        nullptr
    );

    if (nMsgLen == 0)
    {
        return "No error description available";
    }

    std::string description = pMsgBuf;
    LocalFree(pMsgBuf);

    return description;
}

std::string DX11HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char* DX11DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed]";
}

DX11InfoException::DX11InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    : DX11Exception(line, file)
{
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    if (!info.empty())
    {
        info.pop_back();
    }
}


const char* DX11InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    WhatBuffer = oss.str();
    return WhatBuffer.c_str();
}

const char* DX11InfoException::GetType() const noexcept
{
    return "Graphics Info Exception";
}

std::string DX11InfoException::GetErrorInfo() const noexcept
{
    return info;
}
