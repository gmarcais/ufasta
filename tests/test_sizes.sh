#! /bin/sh

. ./common.sh

ES=$(ufasta n50 -H -S test1.fasta)
S=$(ufasta sizes test1.fasta | perl -ane '$c += $F[0]; END { print $c, "\n" }')
EXPECT_EQ "$ES" "$S" "Sum of sizes"

offset=0
ufasta sizes -i test1.fasta | while read s o; do
    test $offset -lt $o || fail "Offset not increasing enough"
    offset=$((o + s))
done
EXPECT_SUCCESS "Offset increment"

i=0
ufasta one test1.fasta | ufasta sizes -i /dev/fd/0 | while read s o; do
    diff <(ufasta hgrep "^read$i\b" <(ufasta one test1.fasta) | tail -n +2) \
        <(tail -c +$((o + 1)) <(ufasta one test1.fasta) | head -c $((s + 1)))
    EXPECT_SUCCESS "Compare sequence $i"
    i=$((i+1))
done
