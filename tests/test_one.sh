#! /bin/bash

. ./common.sh

diff <(grep '^>' test1.fasta) <(ufasta one test1.fasta | grep '^>')
EXPECT_SUCCESS "Identical headers"

diff <(grep -v '^>' test1.fasta | perl -ne 'chomp; print') <(ufasta one test1.fasta | grep -v '^>' | perl -ne 'chomp; print')
EXPECT_SUCCESS "Identical sequence"

first=$((echo -e 'toto\ntata'; cat test1.fasta) | ufasta one /dev/fd/0 | head -n 2)
expect=$(echo -e 'toto\ntata')
EXPECT_EQ "$expect" "$first" "Keep pre-header"
