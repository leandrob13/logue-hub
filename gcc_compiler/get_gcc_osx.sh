#!/bin/bash
#
# BSD 3-Clause License
#
# Copyright (c) 2018, KORG INC.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

SCRIPT_DIR="$(pwd)/$(dirname $0)"

pushd ${SCRIPT_DIR} 2>&1 > /dev/null

PKGNAME="gcc-arm-none-eabi"
VERSION="5-2016-q3"

ARCHIVE_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/5_4-2016q3/gcc-arm-none-eabi-5_4-2016q3-20160926-mac.tar.bz2"
ARCHIVE_SHA1="defb787491a64d24f25b2e3c190170f735d4f14d"
ARCHIVE_NAME="gcc-arm-none-eabi-5_4-2016q3-20160926-mac.tar.bz2"

if [[ "${OSTYPE}" == "darwin"* ]]; then
    echo ">> Assuming OSX 64 bit platform."
else
    echo ">> This script is meant for OSX."
    popd 2>&1 > /dev/null
    exit 1
fi

# assert_success(fail_msg)
assert_success() {
    [[ $? -eq 0 ]] && return 0
    [[ ! -z "$1" ]] && echo "Error: $1" 
    popd 2>&1 > /dev/null
    return 1
}

AWK=$(which awk) || assert_success "dependency not found..." || exit $?

CURL=$(which curl) || assert_success "dependency not found..." || exit $?

TAR=$(which tar) || assert_success "dependency not found..." || exit $?

SHA1SUM=$(which shasum) || assert_success "dependency not found..." || exit $?

# test_sha1sum(sha1, path_to_file)
test_sha1sum() {
    SHA1=$(${SHA1SUM} "$2" | ${AWK} '{print $1};')
    [[ "${SHA1}" != "$1" ]] && return 1
    return 0
}

if [[ ! -f "${ARCHIVE_NAME}" ]]; then
    echo ">> Downloading..."
    ${CURL} -# -L -o "${ARCHIVE_NAME}" "${ARCHIVE_URL}"
    assert_success "Download of ${ARCHIVE_NAME} failed..." || exit $?
fi

test_sha1sum "${ARCHIVE_SHA1}" "${ARCHIVE_NAME}"
assert_success "SHA1 mismatch. Try redownloading the archive..." || exit $?

echo ">> Unpacking..."
${TAR} -jxf "${ARCHIVE_NAME}"
assert_success "Could not unpack archive..." || exit $?

echo ">> Cleaning up..."
rm -f "${ARCHIVE_NAME}"
assert_success "Could not delete temporary archive..." || exit $?

popd 2>&1 > /dev/null

