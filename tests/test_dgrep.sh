#! /bin/sh

. ./common.sh

RES=$(ufasta dgrep 'CacGACgcggatGaGTcGcagAgccG.*GcGatcGcCtCacAGCAaGTtC' test1.fasta)
EXPECT_EQ ">read6" $(echo "$RES" | head -n 1) "Got read6"
