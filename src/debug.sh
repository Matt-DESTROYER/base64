#!/bin/bash

. ./detect-runner.sh

echo "Running valgrind check..."
echo ""
echo "============================================================"
echo ""
valgrind -s --leak-check=full --show-leak-kinds=all ../build/$RUNNER_OS/$ARCH/base64$EXT "Many hands make light work."
echo ""
echo "============================================================"

echo ""
echo "Running test cases."
tests=("Man" "Ma" "M" "Many hands make light work." "light work." "light work" "light wor" "light wo" "light w", "--file ./test.txt")
expected=("TWFu" "TWE=" "TQ==" "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu" "bGlnaHQgd29yay4=" "bGlnaHQgd29yaw==" "bGlnaHQgd29y" "bGlnaHQgd28=" "bGlnaHQgdw==", "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu")

for test in "${tests[@]}"; do
	echo ""
	echo "Testing '$test'"
	../build/$RUNNER_OS/$ARCH/base64$EXT "$test"
done
