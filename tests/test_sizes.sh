#! /bin/sh

. ./common.sh

ES=$(ufasta n50 -H -S test1.fasta)
S=$(ufasta sizes test1.fasta | perl -ane '$c += $F[0]; END { print $c, "\n" }')
EXPECT_EQ "$ES" "$S" "Sum of sizes"

offset=0
ufasta sizes -i test1.fasta | while read s o eo; do
    test $offset -lt $o || fail "Offset not increasing enough"
    offset=$((o + s))
done
EXPECT_SUCCESS "Offset increment"

i=0
ufasta sizes -i test1.fasta | while read s o eo; do
    diff <(ufasta hgrep "^read$i\b" test1.fasta | tail -n +2) \
        <(tail -c +$((o + 1)) test1.fasta | head -c $((eo - o)))
    EXPECT_SUCCESS "Compare sequence $i"
    i=$((i+1))
done
