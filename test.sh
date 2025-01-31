#!/usr/bin/env bash
set -e
shopt -s nullglob

function join_by {
  local IFS="$1"
  shift
  echo "$*"
}

NL=$'\n'

METHODS=("plain" "omp" "mpi" "mpi-omp" "cv2")
ALGORITHMS=("grayscale" "hsv" "emboss")

THIS="$(realpath "$(dirname "${BASH_SOURCES[0]}")")"
BUILD="$THIS/build"
TEST_IMAGES="$THIS/test-images"
PROG="$BUILD/progko"

mkdir -p "$BUILD" && cd "$BUILD"
cmake ..
make -j

cd "$TEST_IMAGES"

for ALGO in "${ALGORITHMS[@]}"; do
  echo "Testing '$ALGO'"
#  for IMGDIR in */; do
  for IMGDIR in "dice/" "giant-images"; do
    echo "In Directory '$IMGDIR'"
    for img in "${IMGDIR%/}"/*-out*; do
      rm "$img"
    done
    OUTPUT="$ALGO,$(join_by ',' "${METHODS[@]}")$NL"
    for IMG in "${IMGDIR%/}"/*; do
      echo "On File '$IMG'"
      OUTPUT+="$(basename "$IMG")"
      for METHOD in "${METHODS[@]}"; do
        echo "With Method '$METHOD'"
        CMD=("$PROG")
        if [[ "$METHOD" =~ ^mpi ]]; then
          CMD=("mpirun" "$PROG")
        fi
        TIME="$(for _ in {1..5}; do
          NO_SHOW=1 "${CMD[@]}" "$METHOD-$ALGO" "$IMG"
          echo
          sleep 0.5
        done | sort -n | head -n 1)"
        OUTPUT+=",$TIME"
        sleep 1
      done
      OUTPUT+="$NL"
      sleep 1
    done
    echo "Writing CSV '$ALGO-${IMGDIR%/}.csv'"
    echo -n "$OUTPUT" > "$ALGO-${IMGDIR%/}.csv"
#    exit 0
  done
done
