#!/usr/bin/env bash
set -e

THIS="$(realpath "$(dirname "${BASH_SOURCES[0]}")")"
BUILD="$THIS/build"
TEST_IMAGES="$THIS/test-images"
PROG="$BUILD/progko"

mkdir -p "$BUILD" && cd "$BUILD"
cmake ..
make

cd "$TEST_IMAGES"

for img in */*-out*; do
  echo "rm '$img'"
  rm "$img"
done

for img in */*; do
  echo "-------------------------" | tee -a log.txt
  echo "Testing: '$img'" | tee -a log.txt
  multitime -q -n 10 "$PROG" gray "$img" 2>&1 | tee -a log.txt
done
