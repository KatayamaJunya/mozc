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
    # The following variables are passed to ../data_manager.gypi.
    'current_dir': '.',
    'mozc_dir': '../..',
    'common_data_dir': '<(mozc_dir)/data',
    'platform_data_dir': '<(mozc_oss_src_dir)/data/dictionary_oss',
    'boundary_def': '<(mozc_oss_src_dir)/data/rules/boundary.def',
    'dataset_tag': 'oss',
    'use_1byte_cost_for_connection_data': 'false',
    'dictionary_files': [
      '<(platform_data_dir)/mozcdic-ut0425あ.txt',
      '<(platform_data_dir)/mozcdic-ut0425い.txt',
      '<(platform_data_dir)/mozcdic-ut0425う.txt',
      '<(platform_data_dir)/mozcdic-ut0425え.txt',
      '<(platform_data_dir)/mozcdic-ut0425お.txt',
      '<(platform_data_dir)/mozcdic-ut0425か.txt',
      '<(platform_data_dir)/mozcdic-ut0425き.txt',
      '<(platform_data_dir)/mozcdic-ut0425く.txt',
      '<(platform_data_dir)/mozcdic-ut0425け.txt',
      '<(platform_data_dir)/mozcdic-ut0425こ.txt',
      '<(platform_data_dir)/mozcdic-ut0425さ.txt',
      '<(platform_data_dir)/mozcdic-ut0425し.txt',
      '<(platform_data_dir)/mozcdic-ut0425す.txt',
      '<(platform_data_dir)/mozcdic-ut0425せ.txt',
      '<(platform_data_dir)/mozcdic-ut0425そ.txt',
      '<(platform_data_dir)/mozcdic-ut0425た.txt',
      '<(platform_data_dir)/mozcdic-ut0425ち.txt',
      '<(platform_data_dir)/mozcdic-ut0425つ.txt',
      '<(platform_data_dir)/mozcdic-ut0425て.txt',
      '<(platform_data_dir)/mozcdic-ut0425と.txt',
      '<(platform_data_dir)/mozcdic-ut0425な.txt',
      '<(platform_data_dir)/mozcdic-ut0425に.txt',
      '<(platform_data_dir)/mozcdic-ut0425ぬ.txt',
      '<(platform_data_dir)/mozcdic-ut0425ね.txt',
      '<(platform_data_dir)/mozcdic-ut0425の.txt',
      '<(platform_data_dir)/mozcdic-ut0425は.txt',
      '<(platform_data_dir)/mozcdic-ut0425ひ.txt',
      '<(platform_data_dir)/mozcdic-ut0425ふ.txt',
      '<(platform_data_dir)/mozcdic-ut0425へ.txt',
      '<(platform_data_dir)/mozcdic-ut0425ほ.txt',
      '<(platform_data_dir)/mozcdic-ut0425ま.txt',
      '<(platform_data_dir)/mozcdic-ut0425み.txt',
      '<(platform_data_dir)/mozcdic-ut0425む.txt',
      '<(platform_data_dir)/mozcdic-ut0425め.txt',
      '<(platform_data_dir)/mozcdic-ut0425も.txt',
      '<(platform_data_dir)/mozcdic-ut0425や.txt',
      '<(platform_data_dir)/mozcdic-ut0425ゆ.txt',
      '<(platform_data_dir)/mozcdic-ut0425よ.txt',
      '<(platform_data_dir)/mozcdic-ut0425ら.txt',
      '<(platform_data_dir)/mozcdic-ut0425り.txt',
      '<(platform_data_dir)/mozcdic-ut0425る.txt',
      '<(platform_data_dir)/mozcdic-ut0425れ.txt',
      '<(platform_data_dir)/mozcdic-ut0425ろ.txt',
      '<(platform_data_dir)/mozcdic-ut0425わ.txt',
      '<(platform_data_dir)/mozcdic-ut0425を.txt',
      '<(platform_data_dir)/mozcdic-ut0425ん.txt',
      '<(mozc_oss_src_dir)/data/dictionary_manual/domain.txt',
    ],
    # Hex-escaped string of "\xEFMOZC\r\n"
    'magic_number': "\\xEF\\x4D\\x4F\\x5A\\x43\\x0D\\x0A",
    'magic_number_length': '7',
    'mozc_data_varname': 'kOssMozcDataSet',
    'out_mozc_data': 'mozc.data',
    'out_mozc_data_header': 'mozc_data.inc',
    'additional_dendencies': [],
  },
  # This 'includes' defines the following targets:
  #   - oss_data_manager  (type: static_library)
  #   - gen_separate_dictionary_data_for_oss (type: none)
  #   - gen_separate_connection_data_for_oss (type: none)
  #   - gen_oss_embedded_data  (type: none)
  'includes': [ '../data_manager.gypi' ],
}
