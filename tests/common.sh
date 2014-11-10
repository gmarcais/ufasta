success() {
    echo "[ OK ] $1"
    true
}

fail() {
    echo "[FAIL] $1"
    false
}

EXPECT_EQ() {
    test "$1" = "$2" && success "$3" || fail "$3. Expected '$1' got '$2'"
}

EXPECT_SUCCESS() {
    test "$?" = "0" && success "$1" || fail "$1"
}

PATH=..:$PATH

set +o | grep pipefail > /dev/null && set -o pipefail
set -o errexit
