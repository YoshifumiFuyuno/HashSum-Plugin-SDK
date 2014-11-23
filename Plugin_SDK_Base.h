/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: Mozilla Public License Version 2.0 <https://www.mozilla.org/MPL/2.0/>
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __HSPSDK_BASE_H
#define __HSPSDK_BASE_H

#if defined(_MSC_VER) && (_MSC_VER < 1600)
  #ifndef STDINT_H
  typedef __int32          int32_t;
  typedef unsigned __int8  uint8_t;
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int32 uint32_t;
  typedef unsigned __int64 uint64_t;
  typedef unsigned char    uint_fast8_t;
  typedef unsigned int     uint_fast16_t;
  typedef unsigned int     uint_fast32_t;
  //typedef _ULonglong       uint_fast64_t;
  typedef unsigned __int64 uint_fast64_t;
  #endif
  #ifndef __cplusplus
    #include <wchar.h>
  #endif
  typedef wchar_t char16_t;
  #define static_assert HSP_STATIC_ASSERT
#else
  #ifdef __cplusplus
    #include <cstdint>
    #ifdef USE_UCHAR_H
      #include <cuchar> // MinGW (GCC 4.8.1) is nothing Header file
    #endif
  #else
    #include <stdint.h>
    #ifdef USE_UCHAR_H
      #include <uchar.h> // MinGW (GCC 4.8.1) is nothing Header file
    #else
      #include <wchar.h>
      typedef wchar_t char16_t;
    #endif
    #if (__STDC_VERSION__ >= 201112L) // C11
      //#include <assert.h> // static_assert macro // MinGW (GCC 4.8.1) is not defined static_assert macro
      #define static_assert _Static_assert
    #else
      #define static_assert HSP_STATIC_ASSERT
    #endif
  #endif
#endif
#ifdef __cplusplus
  #include <cstddef>
  #if (__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__) || (_MSC_VER >= 1700) // C++11
    #define HSP_ENUMCLASS enum class
    #ifdef _MSC_VER
      #pragma warning(disable:4480) // legitimate C++11 // specifying underlying type for enum.
    #endif
  #else
    #define HSP_ENUMCLASS enum
    #if defined(_MSC_VER) && (_MSC_VER < 1600)
      const class nullptr_t
      {
        public:
          template<class T> inline operator T*() const {return 0;}
          template<class C, class T> inline operator T C::*() const {return 0;}
        private:
          void operator&() const;
      } nullptr = {};
    #endif
  #endif
  #if defined(_MSC_VER) && (_MSC_VER < 1700)
    #pragma warning(disable:4482) // legitimate C++11 // enum 'enum' used in qualified name.
    #pragma warning(disable:4481) // legitimate C++11 // override specifier 'keyword'
  #endif
#else // __cplusplus
  #include <stddef.h>
  #if (__STDC_VERSION__ >= 199901L) || (__GNUC__ >= 3) || (_MSC_VER >=1800) // C99
    #include <stdbool.h>
  #else
    typedef uint8_t _Bool;
    #define bool _Bool
    #define false 0
    #define true 1
    #define __bool_true_false_are_defined 1
  #endif
#endif
#ifndef RSIZE_MAX
  #define RSIZE_MAX SIZE_MAX
  typedef size_t rsize_t;
#endif
#if defined(USE_UCHAR_H) && !(defined(__STD_UTF_16__) || defined(__STDC_UTF_16__))
  #error "#ifndef __STD_UTF_16__"
#endif

#define HSP_CAT(a,b) HSP_CAT_(a,b)
//*
#define HSP_CAT_(a,b) a##b
/*/
#define HSP_CAT_(a,b) HSP_CAT__(a##b)
#define HSP_CAT__(s) s
//*/
#define HSP_STATIC_ASSERT(x, b) typedef char HSP_CAT(hspStatic_assert_,__COUNTER__)[(x)?1:-1]

static_assert(sizeof(bool) == 1, "sizeof(bool) != 1");
#if defined(_MSC_VER) && (_MSC_VER < 1600) || !defined(USE_UCHAR_H)
static_assert(sizeof(wchar_t) == 2, "sizeof(wchar_t) != 2");
#endif

#define HSPCALL __stdcall

#ifdef _MSC_VER
  #define HSPINLINE __inline
#else
  #define HSPINLINE inline
#endif

/*
#define HSPCHAR wchar_t
#define APICHAR wchar_t
#define U16SL(UTF16LEliteral) (L##UTF16LEliteral)
/*/
#define HSPCHAR char16_t
#define APICHAR wchar_t
#if defined(_MSC_VER) || (!defined(__cplusplus) && (__STDC_VERSION__ < 201112L))
  #define U16SL(UTF16LEliteral) L##UTF16LEliteral
  #define U16SLAC(UTF16LEliteral) U16SL(UTF16LEliteral)
#else
  #define U16SL(UTF16LEliteral) u##UTF16LEliteral
  #define U16SLAC(UTF16LEliteral) (const APICHAR*)(U16SL(UTF16LEliteral))
#endif
//*/

#ifndef STRICT
  #define STRICT // <windows.h>
#endif
#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN // <windows.h>
#endif

#endif // __HSPSDK_BASE_H
