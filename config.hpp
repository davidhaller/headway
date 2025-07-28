#pragma once

#include <QtSystemDetection>

namespace Headway
{
    constexpr char VERSION[] = "4.1";
    constexpr int VERSION_MAJOR = 4;
    constexpr int VERSION_MINOR = 1;

#if defined(Q_OS_MACOS)
    constexpr bool GLOBAL_MENUBAR = true;
#else
    constexpr bool GLOBAL_MENUBAR = false;
#endif

#if defined(Q_OS_WIN)
    constexpr char QUICK_CONTROLS_STYLE[] = "Universal";
#elif defined(Q_OS_MACOS)
    constexpr char QUICK_CONTROLS_STYLE[] = "macOS";
#else
    constexpr char QUICK_CONTROLS_STYLE[] = "Basic";
#endif
}
