
n=$(($(ulimit -n) - 20))
n=$((n > 2000 ? 2000 : n))

for i in $(seq 1 $n); do
    oarray+=("splita$i")
done

split_clean() {
    rm -f split1 split2 splita0 "${oarray[@]}"
}
trap split_clean EXIT

set -x

ufasta split -i "$TEST1" split1 split2
diff <(cat split1 split2 | ufasta sort) <(ufasta sort $TEST1)
EXPECT_SUCCESS "Split join"

random_sequence | ufasta head -n $((n * 10)) | tee splita0 | ufasta split "${oarray[@]}"
EXPECT_EQ "$(cat "${oarray[@]}"  | ufasta sort | md5sum)" "$(ufasta sort splita0 | md5sum)" "Split $n"
