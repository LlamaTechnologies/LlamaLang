#pragma once

/* gb.h - v0.33  - Ginger Bill's C Helper Library - public domain
                 - no warranty implied; use at your own risk

  This is a single header file with a bunch of useful stuff
  to replace the C/C++ standard library

  IMPORTANT NOTE: THIS IS A HEAVILY MODIFIED VERSION OF THE ORIGINAL
  DO NO REPLACE IT WITH THE ORIGINAL
*/

#if !defined(GB_NO_DEFER) && defined(__cplusplus) && \
  ((defined(_MSC_VER) && _MSC_VER >= 1400) || (__cplusplus >= 201103L))
extern "C++" {
  // NOTE(bill): Stupid fucking templates
  template<typename T>
  struct gbRemoveReference {
    typedef T Type;
  };
  template<typename T>
  struct gbRemoveReference<T &> {
    typedef T Type;
  };
  template<typename T>
  struct gbRemoveReference<T &&> {
    typedef T Type;
  };

  /// NOTE(bill): "Move" semantics - invented because the C++ committee are idiots (as a collective not as indiviuals
  /// (well a least some aren't))
  template<typename T>
  inline T &&gb_forward(typename gbRemoveReference<T>::Type &t) {
    return static_cast<T &&>(t);
  }

  template<typename T>
  inline T &&gb_forward(typename gbRemoveReference<T>::Type &&t) {
    return static_cast<T &&>(t);
  }

  template<typename F>
  struct Deferer {
    F f;
    Deferer(F &&f) : f(gb_forward<F>(f)) {}
    ~Deferer() { f(); }
  };

  template<typename F>
  Deferer<F> defer_func(F &&f) {
    return Deferer<F>(gb_forward<F>(f));
  }

  #define DEFER_CONCAT_STRINGS(x, y) x##y
  #define DEFER_NAME_CONCAT(x, y) DEFER_CONCAT_STRINGS(x, y)
  #define DEFER_NAME(x) DEFER_NAME_CONCAT(x, __COUNTER__)
  #define defer(code) auto DEFER_NAME(_defer_) = defer_func([&]() -> void { code; })
}
#endif
