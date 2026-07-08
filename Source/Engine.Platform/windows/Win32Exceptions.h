#pragma once
#include <BaseException.h>
#include "MinWindows.h"

class Win32Exception : public BaseException
{
    using BaseException::BaseException;
public:
    static std::string TranslateErrorCode(HRESULT hr) noexcept;
};

class Win32HrException : public Win32Exception
{
public:
    Win32HrException(int line, const char* file, HRESULT hr) noexcept;
    const char* what() const noexcept override;
    virtual const char* GetType() const noexcept override;
    HRESULT GetErrorCode() const noexcept;
    std::string GetErrorDescription() const noexcept;

private:
    HRESULT hr;
};
class Win32NoGfxException : public Win32Exception
{
public:
    using Win32Exception::Win32Exception;
    const char* GetType() const noexcept override;
};