#! /bin/sh

. ./common.sh

ori=$(cat test1.fasta | wc)
rc=$(ufasta rc test1.fasta | wc)
EXPECT_EQ "$ori" "$rc" "Output same length"

ori=$(cat test1.fasta)
id=$(ufasta rc test1.fasta | ufasta rc /dev/fd/0)
EXPECT_EQ "$ori" "$id" "Involution"
