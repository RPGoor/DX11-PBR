#include "Win32Exceptions.h"
#include <sstream>

Win32HrException::Win32HrException(int line, const char* file, HRESULT hr) noexcept
    : Win32Exception(line, file), hr(hr)
{}

const char* Win32HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType()
        << std::endl
        << "[Error Code] "
        << GetErrorCode()
        << std::endl
        << "[Description] "
        << GetErrorDescription()
        << std::endl
        << GetOriginString();

    WhatBuffer = oss.str();
    return WhatBuffer.c_str();
}

const char* Win32HrException::GetType() const noexcept
{
    return "Window Exception";
}

std::string Win32Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&pMsgBuf),
        0,
        nullptr);

    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);

    return errorString;
}

HRESULT Win32HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Win32HrException::GetErrorDescription() const noexcept
{
    return Win32Exception::TranslateErrorCode(hr);
}

const char* Win32NoGfxException::GetType() const noexcept
{
    return "Window Exception [No Graphics Device]";
}
