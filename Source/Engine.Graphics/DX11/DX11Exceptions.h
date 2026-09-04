#pragma once
#include <BaseException.h>
#include <MinWindows.h>
#include <vector>

class DX11Exception : public BaseException
{
    using BaseException::BaseException;
};

class DX11HrException : public DX11Exception
{
  public:
    DX11HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;

  public:
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    HRESULT GetErrorCode() const noexcept;
    std::string GetErrorString() const noexcept;
    std::string GetErrorDescription() const noexcept;
    std::string GetErrorInfo() const noexcept;

  private:
    HRESULT hr;
    std::string info;
};

class DX11DeviceRemovedException : public DX11HrException
{
  public:
    using DX11HrException::DX11HrException;
    const char* GetType() const noexcept override;

  private:
    std::string reason;
};

class DX11InfoException : public DX11Exception
{
  public:
    DX11InfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    std::string GetErrorInfo() const noexcept;

  private:
    std::string info;
};

class DX11SurfaceException : public BaseException
{
  public:
    DX11SurfaceException(int line, const char* file, std::string note) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    const std::string& GetNote() const noexcept;

  private:
    std::string note;
};
