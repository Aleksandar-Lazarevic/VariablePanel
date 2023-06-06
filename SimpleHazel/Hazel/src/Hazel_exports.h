#pragma once

#ifdef _WIN32
#  ifdef Hazel_EXPORTS
#    define HAZEL_API __declspec(dllexport)
#  elif Hazel_IMPORTS
#    define HAZEL_API __declspec(dllimport)
#  else
#    define HAZEL_API
#  endif
#elif __GNUC__ >= 4
#  ifdef Hazel_EXPORTS
#    define HAZEL_API __attribute__((visibility("default")))
#  else
#    define HAZEL_API
#  endif
#else
#   define HAZEL_API
#endif