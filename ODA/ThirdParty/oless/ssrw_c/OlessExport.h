#ifndef _OLESS_EXPORT_H
#define _OLESS_EXPORT_H

#ifdef  _TOOLKIT_IN_DLL_
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#    pragma warning( disable: 4275 4251 )
#    ifdef  OLESS_EXPORT_API
#      define _OLESS_API    __declspec( dllexport )
#    else
#      define _OLESS_API    __declspec( dllimport )
#    endif
#  else // defined(__GNUC__) || defined(__APPLE__) or other unix
#    define _OLESS_API __attribute__((visibility("default")))
#  endif
#else
#  define _OLESS_API
#endif

#endif //_OLESS_EXPORT_H
