#!/bin/bash

#echo "$1"
#echo "$2"

if [ $# -ne 2 ]; then
    echo "Usage: $0 <baseline file> <file to reorder>"
    exit 1
fi

BASELINE_FILE="$1"
OTHER_FILE="$2"

cp $OTHER_FILE temp

while IFS= read -r line; do
    BASE_TAG="${line%%,*}"
    sed -n "0,/$(echo "$BASE_TAG" | sed 's/[&/\]/\\&/g')/{/$(echo "$BASE_TAG" | sed 's/[&/\]/\\&/g')/p;d;}" "$OTHER_FILE" && sed -i "0,/$(echo "$BASE_TAG" | sed 's/[&/\]/\\&/g')/{/$(echo "$BASE_TAG" | sed 's/[&/\]/\\&/g')/d;}" "$OTHER_FILE"
done < "$BASELINE_FILE"

mv temp $OTHER_FILE
