RSORT=$(ufasta hsort -R $TEST1 | ufasta rsort --prefix $TEST1)
SORT=$(perl -ne 'if(/>read(\d+)/) { printf(">%d\n", $1) } else { print }' $TEST1 | \
           ufasta sort -n /dev/stdin | \
           perl -ne 'if(/>(\d+)/) { printf(">read%d\n", $1) } else { print }')
echo $RSORT
echo $SORT
EXPECT_EQ "$SORT" "$RSORT" "Sort vs rsort"
