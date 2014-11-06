success() {
    echo "[ OK ] $1"
    true
}

fail() {
    echo "[FAIL] $1"
    false
}

EXPECT_EQ() {
    [ "$1" == "$2" ] && success "$3" || fail "$3. Expected '$1' got '$2'"
}

PATH=..:$PATH

set -o errexit
set -o pipefail
