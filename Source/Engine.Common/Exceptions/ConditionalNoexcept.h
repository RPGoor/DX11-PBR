#pragma once

#ifndef NDEBUG
#define conexcept noexcept(false)
#else
#define conexcept noexcept(true)
#endif