//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef LIBCXX_TEST_SUPPORT_LOCALE_HELPERS_H
#define LIBCXX_TEST_SUPPORT_LOCALE_HELPERS_H

#include <string>
#include "platform_support.h"
#include "test_macros.h"
#include "make_string.h"

#ifndef TEST_HAS_NO_WIDE_CHARACTERS

#include <cwctype>

#endif // TEST_HAS_NO_WIDE_CHARACTERS

namespace LocaleHelpers {

#ifndef TEST_HAS_NO_WIDE_CHARACTERS

std::wstring convert_thousands_sep(std::wstring const& in, wchar_t sep) {
  std::wstring out;
  bool seen_num_start = false;
  bool seen_decimal = false;
  for (unsigned i = 0; i < in.size(); ++i) {
    seen_decimal |= in[i] == L',';
    seen_num_start |= in[i] == L'-' || std::iswdigit(in[i]);
    if (seen_decimal || !seen_num_start || in[i] != L' ') {
      out.push_back(in[i]);
      continue;
    }
    assert(in[i] == L' ');
    out.push_back(sep);
  }
  return out;
}

// GLIBC 2.27 and newer use U+202F NARROW NO-BREAK SPACE as a thousands separator.
// This function converts the spaces in string inputs to U+202F if need
// be. FreeBSD's locale data also uses U+202F, since 2018.
// Windows uses U+00A0 NO-BREAK SPACE.
std::wstring convert_thousands_sep_fr_FR(std::wstring const& in) {
#if defined(_CS_GNU_LIBC_VERSION)
  if (glibc_version_less_than("2.27"))
    return in;
  else
    return convert_thousands_sep(in, L'\u202F');
#elif defined(__FreeBSD__)
  return convert_thousands_sep(in, L'\u202F');
#elif defined(_WIN32)
  return convert_thousands_sep(in, L'\u00A0');
#else
  return in;
#endif
}

// GLIBC 2.27 uses U+202F NARROW NO-BREAK SPACE as a thousands separator.
// FreeBSD and Windows use U+00A0 NO-BREAK SPACE.
std::wstring convert_thousands_sep_ru_RU(std::wstring const& in) {
#if defined(TEST_HAS_GLIBC)
  return convert_thousands_sep(in, L'\u202F');
#elif defined(__FreeBSD__) || defined(_WIN32)
  return convert_thousands_sep(in, L'\u00A0');
#else
  return in;
#endif
}

#endif // TEST_HAS_NO_WIDE_CHARACTERS

MultiStringType currency_symbol_ru_RU() {
#if defined(_CS_GNU_LIBC_VERSION)
  if (glibc_version_less_than("2.24"))
    return MKSTR("\u0440\u0443\u0431");
  else
    return MKSTR("\u20BD"); // U+20BD RUBLE SIGN
#elif defined(_WIN32) || defined(__FreeBSD__)
  return MKSTR("\u20BD"); // U+20BD RUBLE SIGN
#else
  return MKSTR("\u0440\u0443\u0431.");
#endif
}

} // namespace LocaleHelpers

#endif // LIBCXX_TEST_SUPPORT_LOCALE_HELPERS_H
