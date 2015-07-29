#! /bin/bash

. ./common.sh

EXPECT_EQ "" "$(ufasta extract test1.fasta | head -n 1)" "Extract empty"
diff test1.fasta <(ufasta extract -v -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract all"

diff <(ufasta hgrep '^read(4|5|7|10|32)\b' test1.fasta) \
    <(ufasta extract -n read10 -n read5 -n toto -f <(echo read4 read5 blah read7) -f <(echo read32) -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract 5 reads"

diff <(ufasta hgrep -v '^read(4|5|7|10|32)\b' test1.fasta) \
    <(ufasta extract -v -n read10 -n read5 -n toto -f <(echo read4 read5 blah read7) -f <(echo read32) -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract complement 5 reads"

diff <(ufasta hgrep -v -m 4 '^read(4|5|7|10|32)\b' test1.fasta) \
    <(ufasta extract -v -m 4 -n read10 -n read5 -n toto -f <(echo read4 read5 blah read7) -f <(echo read32) -f /dev/null test1.fasta)
EXPECT_SUCCESS "Extract complement 4 reads"

NBT=$(grep -c '^>' test1.fasta)
NBE=$(ufasta extract -p 0.5 test1.fasta | grep -c '^>')
# Small probability to fail
perl -e 'exit(!($ARGV[1] >= $ARGV[0] / 4 && $ARGV[1] <= 3 * $ARGV[0] / 4))' $NBT $NBE
EXPECT_SUCCESS "Extract random number of reads"
