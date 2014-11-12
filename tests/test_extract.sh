#! /bin/bash

. ./common.sh

diff <(ufasta hgrep '^read(4|5|7|10|32)\b' test1.fasta) \
    <(ufasta extract -n read10 -n read5 -n toto -f <(echo read4 read5 blah read7) -f <(echo read32) -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract 5 reads"

diff <(ufasta hgrep -v '^read(4|5|7|10|32)\b' test1.fasta) \
    <(ufasta extract -v -n read10 -n read5 -n toto -f <(echo read4 read5 blah read7) -f <(echo read32) -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract complement 5 reads"

diff <(ufasta hgrep -v -m 4 '^read(4|5|7|10|32)\b' test1.fasta) \
    <(ufasta extract -v -m 4 -n read10 -n read5 -n toto -f <(echo read4 read5 blah read7) -f <(echo read32) -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract complement 4 reads"
