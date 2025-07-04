#!/bin/bash

. ./detect-runner.sh

passed_checks=0
failed_checks=0

echo "Running valgrind check..."
echo ""
#echo "============================================================"
#echo ""
if valgrind -s --leak-check=full --show-leak-kinds=all ../build/$RUNNER_OS/$ARCH/base64$EXT "Many hands make light work." &> /dev/null; then
	echo "Valgrind check passed!"
	passed_checks=$((passed_checks + 1))
else
	echo "Valgrind check failed..."
	failed_checks=$((failed_checks + 1))
fi
#echo ""
#echo "============================================================"

echo ""
echo "Running test cases."
tests=("Man" "Ma" "M" "Many hands make light work." "light work." "light work" "light wor" "light wo" "light w")
expected=("TWFu" "TWE=" "TQ==" "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu" "bGlnaHQgd29yay4=" "bGlnaHQgd29yaw==" "bGlnaHQgd29y" "bGlnaHQgd28=" "bGlnaHQgdw==")

for i in "${!tests[@]}"; do
	test="${tests[$i]}"
	echo ""
	echo "Testing '$test'"

	output=$(../build/$RUNNER_OS/$ARCH/base64$EXT "$test" | tr -d '\n') # tr -d '\n' to remove potential trailing newlines

	if [ "$output" == "${expected[$i]}" ]; then
		echo "Test passed!"
		passed_checks=$((passed_checks + 1))
	else
		echo "Test failed..."
		echo "Output: '$output'"
		echo "Expected: '${expected[$i]}'"
		failed_checks=$((failed_checks + 1))
	fi
done

echo ""
if [ ! -f man_test.txt ]; then
    echo "Man" > man_test.txt
fi
output=$(../build/$RUNNER_OS/$ARCH/base64$EXT --file -f ./man_test.txt | tr -d '\n') # tr -d '\n' to remove potential trailing newlines
if [ "$output" == "TWFuCgA=" ]; then
	echo "File input test passed!"
	passed_checks=$((passed_checks + 1))
else
	echo "File input test failed..."
	echo "Output: '$output'"
	echo "Expected: 'TWFuCgA='"
	failed_checks=$((failed_checks + 1))
fi

echo ""
echo "Passed $passed_checks of $((passed_checks + failed_checks)) checks."
if [ $failed_checks -eq 0 ]; then
	echo "All tests passed successfully!"
	exit 0
else
	echo "Some tests failed; please check the output above."
	exit 1
fi
