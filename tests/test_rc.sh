#! /bin/sh

. ./common.sh

ori=$(cat test1.fasta | wc)
rc=$(ufasta rc test1.fasta | wc)
EXPECT_EQ "$ori" "$rc" "Output same length"

ori=$(cat test1.fasta)
id=$(ufasta rc test1.fasta | ufasta rc /dev/fd/0)
EXPECT_EQ "$ori" "$id" "Involution"

paste <(ufasta rc test1.fasta | ufasta one /dev/fd/0 | grep -v '^>') <(ufasta one test1.fasta | grep -v '^>') | \
    perl -ane '$F[0] = uc($F[0]); $F[1] =~ tr/ACGTacgt/TGCATGCA/; $F[1] = reverse($F[1]); exit(1) unless $F[0] eq $F[1]'
EXPECT_SUCCESS "Reverse complement"

ori=$(cat test1.fasta | wc)
can=$(ufasta rc -C test1.fasta | wc)
EXPECT_EQ "$ori" "$rc" "Output -C same length"

first=$(ufasta rc -C test1.fasta)
second=$(echo "$first" | ufasta rc -C /dev/fd/0)
EXPECT_EQ "$first" "$second" "Projection"

paste <(ufasta rc -C test1.fasta | ufasta one /dev/fd/0 | grep -v '^>') <(ufasta one test1.fasta | grep -v '^>') | \
    perl -ane '$F[0] = uc($F[0]); $F[1] = uc($F[1]); $rc = reverse($F[1]); $rc =~ tr/ACGT/TGCA/; unless(($F[0] eq $rc || $F[0] eq $F[1]) && $F[0] cmp $F[1] <= 0) { print("$.\n0 $F[0]\n1 $F[1]\n2 $rc"); exit(1) }'
EXPECT_SUCCESS "Canonical"
