#pragma once

#ifdef _WIN32
#  ifdef ForcePanel_EXPORTS
#    define FORCE_PANEL_API __declspec(dllexport)
#  elif ForcePanel_IMPORTS
#    define FORCE_PANEL_API __declspec(dllimport)
#  else
#    define FORCE_PANEL_API
#  endif
#elif __GNUC__ >= 4
#  ifdef ForcePanel_EXPORTS
#    define FORCE_PANEL_API __attribute__((visibility("default")))
#  else
#    define FORCE_PANEL_API
#  endif
#else
#   define FORCE_PANEL_API
#endif