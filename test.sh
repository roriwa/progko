#!/usr/bin/env bash
set -e
shopt -s nullglob

N=10
ALGO="plain-grayscale"

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
  multitime -q -n $N "$PROG" "$ALGO" "$img" 2>&1 | tee -a log.txt
done
