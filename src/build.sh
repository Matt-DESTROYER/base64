#!/bin/bash

. ./detect-runner.sh

if [ "$OSTYPE" = "Windows" ]; then
	export EXT=".exe"
fi

mkdir -p ../build/$RUNNER_OS/$ARCH

gcc ./main.c ./base64.c -o ../build/$RUNNER_OS/$ARCH/base64$EXT -Wall -Wextra -pedantic
