#! /bin/sh

. ./common.sh

NB=$(grep -c '^>' test1.fasta)
for i in $(seq 0 $((NB - 1))); do
    ufasta dsort test1.fasta | ufasta extract -n "read$i" /dev/fd/0 | tail -n +2 | sort -C >/dev/null
    EXPECT_SUCCESS "Sorted entry $i"
done
