#!/bin/bash

DIRNAME=$(dirname "$0")

for DIR in "$DIRNAME"/*/; do
  if [ -d "$DIR" ]; then
    basename $DIR
    (cd "$DIR" && g++ main.cpp && ./a.out)
  fi
done
