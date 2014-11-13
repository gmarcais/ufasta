#! /bin/bash

. ./common.sh

diff <(ufasta one test1.fasta) <(ufasta format -l 15 test1.fasta | ufasta one test1.fasta)
EXPECT_SUCCESS "Format then one"

ufasta format -l 15 test1.fasta | grep -v '^>' | perl -ne 'chomp; length($_) <= 15 || exit(1)'
EXPECT_SUCCESS "Short lines"

diff <(ufasta one test1.fasta | grep -v '^>' | tr '[:upper:]' '[:lower:]') <(ufasta format -L test1.fasta | ufasta one /dev/fd/0 | grep -v '^>')
EXPECT_SUCCESS "Lower case"

diff <(ufasta one test1.fasta | grep -v '^>' | tr '[:lower:]' '[:upper:]') <(ufasta format -U test1.fasta | ufasta one /dev/fd/0 | grep -v '^>')
EXPECT_SUCCESS "Uppr case"
