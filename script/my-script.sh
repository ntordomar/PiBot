#! /bin/bash

set -e
ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT/.."



for test in $(ls test/accept/); do

cat "test/accept/$test"  | ./bin/Compiler "$test.tex"
done;
