#!/bin/bash

if [ -z "${C_COMPILER:-}" ]; then
	export C_COMPILER="gcc"
fi

. ./detect-runner.sh

mkdir -p ../build/$RUNNER_OS/$ARCH

$C_COMPILER ./main.c ./base64.c -o ../build/$RUNNER_OS/$ARCH/base64$EXT -Wall -Wextra -pedantic
