# Copyright 2010-2021, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

{
  'variables': {
    'relative_dir': 'data_manager/oss',
    'relative_mozc_dir': '',
    'gen_out_dir': '<(SHARED_INTERMEDIATE_DIR)/<(relative_dir)',
    'gen_out_mozc_dir': '<(SHARED_INTERMEDIATE_DIR)/<(relative_mozc_dir)',
  },
  'targets': [
    {
      'target_name': 'oss_data_manager_test',
      'type': 'executable',
      'sources': [
        'oss_data_manager_test.cc',
      ],
      'dependencies': [
        '<(mozc_oss_src_dir)/testing/testing.gyp:gtest_main',
        '<(mozc_oss_src_dir)/testing/testing.gyp:mozctest',
        '<(mozc_oss_src_dir)/data_manager/data_manager_test.gyp:data_manager_test_base',
        'oss_data_manager.gyp:oss_data_manager',
        'oss_data_manager.gyp:gen_oss_segmenter_inl_header#host',
        'install_oss_data_manager_test_data',
      ],
      'variables': {
        'test_size': 'small',
      },
    },
    {
      'target_name': 'install_oss_data_manager_test_data',
      'type': 'none',
      'variables': {
        'test_data': [
          '<(mozc_oss_src_dir)/data/dictionary_oss/connection_single_column.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425あ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425い.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425う.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425え.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425お.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425か.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425き.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425く.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425け.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425こ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425さ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425し.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425す.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425せ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425そ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425た.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ち.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425つ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425て.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425と.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425な.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425に.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ぬ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ね.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425の.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425は.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ひ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ふ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425へ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ほ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ま.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425み.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425む.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425め.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425も.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425や.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ゆ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425よ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ら.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425り.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425る.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425れ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ろ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425わ.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425を.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425ん.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/mozcdic-ut0425記号.txt',
          '<(mozc_oss_src_dir)/data/dictionary_oss/suggestion_filter.txt',
        ],
        'test_data_subdir': 'data/dictionary_oss',
      },
      'includes': ['../../gyp/install_testdata.gypi'],
    },
  ],
}
