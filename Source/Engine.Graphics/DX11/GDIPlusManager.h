#pragma once
#include <MinWindows.h>

#pragma comment( lib,"gdiplus.lib" )

class GDIPlusManager
{
public:
    GDIPlusManager();
    ~GDIPlusManager();
private:
    static ULONG_PTR token;
    static int refCount;
};