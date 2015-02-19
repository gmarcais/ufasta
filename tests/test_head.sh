#! /bin/sh

. ./common.sh

x=$(ufasta head test1.fasta | grep -c '^>')
EXPECT_EQ 10 $x "Output default number of entries"

x=$(ufasta head -n 2 test1.fasta | grep -c '^>')
EXPECT_EQ 2 $x "Output 2 entries"
x=$(ufasta head -n -2 test1.fasta | grep -c '^>')
EXPECT_EQ 48 $x "Output 48 entries"

x=$(ufasta head -n 3 test1.fasta | wc -l)
r=$(head -n $((x + 1)) test1.fasta | tail -n 1)
EXPECT_EQ ">read3" "$r" "Output correct number of lines 1"
x2=$(ufasta head -n -3 test1.fasta | wc -l)
r2=$(head -n $x2 test1.fasta | grep '^>' | tail -n 1)
EXPECT_EQ ">read46" "$r2" "Output correct number of lines 2"

l=$(ufasta head -n 3 test1.fasta)
el=$(head -n $x test1.fasta)
EXPECT_EQ "$el" "$l" "Output correct lines 1"
l=$(ufasta head -n -3 test1.fasta)
el=$(head -n $x2 test1.fasta)
EXPECT_EQ "$el" "$l" "Output correct lines 2"

l=$(ufasta head -v test1.fasta | head -n 1)
EXPECT_EQ "==> test1.fasta <==" "$l" "Output header verbose"
l=$(ufasta head test1.fasta test1.fasta | head -n 1)
EXPECT_EQ "==> test1.fasta <==" "$l" "Output header many"
l=$(ufasta head -q test1.fasta test1.fasta | head -n 1)
EXPECT_EQ ">read0" "$l" "Output header quiet"


# Test byte switch
l=$(ufasta head -c 0 test1.fasta)
EXPECT_EQ "" "$l" "-c 0"
for i in $(seq 1 50 $(wc -c < test1.fasta)); do
    l=$(ufasta head -c $i test1.fasta | grep -c '^>')
    EXPECT_LT 0 "$l" "-c $i at least one entry"
    l=$(ufasta head -c $i test1.fasta | wc -c)
    EXPECT_LE $i $l "-c $i at least $i bytes"
    
    l=$(ufasta head -c $i test1.fasta; ufasta tail -c +$i test1.fasta)
    EXPECT_EQ "$(cat test1.fasta)" "$l" "tail + head -c +$i"
done
