#! /bin/sh

. ./common.sh

x=$(ufasta tail -n +1 test1.fasta | grep -c '^>')
EXPECT_EQ "50" $x "Output all 1"
x=$(ufasta tail -n 50 test1.fasta | grep -c '^>')
EXPECT_EQ "50" $x "Output all 2"

x=$(ufasta tail -n +2 test1.fasta | grep -c '^>')
EXPECT_EQ "49" $x "Output all but first 1"
x=$(ufasta tail -n 49 test1.fasta | grep -c '^>')
EXPECT_EQ "49" $x "Output all but first 2"

x=$(ufasta tail -n +49 test1.fasta | wc -l)
eh=$(tail -n $x test1.fasta | head -n 1)
EXPECT_EQ ">read48" "$eh" "Output last two headers"
x2=$(ufasta tail -n 2 test1.fasta | wc -l)
EXPECT_EQ $x $x2 "Complementary tail"

el=$(tail -n $x test1.fasta)
l=$(ufasta tail -n +49 test1.fasta)
EXPECT_EQ "$el" "$l" "Output last two entries 1"
l=$(ufasta tail -n 2 test1.fasta)
EXPECT_EQ "$el" "$l" "Output last two entries 2"
