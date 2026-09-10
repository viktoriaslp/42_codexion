#!/bin/bash

BIN=./codexion
PASSED=0
FAILED=0

expect_error()
{
	DESC="$1"
	shift

	$BIN "$@" > /dev/null 2>&1

	if [ $? -ne 0 ]; then
		echo "✅ PASS | $DESC | $*"
		PASSED=$((PASSED + 1))
	else
		echo "❌ FAIL | $DESC | $*"
		FAILED=$((FAILED + 1))
	fi
}

expect_ok()
{
	DESC="$1"
	shift

	$BIN "$@" > /dev/null 2>&1

	if [ $? -eq 0 ]; then
		echo "✅ PASS | $DESC | $*"
		PASSED=$((PASSED + 1))
	else
		echo "❌ FAIL | $DESC | $*"
		FAILED=$((FAILED + 1))
	fi
}

echo "=== VALID INPUTS ==="

expect_ok "normal fifo" 5 800 200 200 200 3 50 fifo
expect_ok "normal edf" 5 800 200 200 200 3 50 edf
expect_ok "single coder" 1 800 200 200 200 1 50 fifo

echo
echo "=== INVALID INPUTS ==="

expect_error "missing all arguments"
expect_error "too few arguments" 5 800 200
expect_error "zero coders" 0 800 200 200 200 3 50 fifo
expect_error "negative coders" -5 800 200 200 200 3 50 fifo
expect_error "letters inside integer" 5 800abc 200 200 200 3 50 fifo
expect_error "decimal number" 5 800 200.5 200 200 3 50 fifo
expect_error "uppercase scheduler" 5 800 200 200 200 3 50 FIFO
expect_error "invalid scheduler" 5 800 200 200 200 3 50 random
expect_error "int overflow" 5 2147483648 200 200 200 3 50 fifo

echo
echo "===================="
echo "Passed: $PASSED"
echo "Failed: $FAILED"