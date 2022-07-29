#pragma once

namespace Headway
{
    constexpr auto VERSION = "4.1";
    constexpr int VERSION_MAJOR = 4;
    constexpr int VERSION_MINOR = 1;

#if defined(Q_OS_MACOS)
    constexpr bool GLOBAL_MENUBAR = true;
#else
    constexpr bool GLOBAL_MENUBAR = false;
#endif

#if defined(Q_OS_WINDOWS)
    constexpr auto QUICK_CONTROLS_STYLE = "Universal";
#elif defined(Q_OS_MACOS)
    constexpr auto QUICK_CONTROLS_STYLE = "macOS";
#else
    constexpr auto QUICK_CONTROLS_STYLE = "Basic";
#endif
}
