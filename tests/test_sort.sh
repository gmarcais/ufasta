#! /bin/bash

. ./common.sh

SORT_SIZES=$(ufasta sort test1.fasta | ufasta sizes -H /dev/fd/0)
SIZES_SORT=$(ufasta sizes -H test1.fasta | sort -k1,1)
EXPECT_EQ "$SIZES_SORT" "$SORT_SIZES" "Sorted sizes"

SORT_SORT=$(ufasta sort test1.fasta | sort)
SORT_ONCE=$(sort test1.fasta)
EXPECT_EQ "$SORT_ONCE" "$SORT_SORT" "Sorted content"

SORT_SIZES=$(ufasta sort -C 5 -n test1.fasta | ufasta sizes -H /dev/fd/0)
SIZES_SORT=$(ufasta sizes -H test1.fasta | sort -n -k1.5,1.10)
EXPECT_EQ "$SIZES_SORT" "$SORT_SIZES" "Sorted numeric sizes"

SORT_SORT=$(cat test1.fasta | ufasta sort -C 5 -n /dev/fd/0 | sort)
SORT_ONCE=$(sort test1.fasta)
EXPECT_EQ "$SORT_ONCE" "$SORT_SORT" "Sorted numeric content"
