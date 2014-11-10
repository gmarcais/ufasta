#! /bin/sh

. ./common.sh

diff <(ufasta one test1.fasta | grep -A 1 '^>read1' | grep -v -- --) <(ufasta hgrep '^read1' test1.fasta | ufasta one /dev/fd/0)
EXPECT_SUCCESS "Grep for ^read1"
