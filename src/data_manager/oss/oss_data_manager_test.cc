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

#include "data_manager/oss/oss_data_manager.h"

#include "data_manager/data_manager_test_base.h"
#include "testing/gunit.h"
#include "testing/mozctest.h"

namespace mozc {
namespace oss {
namespace {

#include "data_manager/oss/segmenter_inl.inc"

}  // namespace

class OssDataManagerTest : public DataManagerTestBase {
 protected:
  OssDataManagerTest()
      : DataManagerTestBase(
            new OssDataManager, kLSize, kRSize, IsBoundaryInternal,
            mozc::testing::GetSourceFileOrDie({MOZC_DICT_DIR_COMPONENTS,
                                               "dictionary_oss",
                                               "connection_single_column.txt"}),
            1,
            mozc::testing::GetSourceFilesInDirOrDie(
                {MOZC_DICT_DIR_COMPONENTS, "dictionary_oss"},
#ifdef MOZC_GYP_BUILD
                {"mozcdic-ut0425あ.txt", "mozcdic-ut0425い.txt", "mozcdic-ut0425う.txt",
                 "mozcdic-ut0425え.txt", "mozcdic-ut0425お.txt", "mozcdic-ut0425か.txt",
                 "mozcdic-ut0425き.txt", "mozcdic-ut0425く.txt", "mozcdic-ut0425け.txt",
                 "mozcdic-ut0425こ.txt", "mozcdic-ut0425さ.txt", "mozcdic-ut0425し.txt",
                 "mozcdic-ut0425す.txt", "mozcdic-ut0425せ.txt", "mozcdic-ut0425そ.txt",
                 "mozcdic-ut0425た.txt", "mozcdic-ut0425ち.txt", "mozcdic-ut0425つ.txt",
                 "mozcdic-ut0425て.txt", "mozcdic-ut0425と.txt", "mozcdic-ut0425な.txt",
                 "mozcdic-ut0425に.txt", "mozcdic-ut0425ぬ.txt", "mozcdic-ut0425ね.txt",
                 "mozcdic-ut0425の.txt", "mozcdic-ut0425は.txt", "mozcdic-ut0425ひ.txt",
                 "mozcdic-ut0425ふ.txt", "mozcdic-ut0425へ.txt", "mozcdic-ut0425ほ.txt",
                 "mozcdic-ut0425ま.txt", "mozcdic-ut0425み.txt", "mozcdic-ut0425む.txt",
                 "mozcdic-ut0425め.txt", "mozcdic-ut0425も.txt", "mozcdic-ut0425や.txt",
                 "mozcdic-ut0425ゆ.txt", "mozcdic-ut0425よ.txt", "mozcdic-ut0425ら.txt",
                 "mozcdic-ut0425り.txt", "mozcdic-ut0425る.txt", "mozcdic-ut0425れ.txt",
                 "mozcdic-ut0425ろ.txt", "mozcdic-ut0425わ.txt", "mozcdic-ut0425を.txt",
                 "mozcdic-ut0425ん.txt", "mozcdic-ut0425記号.txt"}
#else  // MOZC_GYP_BUILD
                {"aux_dictionary.txt", "dictionary.txt"}
#endif  // MOZC_GYP_BUILD
            ),
            mozc::testing::GetSourceFilesInDirOrDie(
                {MOZC_DICT_DIR_COMPONENTS, "dictionary_oss"},
                {"suggestion_filter.txt"})) {}
};

TEST_F(OssDataManagerTest, AllTests) { RunAllTests(); }

}  // namespace oss
}  // namespace mozc
