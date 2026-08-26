# 0 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/src/null.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/src/null.cpp"
# 1 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/include/yaml-cpp/null.h" 1






       


# 1 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/include/yaml-cpp/dll.h" 1
# 11 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/include/yaml-cpp/null.h" 2
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstddef" 1 3
# 51 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstddef" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 1 3
# 37 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wc++23-extensions"
# 342 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
namespace std
{
  typedef unsigned int size_t;
  typedef int ptrdiff_t;


  typedef decltype(nullptr) nullptr_t;


#pragma GCC visibility push(default)


  extern "C++" __attribute__ ((__noreturn__, __always_inline__))
  inline void __terminate() noexcept
  {
    void terminate() noexcept __attribute__ ((__noreturn__,__cold__));
    terminate();
  }
#pragma GCC visibility pop
}
# 375 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
namespace std
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
namespace __gnu_cxx
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
# 579 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
namespace std
{
#pragma GCC visibility push(default)




  __attribute__((__always_inline__))
  constexpr inline bool
  __is_constant_evaluated() noexcept
  {





    return __builtin_is_constant_evaluated();



  }
#pragma GCC visibility pop
}
# 654 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
namespace std
{
  __attribute__((__always_inline__,__visibility__("default")))
  inline void
  __glibcxx_assert_fail()
  { }
}
# 733 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/os_defines.h" 1 3
# 734 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 2 3


# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/cpu_defines.h" 1 3
# 737 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 2 3
# 962 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/pstl/pstl_config.h" 1 3
# 963 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/powerpc-eabi/bits/c++config.h" 2 3



#pragma GCC diagnostic pop
# 52 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstddef" 2 3
# 1 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 1 3
# 160 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 3
typedef int ptrdiff_t;
# 229 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 3
typedef unsigned int size_t;
# 440 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 3
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 451 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 3
} max_align_t;






  typedef decltype(nullptr) nullptr_t;
# 53 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstddef" 2 3


# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/bits/version.h" 1 3
# 56 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstddef" 2 3

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

extern "C++"
{

namespace std
{

  using ::max_align_t;
}



namespace std
{


  enum class byte : unsigned char {};

  template<typename _IntegerType> struct __byte_operand { };
  template<> struct __byte_operand<bool> { using __type = byte; };
  template<> struct __byte_operand<char> { using __type = byte; };
  template<> struct __byte_operand<signed char> { using __type = byte; };
  template<> struct __byte_operand<unsigned char> { using __type = byte; };
  template<> struct __byte_operand<wchar_t> { using __type = byte; };

  template<> struct __byte_operand<char8_t> { using __type = byte; };

  template<> struct __byte_operand<char16_t> { using __type = byte; };
  template<> struct __byte_operand<char32_t> { using __type = byte; };
  template<> struct __byte_operand<short> { using __type = byte; };
  template<> struct __byte_operand<unsigned short> { using __type = byte; };
  template<> struct __byte_operand<int> { using __type = byte; };
  template<> struct __byte_operand<unsigned int> { using __type = byte; };
  template<> struct __byte_operand<long> { using __type = byte; };
  template<> struct __byte_operand<unsigned long> { using __type = byte; };
  template<> struct __byte_operand<long long> { using __type = byte; };
  template<> struct __byte_operand<unsigned long long> { using __type = byte; };
# 114 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstddef" 3
  template<typename _IntegerType>
    struct __byte_operand<const _IntegerType>
    : __byte_operand<_IntegerType> { };
  template<typename _IntegerType>
    struct __byte_operand<volatile _IntegerType>
    : __byte_operand<_IntegerType> { };
  template<typename _IntegerType>
    struct __byte_operand<const volatile _IntegerType>
    : __byte_operand<_IntegerType> { };

  template<typename _IntegerType>
    using __byte_op_t = typename __byte_operand<_IntegerType>::__type;

  template<typename _IntegerType>
    [[__gnu__::__always_inline__]]
    constexpr __byte_op_t<_IntegerType>
    operator<<(byte __b, _IntegerType __shift) noexcept
    { return (byte)(unsigned char)((unsigned)__b << __shift); }

  template<typename _IntegerType>
    [[__gnu__::__always_inline__]]
    constexpr __byte_op_t<_IntegerType>
    operator>>(byte __b, _IntegerType __shift) noexcept
    { return (byte)(unsigned char)((unsigned)__b >> __shift); }

  [[__gnu__::__always_inline__]]
  constexpr byte
  operator|(byte __l, byte __r) noexcept
  { return (byte)(unsigned char)((unsigned)__l | (unsigned)__r); }

  [[__gnu__::__always_inline__]]
  constexpr byte
  operator&(byte __l, byte __r) noexcept
  { return (byte)(unsigned char)((unsigned)__l & (unsigned)__r); }

  [[__gnu__::__always_inline__]]
  constexpr byte
  operator^(byte __l, byte __r) noexcept
  { return (byte)(unsigned char)((unsigned)__l ^ (unsigned)__r); }

  [[__gnu__::__always_inline__]]
  constexpr byte
  operator~(byte __b) noexcept
  { return (byte)(unsigned char)~(unsigned)__b; }

  template<typename _IntegerType>
    [[__gnu__::__always_inline__]]
    constexpr __byte_op_t<_IntegerType>&
    operator<<=(byte& __b, _IntegerType __shift) noexcept
    { return __b = __b << __shift; }

  template<typename _IntegerType>
    [[__gnu__::__always_inline__]]
    constexpr __byte_op_t<_IntegerType>&
    operator>>=(byte& __b, _IntegerType __shift) noexcept
    { return __b = __b >> __shift; }

  [[__gnu__::__always_inline__]]
  constexpr byte&
  operator|=(byte& __l, byte __r) noexcept
  { return __l = __l | __r; }

  [[__gnu__::__always_inline__]]
  constexpr byte&
  operator&=(byte& __l, byte __r) noexcept
  { return __l = __l & __r; }

  [[__gnu__::__always_inline__]]
  constexpr byte&
  operator^=(byte& __l, byte __r) noexcept
  { return __l = __l ^ __r; }

  template<typename _IntegerType>
    [[nodiscard,__gnu__::__always_inline__]]
    constexpr _IntegerType
    to_integer(__byte_op_t<_IntegerType> __b) noexcept
    { return _IntegerType(__b); }


}

}

#pragma GCC diagnostic pop
# 12 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/include/yaml-cpp/null.h" 2


# 13 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/include/yaml-cpp/null.h"
namespace YAML {
class Node;

struct _Null {};
inline bool operator==(const _Null&, const _Null&) { return true; }
inline bool operator!=(const _Null&, const _Null&) { return false; }

 bool IsNull(const Node& node);
 bool IsNullString(const char* str, std::size_t size);

extern _Null Null;
}
# 2 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/src/null.cpp" 2
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstring" 1 3
# 47 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstring" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/bits/version.h" 1 3
# 48 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstring" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 1 3
# 10 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/_ansi.h" 1 3
# 10 "C:/devkitPro/devkitPPC/powerpc-eabi/include/_ansi.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/newlib.h" 1 3
# 10 "C:/devkitPro/devkitPPC/powerpc-eabi/include/newlib.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/_newlib_version.h" 1 3
# 11 "C:/devkitPro/devkitPPC/powerpc-eabi/include/newlib.h" 2 3
# 11 "C:/devkitPro/devkitPPC/powerpc-eabi/include/_ansi.h" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/config.h" 1 3



# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/ieeefp.h" 1 3
# 5 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/config.h" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/features.h" 1 3
# 25 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/features.h" 3

# 25 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/features.h" 3
extern "C" {
# 594 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/features.h" 3
}
# 6 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/config.h" 2 3
# 12 "C:/devkitPro/devkitPPC/powerpc-eabi/include/_ansi.h" 2 3
# 11 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 1 3
# 9 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
extern "C" {



# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/_ansi.h" 1 3
# 14 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 2 3
# 1 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 1 3
# 15 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/cdefs.h" 1 3
# 45 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/cdefs.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 1 3
# 37 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
extern "C" {



typedef signed char __int8_t;

typedef unsigned char __uint8_t;
# 55 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef short int __int16_t;

typedef short unsigned int __uint16_t;
# 77 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef int __int32_t;

typedef unsigned int __uint32_t;
# 103 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef long long int __int64_t;

typedef long long unsigned int __uint64_t;
# 134 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef signed char __int_least8_t;

typedef unsigned char __uint_least8_t;
# 160 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef short int __int_least16_t;

typedef short unsigned int __uint_least16_t;
# 182 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef int __int_least32_t;

typedef unsigned int __uint_least32_t;
# 200 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef long long int __int_least64_t;

typedef long long unsigned int __uint_least64_t;
# 214 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
typedef long long int __intmax_t;







typedef long long unsigned int __uintmax_t;







typedef int __intptr_t;

typedef unsigned int __uintptr_t;
# 247 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_default_types.h" 3
}
# 46 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/cdefs.h" 2 3

# 1 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 1 3
# 48 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/cdefs.h" 2 3
# 16 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 1 3
# 24 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 3
# 1 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 1 3
# 374 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 3
typedef unsigned int wint_t;
# 25 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 2 3


# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/machine/_types.h" 1 3






typedef __int64_t _off_t;


typedef __int64_t _fpos_t;


typedef __uint32_t __ino_t;


typedef __uint32_t __dev_t;
# 28 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 2 3


typedef long __blkcnt_t;



typedef long __blksize_t;



typedef __uint64_t __fsblkcnt_t;



typedef __uint32_t __fsfilcnt_t;
# 52 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 3
typedef int __pid_t;







typedef unsigned short __uid_t;


typedef unsigned short __gid_t;



typedef __uint32_t __id_t;
# 90 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 3
typedef __uint32_t __mode_t;





__extension__ typedef long long _off64_t;





typedef _off_t __off_t;


typedef _off64_t __loff_t;


typedef long __key_t;
# 131 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 3
typedef unsigned int __size_t;
# 147 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 3
typedef signed int _ssize_t;
# 158 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_types.h" 3
typedef _ssize_t __ssize_t;



typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    unsigned char __wchb[4];
  } __value;
} _mbstate_t;




typedef void *_iconv_t;






typedef unsigned long __clock_t;






typedef __int_least64_t __time_t;





typedef unsigned long __clockid_t;


typedef long __daddr_t;



typedef unsigned long __timer_t;


typedef __uint8_t __sa_family_t;



typedef __uint32_t __socklen_t;


typedef int __nl_item;
typedef unsigned short __nlink_t;
typedef long __suseconds_t;
typedef unsigned long __useconds_t;







typedef __builtin_va_list __va_list;
# 17 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 2 3






typedef unsigned long __ULong;
# 35 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/lock.h" 1 3




# 1 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stdint.h" 1 3
# 9 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stdint.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 1 3
# 13 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_intsup.h" 1 3
# 35 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_intsup.h" 3
       
       
       
       
       
       
       
       
# 190 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_intsup.h" 3
       
       
       
       
       
       
       
       
# 14 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 2 3
# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_stdint.h" 1 3
# 15 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_stdint.h" 3
extern "C" {




typedef __int8_t int8_t ;



typedef __uint8_t uint8_t ;







typedef __int16_t int16_t ;



typedef __uint16_t uint16_t ;







typedef __int32_t int32_t ;



typedef __uint32_t uint32_t ;







typedef __int64_t int64_t ;



typedef __uint64_t uint64_t ;






typedef __intmax_t intmax_t;




typedef __uintmax_t uintmax_t;




typedef __intptr_t intptr_t;




typedef __uintptr_t uintptr_t;




}
# 15 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 2 3


extern "C" {



typedef __int_least8_t int_least8_t;
typedef __uint_least8_t uint_least8_t;




typedef __int_least16_t int_least16_t;
typedef __uint_least16_t uint_least16_t;




typedef __int_least32_t int_least32_t;
typedef __uint_least32_t uint_least32_t;




typedef __int_least64_t int_least64_t;
typedef __uint_least64_t uint_least64_t;
# 51 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 3
  typedef int int_fast8_t;
  typedef unsigned int uint_fast8_t;
# 61 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 3
  typedef int int_fast16_t;
  typedef unsigned int uint_fast16_t;
# 71 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 3
  typedef int int_fast32_t;
  typedef unsigned int uint_fast32_t;
# 81 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 3
  typedef long long int int_fast64_t;
  typedef long long unsigned int uint_fast64_t;
# 463 "C:/devkitPro/devkitPPC/powerpc-eabi/include/stdint.h" 3
}
# 12 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stdint.h" 2 3
#pragma GCC diagnostic pop
# 6 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/lock.h" 2 3

typedef uint32_t _LOCK_T;

struct __lock_t {
 _LOCK_T lock;
 uint32_t counter;
};

typedef struct __lock_t _LOCK_RECURSIVE_T;

typedef uint32_t _COND_T;






extern "C" {


static inline void __libc_lock_init(_LOCK_T *lock) {
        *lock = ((_LOCK_T)0);
}

static inline void __libc_lock_init_recursive(_LOCK_RECURSIVE_T *lock) {
        *lock = ((_LOCK_RECURSIVE_T){((_LOCK_T)0),0});
}

extern void __libc_lock_close(_LOCK_T *lock );
extern void __libc_lock_close_recursive(_LOCK_RECURSIVE_T *lock );

extern int __libc_lock(_LOCK_T *lock);
extern int __libc_lock_recursive(_LOCK_RECURSIVE_T *lock);

extern void __libc_lock_acquire(_LOCK_T *lock);
extern void __libc_lock_acquire_recursive(_LOCK_RECURSIVE_T *lock);
extern void __libc_lock_release(_LOCK_T *lock);
extern void __libc_lock_release_recursive(_LOCK_RECURSIVE_T *lock);


extern int __libc_lock_try_acquire(_LOCK_T *lock);
extern int __libc_lock_try_acquire_recursive(_LOCK_RECURSIVE_T *lock);


static inline int __libc_cond_init(_COND_T *cond) {
        *cond = ((_COND_T)0);
}

extern void __libc_cond_close(_COND_T *cond);
extern int __libc_cond_signal(_COND_T *cond);
extern int __libc_cond_broadcast(_COND_T *cond);
extern int __libc_cond_wait(_COND_T *cond, _LOCK_T *lock, uint64_t timeout_ns);
extern int __libc_cond_wait_recursive(_COND_T *cond, _LOCK_RECURSIVE_T *lock, uint64_t timeout_ns);


}
# 36 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 2 3
typedef _LOCK_RECURSIVE_T _flock_t;







struct _reent;

struct __locale_t;






struct _Bigint
{
  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  __ULong _x[1];
};


struct __tm
{
  int __tm_sec;
  int __tm_min;
  int __tm_hour;
  int __tm_mday;
  int __tm_mon;
  int __tm_year;
  int __tm_wday;
  int __tm_yday;
  int __tm_isdst;
};







struct _on_exit_args {
 void * _fnargs[32];
 void * _dso_handle[32];

 __ULong _fntypes;


 __ULong _is_cxa;
};
# 99 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
struct _atexit {
 struct _atexit *_next;
 int _ind;

 void (*_fns[32])(void);
        struct _on_exit_args _on_exit_args;
};
# 116 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 153 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
struct __sFILE {
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;






  void * _cookie;

  _ssize_t (*_read) (struct _reent *, void *,
        char *, size_t);
  _ssize_t (*_write) (struct _reent *, void *,
         const char *,
         size_t);
  _fpos_t (*_seek) (struct _reent *, void *, _fpos_t, int);
  int (*_close) (struct _reent *, void *);


  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;


  unsigned char _ubuf[3];
  unsigned char _nbuf[1];


  struct __sbuf _lb;


  int _blksize;
  _off_t _offset;


  struct _reent *_data;



  _flock_t _lock;

  _mbstate_t _mbstate;
  int _flags2;
};
# 270 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
typedef struct __sFILE __FILE;



extern __FILE __sf[3];

struct _glue
{
  struct _glue *_next;
  int _niobs;
  __FILE *_iobs;
};

extern struct _glue __sglue;
# 306 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
struct _rand48 {
  unsigned short _seed[3];
  unsigned short _mult[3];
  unsigned short _add;




};
# 582 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
struct _reent
{
  int _errno;




  __FILE *_stdin, *_stdout, *_stderr;

  int _inc;
  char _emergency[25];




  struct __locale_t *_locale;





  void (*__cleanup) (struct _reent *);


  struct _Bigint *_result;
  int _result_k;
  struct _Bigint *_p5s;
  struct _Bigint **_freelist;


  int _cvtlen;
  char *_cvtbuf;

  union
    {
      struct
        {



          char * _strtok_last;
          char _asctime_buf[26];
          struct __tm _localtime_buf;
          int _gamma_signgam;
          __extension__ unsigned long long _rand_next;
          struct _rand48 _r48;
          _mbstate_t _mblen_state;
          _mbstate_t _mbtowc_state;
          _mbstate_t _wctomb_state;
          char _l64a_buf[8];
          char _signal_buf[24];
          int _getdate_err;
          _mbstate_t _mbrlen_state;
          _mbstate_t _mbrtowc_state;
          _mbstate_t _mbsrtowcs_state;
          _mbstate_t _wcrtomb_state;
          _mbstate_t _wcsrtombs_state;
   int _h_errno;
# 652 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
   char _getlocalename_l_buf[7
        * 32 ];

        } _reent;







    } _new;







  void (**_sig_func)(int);

  void *deviceData;
};
# 809 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
extern struct _reent *_impure_ptr __attribute__((__section__(".sdata")));





extern struct _reent _impure_data ;





  struct _reent * __getreent (void);
# 929 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/reent.h" 3
extern struct _atexit *__atexit;
extern struct _atexit __atexit0;

extern void (*__stdio_exit_handler) (void);

void _reclaim_reent (struct _reent *);

extern int _fwalk_sglue (struct _reent *, int (*)(struct _reent *, __FILE *),
    struct _glue *);


}
# 12 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 2 3





# 1 "C:/devkitPro/devkitPPC/lib/gcc/powerpc-eabi/16.1.0/include/stddef.h" 1 3
# 18 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 2 3


# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_locale.h" 1 3
# 9 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/_locale.h" 3
struct __locale_t;
typedef struct __locale_t *locale_t;
# 21 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 2 3



# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/strings.h" 1 3
# 44 "C:/devkitPro/devkitPPC/powerpc-eabi/include/strings.h" 3
extern "C" {

int bcmp(const void *, const void *, size_t) __attribute__((__pure__));
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);


void explicit_bzero(void *, size_t);


int ffs(int) __attribute__((__const__));


int ffsl(long) __attribute__((__const__));
int ffsll(long long) __attribute__((__const__));
int fls(int) __attribute__((__const__));
int flsl(long) __attribute__((__const__));
int flsll(long long) __attribute__((__const__));


char *index(const char *, int) __attribute__((__pure__));
char *rindex(const char *, int) __attribute__((__pure__));

int strcasecmp(const char *, const char *) __attribute__((__pure__));
int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));


int strcasecmp_l (const char *, const char *, locale_t);
int strncasecmp_l (const char *, const char *, size_t, locale_t);

}
# 25 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 2 3


extern "C" {

void * memchr (const void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void * memcpy (void *__restrict, const void *__restrict, size_t);
void * memmove (void *, const void *, size_t);
void * memset (void *, int, size_t);
char *strcat (char *__restrict, const char *__restrict);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
int strcoll (const char *, const char *);
char *strcpy (char *__restrict, const char *__restrict);
size_t strcspn (const char *, const char *);
char *strerror (int);
size_t strlen (const char *);
char *strncat (char *__restrict, const char *__restrict, size_t);
int strncmp (const char *, const char *, size_t);
char *strncpy (char *__restrict, const char *__restrict, size_t);
char *strpbrk (const char *, const char *);
char *strrchr (const char *, int);
size_t strspn (const char *, const char *);
char *strstr (const char *, const char *);

char *strtok (char *__restrict, const char *__restrict);

size_t strxfrm (char *__restrict, const char *__restrict, size_t);


int strcoll_l (const char *, const char *, locale_t);
char *strerror_l (int, locale_t);
size_t strxfrm_l (char *__restrict, const char *__restrict, size_t, locale_t);


char *strtok_r (char *__restrict, const char *__restrict, char **__restrict);


int timingsafe_bcmp (const void *, const void *, size_t);
int timingsafe_memcmp (const void *, const void *, size_t);


void * memccpy (void *__restrict, const void *__restrict, int, size_t);


void * mempcpy (void *, const void *, size_t);
void * memrchr (const void *, int, size_t);
void * rawmemchr (const void *, int);


void * memmem (const void *, size_t, const void *, size_t);


char *stpcpy (char *__restrict, const char *__restrict);
char *stpncpy (char *__restrict, const char *__restrict, size_t);


char *strcasestr (const char *, const char *);
char *strchrnul (const char *, int);


char *strdup (const char *) __attribute__((__malloc__)) __attribute__((__warn_unused_result__));

char *_strdup_r (struct _reent *, const char *);

char *strndup (const char *, size_t) __attribute__((__malloc__)) __attribute__((__warn_unused_result__));

char *_strndup_r (struct _reent *, const char *, size_t);






char *strerror_r (int, char *, size_t);
# 114 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 3
char * _strerror_r (struct _reent *, int, int, int *);


size_t strlcat (char *, const char *, size_t);
size_t strlcpy (char *, const char *, size_t);


size_t strnlen (const char *, size_t);


char *strsep (char **, const char *);


char *strnstr(const char *, const char *, size_t) __attribute__((__pure__));



char *strlwr (char *);
char *strupr (char *);



char *strsignal (int __signo);







int strverscmp (const char *, const char *);
# 174 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 3
char *__attribute__((__nonnull__ (1))) basename (const char *) __asm__("" "__gnu_basename");


# 1 "C:/devkitPro/devkitPPC/powerpc-eabi/include/sys/string.h" 1 3
# 178 "C:/devkitPro/devkitPPC/powerpc-eabi/include/string.h" 2 3

}
# 49 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstring" 2 3
# 74 "C:/devkitPro/devkitPPC/powerpc-eabi/include/c++/16.1.0/cstring" 3
extern "C++"
{
namespace std __attribute__ ((__visibility__ ("default")))
{


  using ::memchr;
  using ::memcmp;
  using ::memcpy;
  using ::memmove;
  using ::memset;
  using ::strcat;
  using ::strcmp;
  using ::strcoll;
  using ::strcpy;
  using ::strcspn;
  using ::strerror;
  using ::strlen;
  using ::strncat;
  using ::strncmp;
  using ::strncpy;
  using ::strspn;

  using ::strtok;

  using ::strxfrm;
  using ::strchr;
  using ::strpbrk;
  using ::strrchr;
  using ::strstr;


  inline void*
  memchr(void* __s, int __c, size_t __n)
  { return __builtin_memchr(__s, __c, __n); }

  inline char*
  strchr(char* __s, int __n)
  { return __builtin_strchr(__s, __n); }

  inline char*
  strpbrk(char* __s1, const char* __s2)
  { return __builtin_strpbrk(__s1, __s2); }

  inline char*
  strrchr(char* __s, int __n)
  { return __builtin_strrchr(__s, __n); }

  inline char*
  strstr(char* __s1, const char* __s2)
  { return __builtin_strstr(__s1, __s2); }



}
}
# 3 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/src/null.cpp" 2


# 4 "C:/claude/bk-wiiu/Lighthouse/build-wiiu/_deps/yaml-cpp-src/src/null.cpp"
namespace YAML {
_Null Null;

template <std::size_t N>
static bool same(const char* str, std::size_t size, const char (&literal)[N]) {
  constexpr int literalSize = N - 1;
  return size == literalSize && std::strncmp(str, literal, literalSize) == 0;
}

bool IsNullString(const char* str, std::size_t size) {
  return size == 0 || same(str, size, "~") || same(str, size, "null") ||
         same(str, size, "Null") || same(str, size, "NULL");
}
}
