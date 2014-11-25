#! /bin/sh

. ./common.sh

ufasta n50 -H -N10 -N25 -N50 -N75 -N90 test1.fasta | sort -c -r
EXPECT_SUCCESS "Ns statistics sorted"

S=$(ufasta n50 -H -S test1.fasta)
ES=$(grep -v '^>' test1.fasta | perl -ne 'chomp; print' | wc -c)
EXPECT_EQ "$ES" "$S" "Correct sum size"

EALL=$(cat <<EOF
N10 516
N25 443
N50 314
N90 112
S 9317
A 186.34
E 314.704
EOF
)
ALL=$(ufasta n50 -N10 -N25 -N50 -N90 -E -S -A test1.fasta)
EXPECT_EQ "$EALL" "$ALL" "All statistics"

FS=$(ufasta sizes test1.fasta | ufasta n50 -f -N10 -N25 -N50 -N90 -E -S -A /dev/fd/0)
EXPECT_EQ "$EALL" "$FS" "All statistics from sizes"
