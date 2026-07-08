#pragma once
#include "Win32Exceptions.h"

#define ASOLE_WND_EXCEPT(hr) Win32HrException(__LINE__, __FILE__, hr)
#define ASOLE_WND_NOGFX_EXCEPT() Win32NoGfxException(__LINE__, __FILE__)
#define ASOLE_WND_LAST_EXCEPT() Win32HrException(__LINE__, __FILE__, HRESULT_FROM_WIN32(GetLastError()))
