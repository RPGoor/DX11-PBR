#include "ModelException.h"
#include <sstream>

ModelException::ModelException(int line, const char* file, std::string note) noexcept
    : BaseException(line, file),
      note(std::move(note))
{
}

const char* ModelException::what() const noexcept
{
    std::ostringstream oss;
    oss << BaseException::what() << std::endl << "[Note] " << GetNote();
    WhatBuffer = oss.str();
    return WhatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
    return "Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
    return note;
}
