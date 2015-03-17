#! /bin/sh

. ./common.sh

RES=$(ufasta dgrep 'CacGACgcggatGaGTcGcagAgccG.*GcGatcGcCtCacAGCAaGTtC' test1.fasta)
EXPECT_EQ ">read6" $(echo "$RES" | head -n 1) "Got read6"

RES=$(ufasta dgrep 'acgt' test1.fasta | grep -v '^>')
GR=$(grep 'acgt' test1.fasta)
EXPECT_EQ "$GR" "$RES" "Identical to grep"

RES=$(ufasta dgrep -n 'act' test1.fasta | grep -v '>')
GR=$(grep -n 'act' test1.fasta)
EXPECT_EQ "$GR" "$RES" "Identical to grep, line numbers"
