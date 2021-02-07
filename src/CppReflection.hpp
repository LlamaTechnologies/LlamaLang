#pragma once
#include <string>
#include "SupportedCompilers.hpp"

#ifdef LLANG_VISUALSTUDIO
#define __PRETTY_FUNCTION__  __FUNCSIG__
#endif

template <typename E, E V> constexpr std::string GetEnumValueName() {
  std::string name =  __PRETTY_FUNCTION__;
  auto i = name.find_last_of(',') + 1;
  
  if (name[i] >= '0' && name[i] <= '9') {
    return "";
  }

  auto j = name.find_first_of('>', i);
  std::string enumValue = name.substr(i, j - i);
  return enumValue;
}


template <typename E, E V> constexpr std::string GetEnumClassValueName() {
  std::string name =  __PRETTY_FUNCTION__;
  auto i = name.find_last_of(':') + 1;
  
  if (name[i] >= '0' && name[i] <= '9') {
    return "";
  }

  auto j = name.find_first_of('>', i);
  std::string enumValue = name.substr(i, j - i);
  return enumValue;
}

template <typename E> constexpr std::string GetTypeName() {
  std::string name =  __PRETTY_FUNCTION__;
  auto i = name.find_last_of(' ') + 1;
  
  if (name[i] >= '0' && name[i] <= '9') {
    return "";
  }

  auto j = name.find_first_of('>', i);
  std::string enumValue = name.substr(i, j - i);
  return enumValue;
}

template <typename E, E V> constexpr void AssertEnumValueGraterThan(E correctValue, const char *msg) {
  static_assert (!(V > correctValue), msg);
}

template <typename E, E V> constexpr void AssertEnumValueLessThan(E correctValue, const char *msg) {
  static_assert (!(V < correctValue), msg);
}