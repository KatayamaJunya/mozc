// Copyright 2010-2021, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "converter/key_corrector.h"

#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>

#include "absl/log/check.h"
#include "absl/log/log.h"
#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "base/util.h"
#include "base/vlog.h"

namespace mozc {
namespace {
/*
bool RewriteToLongVowel(char32_t target, const char *begin, const char *end,
                        size_t *mblen, std::string *output) {
  
  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != target) {  // "１文字目"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  if (next_codepoint == 0x3042){ // "あ"
    output_codepoint = 0x30FC; // "ー"
  } else {
    output_codepoint = 0x0000;
  }

  if (output_codepoint != 0x0000) {  // "あー"
    Util::CodepointToUtf8Append(codepoint, output);
    Util::CodepointToUtf8Append(output_codepoint, output);
    *mblen += mblen2;
    return true;
  } else {  // others
    *mblen = 0;
    return false;
  }

  return false;
}

// "ああ" pattern
// "ああ" -> "あー"
bool RewriteAA(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {
  return RewriteToLongVowel(0x3042, begin, end, mblen, output);  // 'あ'
}

// "かあ" pattern
// "かあ" -> "かー"
bool RewriteKAA(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {
  return RewriteToLongVowel(0x304B, begin, end, mblen, output);  // 'か'
}

// "で" pattern
// "で" -> "でぃ"
bool RewriteDE(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {

  // まず最初に、begin～endの範囲に「＠」（0xFF20）が含まれているかチェック
  for (const char *p = begin; p < end; ) {
    size_t tmp_mblen = 0;
    const char32_t c = Util::Utf8ToCodepoint(p, end, &tmp_mblen);
    if (c == 0xFF20 || c == 0x40) {  // "＠ or @"
      // もし「＠」があったら、この関数は変換しない
      *mblen = 0;
      return false;
    }
    p += tmp_mblen;  // 次の文字へ進む
  }

  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != 0x3067) {  // "で"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    // 「で」の後に何もないときは「でぃ」に変換して終了
    Util::CodepointToUtf8Append(codepoint, output);  // "で"
    Util::CodepointToUtf8Append(0x3043, output);  // "ぃ"
    return true;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  if (next_codepoint != 0x3043){ // "ぃ"
    output_codepoint = 0x3043;
  } else {
    output_codepoint = 0x0000;
  }

  if (output_codepoint != 0x0000) {  // "で[^ぃ]"
    Util::CodepointToUtf8Append(codepoint, output); // "で"
    Util::CodepointToUtf8Append(output_codepoint, output);  // "ぃ"
    return true;
  } else {  // others
    *mblen = 0;
    return false;
  }

  return false;
}

// "でい" pattern 1
// "でい" -> "でぃ"
bool RewriteDEI1(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {

  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != 0x3067) {  // "で"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  if (next_codepoint == 0x3044){ // "い"
    output_codepoint = 0x3043;  // "ぃ"
  } else {
    output_codepoint = 0x0000;
  }

  if (output_codepoint != 0x0000) {  // "でぃ"
    Util::CodepointToUtf8Append(codepoint, output); // "で"
    Util::CodepointToUtf8Append(output_codepoint, output);  // "ぃ"
    *mblen += mblen2;
    return true;
  } else {  // others
    *mblen = 0;
    return false;
  }

  return false;
}

// "でい" pattern 2
// "でい" -> "でぃー"
bool RewriteDEI2(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {
  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != 0x3067) {  // "で"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  if (next_codepoint == 0x3044){ // "い"
    output_codepoint = 0x30FC;  // "ー"
  } else {
    output_codepoint = 0x0000;
  }

  if (output_codepoint != 0x0000) {
    Util::CodepointToUtf8Append(0x3067, output);  // "で"
    Util::CodepointToUtf8Append(0x3043, output);  // "ぃ"
    Util::CodepointToUtf8Append(output_codepoint, output);  // "ー"
    *mblen += mblen2;
    return true;
  } else {
    *mblen = 0;
    return false;
  }

  return false;
}

// "へ" pattern
// "へ" -> "ふぇ"
bool RewriteHE(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {

  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != 0x3078) {  // "へ"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    // 「へ」の後に何もないときは「ふぇ」に変換して終了
    Util::CodepointToUtf8Append(0x3075, output);  // "ふ"
    Util::CodepointToUtf8Append(0x3047, output);  // "ぇ"
    return true;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  if (next_codepoint != 0x3041 && next_codepoint != 0x3043 &&
      next_codepoint != 0x3045 && next_codepoint != 0x3047 &&
      next_codepoint != 0x3049 && next_codepoint != 0x3083 &&
      next_codepoint != 0x3085 && next_codepoint != 0x3087 &&
      next_codepoint != 0x308E){ // "[^ぁぃぅぇぉゃゅょゎ]"
    output_codepoint = 0x3047;
  } else {
    output_codepoint = 0x0000;
  }

  if (output_codepoint != 0x0000) {  // "へ[^ぁぃぅぇぉゃゅょゎ]"
    Util::CodepointToUtf8Append(0x3075, output); // "ふ"
    Util::CodepointToUtf8Append(output_codepoint, output);  // "ぇ"
    return true;
  } else {  // others
    *mblen = 0;
    return false;
  }

  return false;
}


// "ん" (few "n" pattern)
// "んあ" -> "んな"
// "んい" -> "んに"
// "んう" -> "んぬ"
// "んえ" -> "んね"
// "んお" -> "んの"
bool RewriteNN(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {
  if (key_pos == 0) {
    *mblen = 0;
    return false;
  }

  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != 0x3093) {  // "ん"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  switch (next_codepoint) {
    case 0x3042:                  // "あ"
      output_codepoint = 0x306A;  // "な"
      break;
    case 0x3044:                  // "い"
      output_codepoint = 0x306B;  // "に"
      break;
    case 0x3046:                  // "う"
      output_codepoint = 0x306C;  // "ぬ"
      break;
    case 0x3048:                  // "え"
      output_codepoint = 0x306D;  // "ね"
      break;
    case 0x304A:                  // "お"
      output_codepoint = 0x306E;  // "の"
      break;
    default:
      break;
  }

  if (output_codepoint != 0x0000) {  // "ん[あいうえお]"
    Util::CodepointToUtf8Append(codepoint, output);
    Util::CodepointToUtf8Append(output_codepoint, output);
    *mblen += mblen2;
    return true;
  } else {  // others
    *mblen = 0;
    return false;
  }

  return false;
}

// "んん" (many "n" pattern)
// "([^ん])んん[ん]" -> ignore
// "([^ん])んん[あいうえお]" ->  $1 and leave "ん[あいうえお]"
// "([^ん])んん[^あいうえお]" -> $1"ん" and leave "[^あいうえお]"
bool RewriteDoubleNN(size_t key_pos, const char *begin, const char *end,
                     size_t *mblen, std::string *output) {
  // 0x3093: "ん"
  static const uint16_t kPattern[] = {0x0000, 0x3093, 0x3093};

  *mblen = 0;
  uint16_t first_char = 0x0000;
  size_t first_mblen = 0;
  for (size_t i = 0; i < std::size(kPattern); ++i) {
    if (begin >= end) {
      *mblen = 0;
      return false;
    }
    size_t mblen2 = 0;
    const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, &mblen2);
    if ((kPattern[i] == 0x0000 && codepoint != 0x3093 &&
         Util::GetScriptType(codepoint) == Util::HIRAGANA) ||
        (kPattern[i] == 0x3093 && codepoint == 0x3093)) {
      if (i == 0) {
        first_mblen = mblen2;
        first_char = codepoint;
      }
    } else {
      *mblen = 0;
      return false;
    }
    begin += mblen2;
    *mblen += mblen2;
  }

  if (begin >= end) {
    *mblen = 0;
    return false;
  }

  if (first_char == 0x0000 || first_mblen == 0) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, &mblen2);
  if (codepoint == 0x3093) {  // "ん" ignore
    *mblen = 0;
    return false;
  } else if (codepoint == 0x3042 ||  // "[あいうえお]"
             codepoint == 0x3044 || codepoint == 0x3046 ||
             codepoint == 0x3048 || codepoint == 0x304A) {
    // drop first "ん" and leave "ん[あいうえお]"
    // remained part will be handled by RewriteNN(), e.g., "んあ" -> "んな"
    Util::CodepointToUtf8Append(first_char, output);
    // Skip one Hiragana character in UTF-8, which is 3 bytes.
    *mblen = first_mblen + 3;
    return true;
  } else {  // "[^あいうえお]"
    Util::CodepointToUtf8Append(first_char, output);
    Util::CodepointToUtf8Append(0x3093, output);  // "ん"
    return true;
  }

  return false;
}

// "に" pattern
// "にゃ" -> "んや"
// "にゅ" -> "んゆ"
// "にょ" -> "んよ"
bool RewriteNI(size_t key_pos, const char *begin, const char *end,
               size_t *mblen, std::string *output) {
  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  if (codepoint != 0x306B) {  // "に"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  uint16_t output_codepoint = 0x0000;
  switch (next_codepoint) {
    case 0x3083:                  // "ゃ"
      output_codepoint = 0x3084;  // "や"
      break;
    case 0x3085:                  // "ゅ"
      output_codepoint = 0x3086;  // "ゆ"
      break;
    case 0x3087:                  // "ょ"
      output_codepoint = 0x3088;  // "よ"
      break;
    default:
      output_codepoint = 0x0000;
      break;
  }

  if (output_codepoint != 0x0000) {
    Util::CodepointToUtf8Append(0x3093, output);  // "ん"
    Util::CodepointToUtf8Append(output_codepoint, output);
    *mblen += mblen2;
    return true;
  } else {
    *mblen = 0;
    return false;
  }

  return false;
}

// "m" Pattern (not BOS)
// "m[ばびぶべぼぱぴぷぺぽ]" -> "ん[ばびぶべぼぱぴぷぺぽ]"
bool RewriteM(size_t key_pos, const char *begin, const char *end, size_t *mblen,
              std::string *output) {
  if (key_pos == 0) {
    *mblen = 0;
    return false;
  }
  const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, mblen);
  // "m" or "ｍ" (don't take capitial letter, as "M" might not be a misspelling)
  if (codepoint != 0x006D && codepoint != 0xFF4D) {
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) {
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const uint16_t next_codepoint =
      Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
  // "[はばぱひびぴふぶぷへべぺほぼぽ]" => [0x306F .. 0X307D]
  // Here we want to take "[は..ぽ]" except for "はひふへほ"
  if (next_codepoint % 3 != 0 &&  // not "はひふへほ"
      next_codepoint >= 0x306F && next_codepoint <= 0x307D) {  // "は..ぽ"
    Util::CodepointToUtf8Append(0x3093, output);               // "ん"
    Util::CodepointToUtf8Append(next_codepoint, output);
    *mblen += mblen2;
    return true;
  } else {
    *mblen = 0;
    return false;
  }

  return true;
}

// "きっって" ->" きって"
// replace "([^っ])っっ([^っ])" => "$1っ$2"
// Don't consider more that three "っっっ"
// e.g., "かっっった" -> "かっっった"
bool RewriteSmallTSU(size_t key_pos, const char *begin, const char *end,
                     size_t *mblen, std::string *output) {
  // 0x0000 is a place holder for "[^っ]"
  // "っ": 0x3063
  static const uint16_t kPattern[] = {0x0000, 0x3063, 0x3063, 0x0000};

  uint16_t first_char = 0x0000;
  uint16_t last_char = 0x0000;
  for (size_t i = 0; i < std::size(kPattern); ++i) {
    if (begin >= end) {
      *mblen = 0;
      return false;
    }
    size_t mblen2 = 0;
    const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, &mblen2);
    if ((kPattern[i] == 0x0000 && codepoint != 0x3063 &&
         Util::GetScriptType(codepoint) == Util::HIRAGANA) ||
        (kPattern[i] == 0x3063 && codepoint == 0x3063)) {
      if (i == 0) {
        first_char = codepoint;
      } else if (i == std::size(kPattern) - 1) {
        last_char = codepoint;
      }
    } else {
      *mblen = 0;
      return false;
    }
    begin += mblen2;
    *mblen += mblen2;
  }

  if (first_char == 0x0000 || last_char == 0x0000) {
    *mblen = 0;
    return false;
  }

  Util::CodepointToUtf8Append(first_char, output);
  Util::CodepointToUtf8Append(0x3063, output);  // "っ"
  Util::CodepointToUtf8Append(last_char, output);

  return true;
}

// Not implemented yet, as they looks minor
// "[子音][ゃゅょ][^う]" Pattern
// "きゅ[^う] -> きゅう"
// "しゅ[^う] -> しゅう"
// "ちゅ[^う] -> ちゅう"
// "にゅ[^う] -> にゅう"
// "ひゅ[^う] -> ひゅう"
// "りゅ[^う] -> りゅう"
bool RewriteYu(size_t key_pos, const char *begin, const char *end,
  size_t *mblen, std::string *output) {
const char32_t first_char = Util::Utf8ToCodepoint(begin, end, mblen);
if (first_char != 0x304D && first_char != 0x3057 && first_char != 0x3061 &&
first_char != 0x306B && first_char != 0x3072 &&
first_char != 0x308A) {  // !"きしちにひり"
*mblen = 0;
return false;
}

if (begin + *mblen >= end) {
*mblen = 0;
return false;
}

size_t mblen2 = 0;
const char32_t next_char =
Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);
if (next_char != 0x3085) {  // "ゅ"
*mblen = 0;
return false;
}

if (begin + *mblen + mblen2 >= end) {
*mblen = 0;
return false;
}

size_t mblen3 = 0;
const char32_t last_char =
Util::Utf8ToCodepoint(begin + *mblen + mblen2, end, &mblen3);
if (last_char == 0x3046) {  // "う"
*mblen = 0;
return false;
}

// OK, rewrite
*mblen += mblen2;
Util::CodepointToUtf8Append(first_char, output);
Util::CodepointToUtf8Append(next_char, output);  // "ゅ"
Util::CodepointToUtf8Append(0x3046, output);     // "う"

return true;
}
*/

// "￥◯◯￥" pattern（最大７文字）
bool RewriteSpecifiedWordYen(size_t key_pos, const char *begin, const char *end,
  size_t *mblen, std::string *output) {
  const char32_t first_char = Util::Utf8ToCodepoint(begin, end, mblen);
  if (first_char != 0xFFE5 && first_char != 0x5c) {  // !"￥\"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) { //１文字目で終わりのとき
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const char32_t second_char =
  Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);

  if (begin + *mblen + mblen2 >= end) { //２文字目で終わりのとき
    *mblen = 0;
    return false;
  }

  size_t mblen3 = 0;
  const char32_t third_char =
  Util::Utf8ToCodepoint(begin + *mblen + mblen2, end, &mblen3);

  uint16_t output_codepoint = 0x0000;
  bool is_hited_at_third = false;  // ３文字目で該当したかどうか
  if (third_char == 0xFFE5 || third_char == 0x5c) {  // "￥\"  ￥◯￥のとき
    switch (second_char) {
      case 0x3042:                  // "あ"
        output_codepoint = 0x3042;  // "あ"
        break;
      case 0x3044:                  // "い"
        output_codepoint = 0x3044;  // "い"
        break;
      case 0x304A:                  // "お"
        output_codepoint = 0x304A;  // "お"
        break;
      case 0x304B:                  // "か"
        output_codepoint = 0x304B;  // "か"
        break;
      case 0x304D:                  // "き"
        output_codepoint = 0x304D;  // "き"
        break;
      case 0x3053:                  // "こ"
        output_codepoint = 0x3053;  // "こ"
        break;
      case 0x3055:                  // "さ"
        output_codepoint = 0x3055;  // "さ"
        break;
      case 0x3058:                  // "じ"
        output_codepoint = 0x3058;  // "じ"
        break;
      case 0x3059:                  // "す"
        output_codepoint = 0x3059;  // "す"
        break;
      case 0x305A:                  // "ず"
        output_codepoint = 0x3058;  // "じ"
        break;
      case 0x3064:                  // "つ"
        output_codepoint = 0x3064;  // "つ"
        break;
      case 0x3069:                  // "ど"
        output_codepoint = 0x3069;  // "ど"
        break;
      case 0x306F:                  // "は"
        output_codepoint = 0x306F;  // "は"
        break;
      case 0x3072:                  // "ひ"
        output_codepoint = 0x3072;  // "ひ"
        break;
      case 0x3076:                  // "ぶ"
        output_codepoint = 0x3076;  // "ぶ"
        break;
      case 0x307B:                  // "ほ"
        output_codepoint = 0x307B;  // "ほ"
        break;
      case 0x3088:                  // "よ"
        output_codepoint = 0x3088;  // "よ"
        break;
      case 0x308F:                  // "わ"
        output_codepoint = 0x308F;  // "わ"
        break;
      default:
        output_codepoint = 0x0000;
        break;
    }

    if (output_codepoint != 0x0000) {
      Util::CodepointToUtf8Append(output_codepoint, output);  // "表記指定外した読み"
      if (begin + *mblen + mblen2 + mblen3 >= end) { //３文字目で終わりのとき
        *mblen += mblen2 + mblen3;
        return true;
      }
      is_hited_at_third = true;
    }

  }

  if (begin + *mblen + mblen2 + mblen3 >= end) { //３文字目で終わりのとき
    *mblen = 0;
    return false;
  }

  size_t mblen4 = 0;
  const char32_t fourth_char =
  Util::Utf8ToCodepoint(begin + *mblen + mblen2 + mblen3, end, &mblen4);

  uint16_t output_codepoint1 = 0x0000; //出力１文字目
  uint16_t output_codepoint2 = 0x0000; //出力２文字目
  bool is_hited_at_fourth = false;  // ４文字目で該当したかどうか
  if (fourth_char == 0xFFE5 || fourth_char == 0x5c) {  // "￥\"  ￥◯◯￥のとき
    switch (second_char) {
      case 0x3046:                  // "う"
        if (third_char == 0x3048){  // "え" ￥うえ￥
          output_codepoint1 = 0x3046;  // "う"
          output_codepoint2 = 0x3048;  // "え"
        } else if (third_char == 0x304B){ // "か" ￥うか￥
          output_codepoint1 = 0x304B;  // "か"（出力は１文字だけ）
        }
        break;
      case 0x3048:                  // "え"
        if (third_char == 0x304D){  // "き" ￥えき￥
          output_codepoint1 = 0x3048;  // "え"
          output_codepoint2 = 0x304D;  // "き"
        } else if (third_char == 0x3093){ // "ん" ￥えん￥
          output_codepoint1 = 0x3048;  // "え"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x304A:                  // "お"
        if (third_char == 0x304A){  // "お" ￥おお￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x304A;  // "お"
        } else if (third_char == 0x304F){ // "く" ￥おく￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x304F;  // "く"
        } else if (third_char == 0x3082){ // "も" ￥おも￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x3082;  // "も"
        } else if (third_char == 0x308F){ // "わ" ￥おわ￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x308F;  // "わ"
        }
        break;
      case 0x304B:                  // "か"
        if (third_char == 0x3044){  // "い" ￥かい￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3048){ // "え" ￥かえ￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3048;  // "え"
        } else if (third_char == 0x304F){ // "く" ￥かく￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x304F;  // "く"
        } else if (third_char == 0x3089){ // "ら" ￥から￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3089;  // "ら"
        }
        break;
      case 0x304C:                  // "が"
        if (third_char == 0x3044){  // "い" ￥がい￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3044;  // "い"
        }
        break;
      case 0x304D:                  // "き"
        if (third_char == 0x305F){  // "た" ￥きた￥
          output_codepoint1 = 0x304D;  // "き"
          output_codepoint2 = 0x305F;  // "た"
        } else if (third_char == 0x308D){ // "ろ" ￥きろ￥
          output_codepoint1 = 0x304D;  // "き"
          output_codepoint2 = 0x308D;  // "ろ"
        }
        break;
      case 0x304F:                  // "く"
        if (third_char == 0x307F){  // "み" ￥くみ￥
          output_codepoint1 = 0x304F;  // "く"
          output_codepoint2 = 0x307F;  // "み"
        }
        break;
      case 0x3051:                  // "け"
        if (third_char == 0x3093){  // "ん" ￥けん￥
          output_codepoint1 = 0x3051;  // "け"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3053:                  // "こ"
        if (third_char == 0x3046){  // "う" ￥こう￥
          output_codepoint1 = 0x3053;  // "こ"
          output_codepoint2 = 0x3046;  // "う"
        }
        break;
      case 0x3054:                  // "ご"
        if (third_char == 0x3046){  // "う" ￥ごう￥
          output_codepoint1 = 0x3054;  // "ご"
          output_codepoint2 = 0x3046;  // "う"
        }
        break;
      case 0x3055:                  // "さ"
        if (third_char == 0x3044){  // "い" ￥さい￥
          output_codepoint1 = 0x3055;  // "さ"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3064){ // "つ" ￥さつ￥
          output_codepoint1 = 0x3055;  // "さ"
          output_codepoint2 = 0x3064;  // "つ"
        }
        break;
      case 0x3057:                  // "し"
        if (third_char == 0x305f){  // "た" ￥した￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x305f;  // "た"
        } else if (third_char == 0x3081){ // "め" ￥しめ￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3081;  // "め" 
        } else if (third_char == 0x3083){ // "ゃ" ￥しゃ￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3083;  // "ゃ"
        } else if (third_char == 0x3093){ // "ん" ￥しん￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3058:                  // "じ"
        if (third_char == 0x3083){  // "ゃ" ￥じゃ￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3083;  // "ゃ"
        }
        break;
      case 0x305b:                  // "せ"
        if (third_char == 0x3093){  // "ん" ￥せん￥
          output_codepoint1 = 0x305b;  // "せ"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x305d:                  // "そ"
        if (third_char == 0x304f){  // "く" ￥そく￥
          output_codepoint1 = 0x305d;  // "そ"
          output_codepoint2 = 0x304f;  // "く"
        }
        break;
      case 0x305f:                  // "た"
        if (third_char == 0x3044){  // "い" ￥たい￥
          output_codepoint1 = 0x305f;  // "た"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x304b){ // "か" ￥たか￥
          output_codepoint1 = 0x305f;  // "た"
          output_codepoint2 = 0x304b;  // "か"
        }
        break;
      case 0x3060:                  // "だ"
        if (third_char == 0x3044){  // "い" ￥だい￥
          output_codepoint1 = 0x3060;  // "だ"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3093){ // "ん" ￥だん￥
          output_codepoint1 = 0x3060;  // "だ"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3061:                  // "ち"
        if (third_char == 0x304b){  // "か" ￥ちか￥
          output_codepoint1 = 0x3061;  // "ち"
          output_codepoint2 = 0x304b;  // "か"
        }
        break;
      case 0x3064:                  // "つ"
        if (third_char == 0x3046){  // "う" ￥つう￥
          output_codepoint1 = 0x3064;  // "つ"
          output_codepoint2 = 0x3046;  // "う"
        } else if (third_char == 0x307c){ // "ぼ" ￥つぼ￥
          output_codepoint1 = 0x3064;  // "つ"
          output_codepoint2 = 0x307c;  // "ぼ"
        }
        break;
      case 0x3066:                  // "て"
        if (third_char == 0x3093){  // "ん" ￥てん￥
          output_codepoint1 = 0x3066;  // "て"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3068:                  // "と"
        if (third_char == 0x3046){  // "う" ￥とう￥
          output_codepoint1 = 0x3068;  // "と"
          output_codepoint2 = 0x3046;  // "う"
        } else if (third_char == 0x3057){ // "し" ￥とし￥
          output_codepoint1 = 0x3068;  // "と"
          output_codepoint2 = 0x3057;  // "し"
        } else if (third_char == 0x307e){ // "ま" ￥とま￥
          output_codepoint1 = 0x3068;  // "と"
          output_codepoint2 = 0x307e;  // "ま"
        } else if (third_char == 0x3081){ // "め" ￥とめ￥
          output_codepoint1 = 0x3068;  // "と"
          output_codepoint2 = 0x3081;  // "め"
        } else if (third_char == 0x3093){ // "ん" ￥とん￥
          output_codepoint1 = 0x3068;  // "と"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3069:                  // "ど"
        if (third_char == 0x3046){  // "う" ￥どう￥
          output_codepoint1 = 0x3069;  // "ど"
          output_codepoint2 = 0x3046;  // "う"
        } else if (third_char == 0x308b){ // "る" ￥どる￥
          output_codepoint1 = 0x3069;  // "ど"
          output_codepoint2 = 0x308b;  // "る"
        }
        break;
      case 0x306A:                  // "な"
        if (third_char == 0x304A){  // "お" ￥なお￥
          output_codepoint1 = 0x306A;  // "な"
          output_codepoint2 = 0x304A;  // "お"
        } else if (third_char == 0x304C){ // "が" ￥なが￥
          output_codepoint1 = 0x306A;  // "な"
          output_codepoint2 = 0x304C;  // "が"
        } else if (third_char == 0x3089){ // "ら" ￥なら￥
          output_codepoint1 = 0x306A;  // "な"
          output_codepoint2 = 0x3089;  // "ら"
        }
        break;
      case 0x306b:                  // "に"
        if (third_char == 0x3057){  // "し" ￥にし￥
          output_codepoint1 = 0x306b;  // "に"
          output_codepoint2 = 0x3057;  // "し"
        } else if (third_char == 0x3093){ // "ん" ￥にん￥
          output_codepoint1 = 0x306b;  // "に"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x306d:                  // "ね"
        if (third_char == 0x3093){  // "ん" ￥ねん￥
          output_codepoint1 = 0x306d;  // "ね"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x306e:                  // "の"
        if (third_char == 0x305E){  // "ぞ" ￥のぞ￥
          output_codepoint1 = 0x306e;  // "の"
          output_codepoint2 = 0x305E;  // "ぞ"
        }
        break;
      case 0x306f:                  // "は"
        if (third_char == 0x3044){  // "い" ￥はい￥
          output_codepoint1 = 0x306f;  // "は"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3055){ // "さ" ￥はさ￥
          output_codepoint1 = 0x306f;  // "は"
          output_codepoint2 = 0x3055;  // "さ"
        } else if (third_char == 0x3064){ // "つ" ￥はつ￥
          output_codepoint1 = 0x306f;  // "は"
          output_codepoint2 = 0x3064;  // "つ"
        }
        break;
      case 0x3070:                  // "ば"
        if (third_char == 0x3044){  // "い" ￥ばい￥
          output_codepoint1 = 0x3070;  // "ば"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3093){ // "ん" ￥ばん￥
          output_codepoint1 = 0x3070;  // "ば"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3072:                  // "ひ"
        if (third_char == 0x304d){  // "き" ￥ひき￥
          output_codepoint1 = 0x3072;  // "ひ"
          output_codepoint2 = 0x304d;  // "き"
        } else if (third_char == 0x3064){ // "つ" ￥ひつ￥
          output_codepoint1 = 0x3072;  // "ひ"
          output_codepoint2 = 0x3064;  // "つ"
        } else if (third_char == 0x308d){ // "ろ" ￥ひろ￥
          output_codepoint1 = 0x3072;  // "ひ"
          output_codepoint2 = 0x308d;  // "ろ"
        }
        break;
      case 0x3073:                  // "び"
        if (third_char == 0x3093){  // "ん" ￥びん￥
          output_codepoint1 = 0x3073;  // "び"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3075:                  // "ふ"
        if (third_char == 0x308b){  // "る" ￥ふる￥
          output_codepoint1 = 0x3075;  // "ふ"
          output_codepoint2 = 0x308b;  // "る"
        } else if (third_char == 0x3093){ // "ん" ￥ふん￥
          output_codepoint1 = 0x3075;  // "ふ"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x3078:                  // "へ"
        if (third_char == 0x3093){  // "ん" ￥へん￥
          output_codepoint1 = 0x3078;  // "へ"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x307b:                  // "ほ"
        if (third_char == 0x3046){  // "う" ￥ほう￥
          output_codepoint1 = 0x307b;  // "ほ"
          output_codepoint2 = 0x3046;  // "う"
        } else if (third_char == 0x3093){ // "ん" ￥ほん￥
          output_codepoint1 = 0x307b;  // "ほ"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x307c:                  // "ぼ"
        if (third_char == 0x3046){  // "う" ￥ぼう￥
          output_codepoint1 = 0x307c;  // "ぼ"
          output_codepoint2 = 0x3046;  // "う"
        }
        break;
      case 0x307e:                  // "ま"
        if (third_char == 0x3044){  // "い" ￥まい￥
          output_codepoint1 = 0x307e;  // "ま"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3093){ // "ん" ￥まん￥
          output_codepoint1 = 0x307e;  // "ま"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x307f:                  // "み"
        if (third_char == 0x305b){  // "せ" ￥みせ￥
          output_codepoint1 = 0x307f;  // "み"
          output_codepoint2 = 0x305b;  // "せ"
        } else if (third_char == 0x308a){ // "り" ￥みり￥
          output_codepoint1 = 0x307f;  // "み"
          output_codepoint2 = 0x308a;  // "り"
        }
        break;
      case 0x3080:                  // "む"
        if (third_char == 0x306d){  // "ね" ￥むね￥
          output_codepoint1 = 0x3080;  // "む"
          output_codepoint2 = 0x306d;  // "ね"
        }
        break;
      case 0x3081:                  // "め"
        if (third_char == 0x3044){  // "い" ￥めい￥
          output_codepoint1 = 0x3081;  // "め"
          output_codepoint2 = 0x3044;  // "い"
        }
        break;
      case 0x3082:                  // "も"
        if (third_char == 0x3046){  // "う" ￥もう￥
          output_codepoint1 = 0x3082;  // "も"
          output_codepoint2 = 0x3046;  // "う"
        }
        break;
      case 0x3084:                  // "や"
        if (third_char == 0x3081){  // "め" ￥やめ￥
          output_codepoint1 = 0x3084;  // "や"
          output_codepoint2 = 0x3081;  // "め"
        }
        break;
      case 0x3088:                  // "よ"
        if (third_char == 0x3046){  // "う" ￥よう￥
          output_codepoint1 = 0x3088;  // "よ"
          output_codepoint2 = 0x3046;  // う"
        }
        break;
      case 0x3089:                  // "ら"
        if (third_char == 0x304b){  // "か" ￥らか￥
          output_codepoint1 = 0x304b;  // "か"（出力は１文字だけ）
        } else if (third_char == 0x304d){ // "き" ￥らき￥
          output_codepoint1 = 0x304d;  // "き"（出力は１文字だけ）
        }
        break;
      case 0x308a:                  // "り"
        if (third_char == 0x304d){  // "き" ￥りき￥
          output_codepoint1 = 0x304d;  // "き"（出力は１文字だけ）
        } else if (third_char == 0x3093){ // "ん" ￥りん￥
          output_codepoint1 = 0x308a;  // "り"
          output_codepoint2 = 0x3093;  // "ん"
        }
        break;
      case 0x308b:                  // "る"
        if (third_char == 0x3044){  // "い" ￥るい￥
          output_codepoint1 = 0x3044;  // "い"（出力は１文字だけ）
        } else if (third_char == 0x304b){ // "か" ￥るか￥
          output_codepoint1 = 0x304b;  // "か"（出力は１文字だけ）
        } else if (third_char == 0x3057){ // "し" ￥るし￥
          output_codepoint1 = 0x3057;  // "し"（出力は１文字だけ）
        } else if (third_char == 0x3088){ // "よ" ￥るよ￥
          output_codepoint1 = 0x3088;  // "よ"（出力は１文字だけ）
        }
        break;
      case 0x308c:                  // "れ"
        if (third_char == 0x3044){  // "い" ￥れい￥
          output_codepoint1 = 0x308c;  // "れ"
          output_codepoint2 = 0x3044;  // "い"
        } else if (third_char == 0x3064){ // "つ" ￥れつ￥
          output_codepoint1 = 0x308c;  // "れ"
          output_codepoint2 = 0x3064;  // "つ"
        }
        break;
      case 0x308d:                  // "ろ"
        if (third_char == 0x304b){  // "か" ￥ろか￥
          output_codepoint1 = 0x304b;  // "か"（出力は１文字だけ）
        } else if (third_char == 0x3088){ // "よ" ￥ろよ￥
          output_codepoint1 = 0x3088;  // "よ"（出力は１文字だけ）
        }
        break;
      case 0x308f:                  // "わ"
        if (third_char == 0x304b){  // "か" ￥わか￥
          output_codepoint1 = 0x308f;  // "わ"
          output_codepoint2 = 0x304b;  // "か"
        } else if (third_char == 0x308a){ // "り" ￥わり￥
          output_codepoint1 = 0x308f;  // "わ"
          output_codepoint2 = 0x308a;  // "り"
        }
        break;
      default:
        output_codepoint1 = 0x0000;
        output_codepoint2 = 0x0000;
        break;
    }

    if (output_codepoint1 != 0x0000) {
      Util::CodepointToUtf8Append(output_codepoint1, output);  // "表記指定外した読み１"
      if (output_codepoint2 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint2, output);  // "表記指定外した読み２"
      }
      if (begin + *mblen + mblen2 + mblen3 + mblen4 >= end) { //４文字目で終わりのとき
        *mblen += mblen2 + mblen3 + mblen4;
        return true;
      }
      is_hited_at_fourth = true;
    }
    
  }
  
  if (begin + *mblen + mblen2 + mblen3 + mblen4 >= end) { //４文字目で終わりのとき
    if (is_hited_at_third) {
      *mblen += mblen2 + mblen3;
      return true;
    } else {
      *mblen = 0;
      return false;
    }
  }

  size_t mblen5 = 0;
  const char32_t fifth_char =
  Util::Utf8ToCodepoint(begin + *mblen + mblen2 + mblen3 + mblen4, end, &mblen5);

  output_codepoint1 = 0x0000; //出力１文字目
  output_codepoint2 = 0x0000; //出力２文字目
  uint16_t output_codepoint3 = 0x0000; //出力３文字目
  bool is_hited_at_fifth = false;  // ５文字目で該当したかどうか
   if (fifth_char == 0xFFE5 || fifth_char == 0x5c) {  // "￥\"  ￥◯◯◯￥のとき
    switch (second_char) {
      case 0x3042:                  // "あ"
        if (third_char == 0x3084 && fourth_char == 0x307E){  // "やま" ￥あやま￥
          output_codepoint1 = 0x3042;  // "あ"
          output_codepoint2 = 0x3084;  // "や"
          output_codepoint3 = 0x307E;  // "ま"
        }
        break;
      case 0x3044:                  // "い"
        if (third_char == 0x3089 && fourth_char == 0x3093){  // "らん" ￥いらん￥
          output_codepoint1 = 0x3044;  // "い"
          output_codepoint2 = 0x3089;  // "ら"
          output_codepoint3 = 0x3093;  // "ん"
        }
        break;
      case 0x3046:                  // "う"
        if (third_char == 0x3049 && fourth_char == 0x3093){  // "ぉん" ￥うぉん￥
          output_codepoint1 = 0x3046;  // "う"
          output_codepoint2 = 0x3049;  // "ぉ"
          output_codepoint3 = 0x3093;  // "ん"
        } else if (third_char == 0x306F && fourth_char == 0x3044){  // "はい" ￥うはい￥
          output_codepoint1 = 0x306F;  // "は"
          output_codepoint2 = 0x3044;  // "い"
        }
        break;
      case 0x304A:                  // "お"
        if (third_char == 0x3044 && fourth_char == 0x3057){  // "いし" ￥おいし￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x3044;  // "い"
          output_codepoint3 = 0x3057;  // "し"
        } else if (third_char == 0x307F && fourth_char == 0x305B){  // "みせ" ￥おみせ￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x307F;  // "み"
          output_codepoint3 = 0x305B;  // "せ"
        } else if (third_char == 0x3093 && fourth_char == 0x3077){  // "んぷ" ￥おんぷ￥
          output_codepoint1 = 0x304A;  // "お"
          output_codepoint2 = 0x3093;  // "ん"
          output_codepoint3 = 0x3077;  // "ぷ"
        }
        break;
      case 0x304B:                  // "か"
        if (third_char == 0x3044 && fourth_char == 0x308A){  // "いり" ￥かいり￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3044;  // "い"
          output_codepoint3 = 0x308A;  // "り"
        } else if (third_char == 0x304B && fourth_char == 0x308A){  // "かり" ￥かかり￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x304B;  // "か"
          output_codepoint3 = 0x308A;  // "り"
        } else if (third_char == 0x3052 && fourth_char == 0x3064){  // "げつ" ￥かげつ￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3052;  // "げ"
          output_codepoint3 = 0x3064;  // "つ"
        } else if (third_char == 0x3053 && fourth_char == 0x304F){  // "こく" ￥かこく￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3053;  // "こ"
          output_codepoint3 = 0x304F;  // "く"
        } else if (third_char == 0x3057 && fourth_char == 0x3087){  // "しょ" ￥かしょ￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3057;  // "し"
          output_codepoint3 = 0x3087;  // "ょ"
        } else if (third_char == 0x306D && fourth_char == 0x3093){  // "ねん" ￥かねん￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x306D;  // "ね"
          output_codepoint3 = 0x3093;  // "ん"
        } else if (third_char == 0x308F && fourth_char == 0x305B){  // "わせ" ￥かわせ￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x308F;  // "わ"
          output_codepoint3 = 0x305B;  // "せ"
        }
        break;
      case 0x304D:                  // "き"
        if (third_char == 0x304B && fourth_char == 0x3044){  // "かい" ￥きかい￥
          output_codepoint1 = 0x304D;  // "き"
          output_codepoint2 = 0x304B;  // "か"
          output_codepoint3 = 0x3044;  // "い"
        } else if (third_char == 0x3083 && fourth_char == 0x304F){  // "ゃく" ￥きゃく￥
          output_codepoint1 = 0x304D;  // "き"
          output_codepoint2 = 0x3083;  // "ゃ"
          output_codepoint3 = 0x304F;  // "く"
        } else if (third_char == 0x3085 && fourth_char == 0x3046){  // "ゅう" ￥きゅう￥
          output_codepoint1 = 0x304D;  // "き"
          output_codepoint2 = 0x3085;  // "ゅ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x304E:                  // "ぎ"
        if (third_char == 0x3087 && fourth_char == 0x3046){  // "ょう" ￥ぎょう￥
          output_codepoint1 = 0x304E;  // "ぎ"
          output_codepoint2 = 0x3087;  // "ょ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x304F:                  // "く"
        if (third_char == 0x3089 && fourth_char == 0x3044){  // "らい" ￥くらい￥
          output_codepoint1 = 0x3044;  // "い"
        }
        break;
      case 0x3050:                  // "ぐ"
        if (third_char == 0x3089 && fourth_char == 0x3080){  // "らむ" ￥ぐらむ￥
          output_codepoint1 = 0x3050;  // "ぐ"
          output_codepoint2 = 0x3089;  // "ら"
          output_codepoint3 = 0x3080;  // "む"
        }
        break;
      case 0x3053:                  // "こ"
        if (third_char == 0x3046 && fourth_char == 0x3058){  // "うじ" ￥こうじ￥
          output_codepoint1 = 0x3053;  // "こ"
          output_codepoint2 = 0x3046;  // "う"
          output_codepoint3 = 0x3058;  // "じ"
        }
        break;
      case 0x3055:                  // "さ"
        if (third_char == 0x3089 && fourth_char == 0x306B){  // "らに" ￥さらに￥
          output_codepoint1 = 0x3055;  // "さ"
          output_codepoint2 = 0x3089;  // "ら"
          output_codepoint3 = 0x306B;  // "に"
        }
        break;
      case 0x3057:                  // "し"
        if (third_char == 0x3085 && fourth_char == 0x3046){  // "ゅう" ￥しゅう￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3085;  // "ゅ"
          output_codepoint3 = 0x3046;  // "う"
        } else if (third_char == 0x3087 && fourth_char == 0x3046){  // "ょう" ￥しょう￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3087;  // "ょ"
          output_codepoint3 = 0x3046;  // "う"
        } else if (third_char == 0x3088 && fourth_char == 0x3046){  // "よう" ￥しよう￥
          output_codepoint1 = 0x3057;  // "し"
          output_codepoint2 = 0x3088;  // "よ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x3058:                  // "じ"
        if (third_char == 0x304B && fourth_char == 0x3093){  // "かん" ￥じかん￥
          output_codepoint1 = 0x3058;  // "じ"
          output_codepoint2 = 0x304B;  // "か"
          output_codepoint3 = 0x3093;  // "ん"
        } else if (third_char == 0x3087 && fourth_char == 0x3046){  // "ょう" ￥じょう￥
          output_codepoint1 = 0x3058;  // "じ"
          output_codepoint2 = 0x3087;  // "ょ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x305B:                  // "せ"
        if (third_char == 0x3093 && fourth_char == 0x3061){  // "んち" ￥せんち￥
          output_codepoint1 = 0x305B;  // "せ"
          output_codepoint2 = 0x3093;  // "ん"
          output_codepoint3 = 0x3061;  // "ち"
        }
        break;
      case 0x305D:                  // "そ"
        if (third_char == 0x3046 && fourth_char == 0x3055){  // "うさ" ￥そうさ￥
          output_codepoint1 = 0x305D;  // "そ"
          output_codepoint2 = 0x3046;  // "う"
          output_codepoint3 = 0x3055;  // "さ"
        }
        break;
      case 0x305F:                  // "た"
        if (third_char == 0x305A && fourth_char == 0x306D){  // "ずね" ￥たずね￥
          output_codepoint1 = 0x305F;  // "た"
          output_codepoint2 = 0x305A;  // "ず"
          output_codepoint3 = 0x306D;  // "ね"
        } else if (third_char == 0x3068 && fourth_char == 0x3048){  // "とえ" ￥たとえ￥
          output_codepoint1 = 0x305F;  // "た"
          output_codepoint2 = 0x3068;  // "と"
          output_codepoint3 = 0x3048;  // "え"
        }
        break;
      case 0x3061:                  // "ち"
        if (third_char == 0x304B && fourth_char == 0x304F){  // "ゃく" ￥ちゃく￥
          output_codepoint1 = 0x3061;  // "ち"
          output_codepoint2 = 0x304B;  // "ゃ"
          output_codepoint3 = 0x304F;  // "く"
        } else if (third_char == 0x3087 && fourth_char == 0x3046){  // "ょう" ￥ちょう￥
          output_codepoint1 = 0x3061;  // "ち"
          output_codepoint2 = 0x3087;  // "ょ"
          output_codepoint3 = 0x3046;  // "う"
        } else if (third_char == 0x3088 && fourth_char == 0x3046){  // "よう" ￥ちよう￥
          output_codepoint1 = 0x3061;  // "ち"
          output_codepoint2 = 0x3088;  // "よ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x306A:                  // "な"
        if (third_char == 0x304F && fourth_char == 0x306A){  // "くな" ￥なくな￥
          output_codepoint1 = 0x306A;  // "な"
          output_codepoint2 = 0x304F;  // "く"
          output_codepoint3 = 0x306A;  // "な"
        }
        break;
      case 0x306B:                  // "に"
        if (third_char == 0x3082 && fourth_char == 0x304B){  // "もか" ￥にもか￥
          output_codepoint1 = 0x304B;  // "か"
        } else if (third_char == 0x3082 && fourth_char == 0x3053){  // "もこ" ￥にもこ￥
          output_codepoint1 = 0x3053;  // "こ"
        }
        break;
      case 0x306D:                  // "ね"
        if (third_char == 0x3093 && fourth_char == 0x3069){  // "んど" ￥ねんど￥
          output_codepoint1 = 0x306D;  // "ね"
          output_codepoint2 = 0x3093;  // "ん"
          output_codepoint3 = 0x3069;  // "ど"
        }
        break;
      case 0x306E:                  // "の"
        if (third_char == 0x3063 && fourth_char == 0x3068){  // "っと" ￥のっと￥
          output_codepoint1 = 0x306E;  // "の"
          output_codepoint2 = 0x3063;  // "っ"
          output_codepoint3 = 0x3068;  // "と"
        }
        break;
      case 0x3072:                  // "ひ"
        if (third_char == 0x304C && fourth_char == 0x3057){  // "がし" ￥ひがし￥
          output_codepoint1 = 0x3072;  // "ひ"
          output_codepoint2 = 0x304C;  // "が"
          output_codepoint3 = 0x3057;  // "し"
        } else if (third_char == 0x3087 && fourth_char == 0x3046){  // "ょう" ￥ひょう￥
          output_codepoint1 = 0x3072;  // "ひ"
          output_codepoint2 = 0x3087;  // "ょ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x3074:                  // "ぴ"
        if (third_char == 0x3087 && fourth_char == 0x3046){  // "ょう" ￥ぴょう￥
          output_codepoint1 = 0x3074;  // "ぴ"
          output_codepoint2 = 0x3087;  // "ょ"
          output_codepoint3 = 0x3046;  // "う"
        }
        break;
      case 0x3076:                  // "ぶ"
        if (third_char == 0x3093 && fourth_char == 0x306E){  // "んの" ￥ぶんの￥
          output_codepoint1 = 0x3076;  // "ぶ"
          output_codepoint2 = 0x3093;  // "ん"
          output_codepoint3 = 0x306E;  // "の"
        }
        break;
      case 0x307A:                  // "ぺ"
        if (third_char == 0x30FC && fourth_char == 0x3058){  // "ーじ" ￥ぶんの￥
          output_codepoint1 = 0x307A;  // "ぺ"
          output_codepoint2 = 0x30FC;  // "ー"
          output_codepoint3 = 0x3058;  // "じ"
        }
        break;
      case 0x307F:                  // "み"
        if (third_char == 0x306A && fourth_char == 0x307F){  // "なみ" ￥みなみ￥
          output_codepoint1 = 0x307F;  // "み"
          output_codepoint2 = 0x306A;  // "な"
          output_codepoint3 = 0x307F;  // "み"
        }
        break;
      case 0x3084:                  // "や"
        if (third_char == 0x30FC && fourth_char == 0x3069){  // "ーど" ￥やーど￥
          output_codepoint1 = 0x3084;  // "や"
          output_codepoint2 = 0x30FC;  // "ー"
          output_codepoint3 = 0x3069;  // "ど"
        }
        break;
      case 0x3086:                  // "ゆ"
        if (third_char == 0x30FC && fourth_char == 0x308D){  // "ーろ" ￥ゆーろ￥
          output_codepoint1 = 0x3086;  // "ゆ"
          output_codepoint2 = 0x30FC;  // "ー"
          output_codepoint3 = 0x308D;  // "ろ"
        }
        break;
      case 0x3088:                  // "よ"
        if (third_char == 0x3055 && fourth_char == 0x3093){  // "さん" ￥よさん￥
          output_codepoint1 = 0x3088;  // "よ"
          output_codepoint2 = 0x3055;  // "さ"
          output_codepoint3 = 0x3093;  // "ん"
        }
        break;
      case 0x308B:                  // "る"
        if (third_char == 0x304B && fourth_char == 0x3048){  // "かえ" ￥るかえ￥
          output_codepoint1 = 0x304B;  // "か"
          output_codepoint2 = 0x3048;  // "え"
        }
        break;
      case 0x308F:                  // "わ"
        if (third_char == 0x304B && fourth_char == 0x308C){  // "かれ" ￥わかれ￥
          output_codepoint1 = 0x308F;  // "わ"
          output_codepoint2 = 0x304B;  // "か"
          output_codepoint3 = 0x308C;  // "れ"
        }
        break;
      default:
        output_codepoint1 = 0x0000;
        output_codepoint2 = 0x0000;
        output_codepoint3 = 0x0000;
        break;
    }

    if (output_codepoint1 != 0x0000) {
      Util::CodepointToUtf8Append(output_codepoint1, output);  // "表記指定外した読み１"
      if (output_codepoint2 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint2, output);  // "表記指定外した読み２"
      }
      if (output_codepoint3 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint3, output);  // "表記指定外した読み３"
      }
      if (begin + *mblen + mblen2 + mblen3 + mblen4 + mblen5 >= end) { //５文字目で終わりのとき
        *mblen += mblen2 + mblen3 + mblen4 + mblen5;
        return true;
      }
      is_hited_at_fifth = true;
    }
    
  }

  if (begin + *mblen + mblen2 + mblen3 + mblen4 + mblen5 >= end) { //５文字目で終わりのとき
    if (is_hited_at_fourth) {
      *mblen += mblen2 + mblen3 + mblen4;
      return true;
    } else if (is_hited_at_third) {
      *mblen += mblen2 + mblen3;
      return true;
    } else {
      *mblen = 0;
      return false;
    }
  }

  size_t mblen6 = 0;
  const char32_t sixth_char =
  Util::Utf8ToCodepoint(begin + *mblen + mblen2 + mblen3 + mblen4 + mblen5, end, &mblen6);

  output_codepoint1 = 0x0000; //出力１文字目
  output_codepoint2 = 0x0000; //出力２文字目
  output_codepoint3 = 0x0000; //出力３文字目
  uint16_t output_codepoint4 = 0x0000; //出力４文字目
  bool is_hited_at_sixth = false;  // ６文字目で該当したかどうか
   if (sixth_char == 0xFFE5 || sixth_char == 0x5c) {  // "￥\"  ￥◯◯◯◯￥のとき
    switch (second_char) {
      case 0x3044:                  // "い"
        if (third_char == 0x3063 && fourth_char == 0x3071 && fifth_char == 0x3044){  // "っぱい" ￥いっぱい￥
          output_codepoint1 = 0x3044;  // "い"
          output_codepoint2 = 0x3063;  // "っ"
          output_codepoint3 = 0x3071;  // "ぱ"
          output_codepoint4 = 0x3044;  // "い"
        } else if (third_char == 0x307E && fourth_char == 0x3060 && fifth_char == 0x306B){  // "まだに" ￥いまだに￥
          output_codepoint1 = 0x3044;  // "い"
          output_codepoint2 = 0x307E;  // "ま"
          output_codepoint3 = 0x3060;  // "だ"
          output_codepoint4 = 0x306B;  // "に"
        } else if (third_char == 0x308D && fourth_char == 0x3044 && fifth_char == 0x308D){  // "ろいろ" ￥いろいろ￥
          output_codepoint1 = 0x3044;  // "い"
          output_codepoint2 = 0x308D;  // "ろ"
          output_codepoint3 = 0x3044;  // "い"
          output_codepoint4 = 0x308D;  // "ろ"
        }
        break;
      default:
        output_codepoint1 = 0x0000;
        output_codepoint2 = 0x0000;
        output_codepoint3 = 0x0000;
        output_codepoint4 = 0x0000;
        break;
    }
/*
＠おくえん＠	億円	
＠おもしろ＠	おもしろ	
＠かろりー＠	カロリー	
＠かんこく＠	韓国	
＠こうりつ＠	効率	
＠さまざま＠	さまざま	
＠さんてん＠	…	
＠しゅるい＠	種類	
＠じょせい＠	女性	
＠たくさん＠	たくさん	
＠ただいま＠	ただいま	
＠だんせい＠	男性	
＠にちかん＠	日間	
＠にもおく＠	億	
＠にもかい＠	回	
＠にもさい＠	歳	
＠にもたい＠	対	
＠にもだい＠	台	
＠にもばん＠	番	
＠ねんかん＠	年間	
＠ふんかん＠	分間	
＠へいほう＠	平方	
＠ぽいんと＠	ポイント	
＠ほとんど＠	ほとんど	
＠まんえん＠	万円	
＠まんどる＠	万ドル	
＠まんにん＠	万人	
＠みずから＠	みずから	
＠めーとる＠	メートル	
＠ようやく＠	ようやく	
＠らたずね＠	尋ね	一段動詞
＠りっとる＠	リットル	
＠りっぽう＠	立方	
＠るあやま＠	誤	ラ行五段
*/



    if (output_codepoint1 != 0x0000) {
      Util::CodepointToUtf8Append(output_codepoint1, output);  // "表記指定外した読み１"
      if (output_codepoint2 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint2, output);  // "表記指定外した読み２"
      }
      if (output_codepoint3 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint3, output);  // "表記指定外した読み３"
      }
      if (output_codepoint4 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint4, output);  // "表記指定外した読み３"
      }
      if (begin + *mblen + mblen2 + mblen3 + mblen4 + mblen5 + mblen6 >= end) { //６文字目で終わりのとき
        *mblen += mblen2 + mblen3 + mblen4 + mblen5 + mblen6;
        return true;
      }
      is_hited_at_sixth = true;
    }
    
  }

  if (begin + *mblen + mblen2 + mblen3 + mblen4 + mblen5 + mblen6 >= end) { //６文字目で終わりのとき
    if (is_hited_at_fifth) {
      *mblen += mblen2 + mblen3 + mblen4 + mblen5;
      return true;
    } else if (is_hited_at_fourth) {
      *mblen += mblen2 + mblen3 + mblen4;
      return true;
    } else if (is_hited_at_third) {
      *mblen += mblen2 + mblen3;
      return true;
    } else {
      *mblen = 0;
      return false;
    }
  }


  //ここから￥の５文字の表記指定書いていく～７文字まで
  //７文字（最後）のときは、そこで終わりじゃなくてもreturn trueで終了
  *mblen = 0;
  return false;

}

// "＃〇＃" pattern
bool RewriteSpecifiedWordSharp(size_t key_pos, const char *begin, const char *end,
  size_t *mblen, std::string *output) {
  const char32_t first_char = Util::Utf8ToCodepoint(begin, end, mblen);
  if (first_char != 0xFF03 && first_char != 0x23) {  // !"＃#"
    *mblen = 0;
    return false;
  }

  if (begin + *mblen >= end) { //１文字目で終わりのとき
    *mblen = 0;
    return false;
  }

  size_t mblen2 = 0;
  const char32_t second_char =
  Util::Utf8ToCodepoint(begin + *mblen, end, &mblen2);

  if (begin + *mblen + mblen2 >= end) { //２文字目で終わりのとき
    *mblen = 0;
    return false;
  }

  size_t mblen3 = 0;
  const char32_t third_char =
  Util::Utf8ToCodepoint(begin + *mblen + mblen2, end, &mblen3);

  uint16_t output_codepoint = 0x0000;
  bool is_hited_at_third = false;  // ３文字目で該当したかどうか
  if (third_char == 0xFF03 || third_char == 0x23) {  // "＃#"  ＃◯＃のとき
    switch (second_char) {
      case 0x306F:                  // "は"
        output_codepoint = 0x306F;  // "は"
        break;
      case 0x306B:                  // "に"
        output_codepoint = 0x306B;  // "に"
        break;
      case 0x306E:                  // "の"
        output_codepoint = 0x306E;  // "の"
        break;
      case 0x304C:                  // "が"
        output_codepoint = 0x304C;  // "が"
        break;
      case 0x3067:                  // "で"
        output_codepoint = 0x3067;  // "で"
        break;
      case 0x3082:                  // "も"
        output_codepoint = 0x3082;  // "も"
        break;
      default:
        output_codepoint = 0x0000;
        break;
    }

    if (output_codepoint != 0x0000) {
      Util::CodepointToUtf8Append(output_codepoint, output);  // "表記指定外した読み"
      if (begin + *mblen + mblen2 + mblen3 >= end) { //３文字目で終わりのとき
        *mblen += mblen2 + mblen3;
        return true;
      }
      is_hited_at_third = true;
    }

  }

  if (begin + *mblen + mblen2 + mblen3 >= end) { //３文字目で終わりのとき
    *mblen = 0;
    return false;
  }

  size_t mblen4 = 0;
  const char32_t fourth_char =
  Util::Utf8ToCodepoint(begin + *mblen + mblen2 + mblen3, end, &mblen4);

  uint16_t output_codepoint1 = 0x0000; //出力１文字目
  uint16_t output_codepoint2 = 0x0000; //出力２文字目
  if (fourth_char == 0xFF03 || fourth_char == 0x23) {  // "＃#"  ＃◯◯＃のとき
    switch (second_char) {
      case 0x306B:                  // "に"
        if (third_char == 0x306F){  // "は" ＃には＃
          output_codepoint1 = 0x306B;  // "に"
          output_codepoint2 = 0x306F;  // "は"
        } else if (third_char == 0x3082){ // "も" ＃にも＃
          output_codepoint1 = 0x306B;  // "に"
          output_codepoint2 = 0x3082;  // "も"
        }
        break;
      case 0x3067:                  // "で"
        if (third_char == 0x3082){  // "も" ＃でも＃
          output_codepoint1 = 0x3067;  // "で"
          output_codepoint2 = 0x3082;  // "も"
        } else if (third_char == 0x306F){ // "は" ＃では＃
          output_codepoint1 = 0x3067;  // "で"
          output_codepoint2 = 0x306F;  // "は"
        }
        break;
      default:
        output_codepoint1 = 0x0000;
        output_codepoint2 = 0x0000;
        break;
    }

    if (output_codepoint1 != 0x0000) {
      Util::CodepointToUtf8Append(output_codepoint1, output);  // "表記指定外した読み１"
      if (output_codepoint2 != 0x0000) {
        Util::CodepointToUtf8Append(output_codepoint2, output);  // "表記指定外した読み２"
      }
      *mblen += mblen2 + mblen3 + mblen4;
      return true;
    }
    
  }

  if (is_hited_at_third) { //３文字目ヒットしたとき
    *mblen += mblen2 + mblen3;
    return true;
  }

  *mblen = 0;
  return false;
}


}  // namespace

size_t KeyCorrector::GetCorrectedPosition(const size_t original_key_pos) const {
  if (original_key_pos < alignment_.size()) {
    return alignment_[original_key_pos];
  }
  return kInvalidPos;
}

size_t KeyCorrector::GetOriginalPosition(const size_t corrected_key_pos) const {
  if (corrected_key_pos < rev_alignment_.size()) {
    return rev_alignment_[corrected_key_pos];
  }
  return kInvalidPos;
}

void KeyCorrector::Clear() {
  available_ = false;
  original_key_.clear();
  corrected_key_.clear();
  alignment_.clear();
  rev_alignment_.clear();
}

bool KeyCorrector::CorrectKey(absl::string_view key, InputMode mode,
                              size_t history_size) {
  Clear();

  // TODO(taku)  support KANA
  if (mode == KANA) {
    return false;
  }

  if (key.empty() || key.size() >= kMaxSize) {
    MOZC_VLOG(1) << "invalid key length";
    return false;
  }

  original_key_ = key;

  const char *begin = key.data();
  const char *end = key.data() + key.size();
  const char *input_begin = key.data() + history_size;
  size_t key_pos = 0;

  while (begin < end) {
    size_t mblen = 0;
    const size_t org_len = corrected_key_.size();
    if (begin < input_begin ||
        (!RewriteSpecifiedWordYen(key_pos, begin, end, &mblen, &corrected_key_) &&
         !RewriteSpecifiedWordSharp(key_pos, begin, end, &mblen, &corrected_key_))) {
      const char32_t codepoint = Util::Utf8ToCodepoint(begin, end, &mblen);
      Util::CodepointToUtf8Append(codepoint, &corrected_key_);
    }

    const size_t corrected_mblen = corrected_key_.size() - org_len;

    if (corrected_mblen <= 0 && mblen <= 0) {
      LOG(ERROR) << "Invalid pattern: " << key;
      Clear();
      return false;
    }

    // one to one mapping
    if (mblen == corrected_mblen) {
      const size_t len = static_cast<size_t>(begin - key.data());
      for (size_t i = 0; i < mblen; ++i) {
        alignment_.push_back(org_len + i);
        rev_alignment_.push_back(len + i);
      }
    } else {
      // NOT a one to one mapping, we take fist/last alignment only
      alignment_.push_back(org_len);
      for (size_t i = 1; i < mblen; ++i) {
        alignment_.push_back(kInvalidPos);
      }
      rev_alignment_.push_back(static_cast<size_t>(begin - key.data()));
      for (size_t i = 1; i < corrected_mblen; ++i) {
        rev_alignment_.push_back(kInvalidPos);
      }
    }

    begin += mblen;
    ++key_pos;
  }

  DCHECK_EQ(original_key_.size(), alignment_.size());
  DCHECK_EQ(corrected_key_.size(), rev_alignment_.size());

  available_ = true;
  return true;
}

const char *KeyCorrector::GetCorrectedPrefix(const size_t original_key_pos,
                                             size_t *length) const {
  if (!IsAvailable()) {
    *length = 0;
    return nullptr;
  }

  if (mode_ == KANA) {
    *length = 0;
    return nullptr;
  }

  const size_t corrected_key_pos = GetCorrectedPosition(original_key_pos);
  if (!IsValidPosition(corrected_key_pos)) {
    *length = 0;
    return nullptr;
  }

  const char *corrected_substr = corrected_key_.data() + corrected_key_pos;
  const size_t corrected_length = corrected_key_.size() - corrected_key_pos;
  const char *original_substr = original_key_.data() + original_key_pos;
  const size_t original_length = original_key_.size() - original_key_pos;
  // substrs are different
  if (original_length != corrected_length ||
      memcmp(original_substr, corrected_substr, original_length) != 0) {
    *length = corrected_length;
    return corrected_substr;
  }

  *length = 0;
  return nullptr;
}

size_t KeyCorrector::GetOriginalOffset(const size_t original_key_pos,
                                       const size_t new_key_offset) const {
  if (!IsAvailable()) {
    return kInvalidPos;
  }

  if (mode_ == KANA) {
    return kInvalidPos;
  }

  const size_t corrected_key_pos = GetCorrectedPosition(original_key_pos);
  if (!IsValidPosition(corrected_key_pos)) {
    return kInvalidPos;
  }

  if (rev_alignment_.size() == corrected_key_pos + new_key_offset) {
    return (alignment_.size() - GetOriginalPosition(corrected_key_pos));
  } else {
    // treat right edge
    const size_t original_key_pos2 =
        GetOriginalPosition(corrected_key_pos + new_key_offset);

    if (!IsValidPosition(original_key_pos2)) {
      return kInvalidPos;
    }

    // Don't allow NULL matching
    if (original_key_pos2 >= original_key_pos) {
      return static_cast<size_t>(original_key_pos2 - original_key_pos);
    }
  }

  return kInvalidPos;
}

// static
int KeyCorrector::GetCorrectedCostPenalty(absl::string_view key) {
  // "んん" and "っっ" must be mis-spelling.
  if (absl::StrContains(key, "んん") || absl::StrContains(key, "っっ")) {
    return 0;
  }
  // add 3000 to the original word cost
  constexpr int kCorrectedCostPenalty = 100;
  return kCorrectedCostPenalty;
}

}  // namespace mozc
