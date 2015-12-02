#! /bin/sh

. ./common.sh

ufasta split -i test1.fasta >(cat > split1) >(cat > split2)
diff <(cat split1 split2 | ufasta sort) <(ufasta sort test1.fasta)
EXPECT_SUCCESS "Split join"
rm split1 split2
