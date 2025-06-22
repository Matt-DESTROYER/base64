#!/bin/bash

echo "Running valgrind check."
valgrind -s --leak-check=full --show-leak-kinds=all ../build/base64 Man

echo ""
echo "Running test cases."
tests=("Man" "Ma" "M" "Many hands make light work." "light work." "light work" "light wor" "light wo" "light w")
expected=("TWFu" "TWE=" "TQ==" "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu" "bGlnaHQgd29yay4=" "bGlnaHQgd29yaw==" "bGlnaHQgd29y" "bGlnaHQgd28=" "bGlnaHQgdw==")

for test in "${tests[@]}"; do
    echo ""
    echo "Testing '$test'"
    ../build/base64 "$test"
done
