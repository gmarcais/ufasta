#! /bin/sh

. ./common.sh

ES=$(ufasta n50 -H -S test1.fasta)
S=$(ufasta sizes test1.fasta | perl -ane '$c += $F[0]; END { print $c, "\n" }')

EXPECT_EQ "$ES" "$S" "Sum of sizes"
