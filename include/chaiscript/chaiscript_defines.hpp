// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2015, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_DEFINES_HPP_
#define CHAISCRIPT_DEFINES_HPP_

#ifdef _MSC_VER
#define CHAISCRIPT_MSVC _MSC_VER
#define CHAISCRIPT_HAS_DECLSPEC
#if _MSC_VER <= 1800
#error "MSVC 2013 is no longer supported"
#endif
#endif


#include <vector>

#if defined( _LIBCPP_VERSION )
#define CHAISCRIPT_LIBCPP
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define CHAISCRIPT_WINDOWS
#endif


#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ == 6)
#define CHAISCRIPT_GCC_4_6
#endif


#ifdef  CHAISCRIPT_HAS_DECLSPEC
#define CHAISCRIPT_MODULE_EXPORT extern "C" __declspec(dllexport)
#else
#define CHAISCRIPT_MODULE_EXPORT extern "C" 
#endif


#include <memory>

namespace chaiscript {
  static const int version_major = 5;
  static const int version_minor = 7;
  static const int version_patch = 2;

  template<typename B, typename D, typename ...Arg>
  inline std::shared_ptr<B> make_shared(Arg && ... arg)
  {
#ifdef CHAISCRIPT_USE_STD_MAKE_SHARED
    return std::make_shared<D>(std::forward<Arg>(arg)...);
#else
    return std::shared_ptr<B>(static_cast<B*>(new D(std::forward<Arg>(arg)...)));
#endif
  }

}
#endif

