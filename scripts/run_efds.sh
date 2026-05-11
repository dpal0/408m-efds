#!/usr/bin/env bash
# run_efds.sh
# Usage: ./run_efds.sh <input_dir> <config_dir> <output_dir>

if [ $# -ne 3 ]; then
    echo "Usage: $0 <input_dir> <config_dir> <output_dir>"
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DRIVER="$SCRIPT_DIR/../efds/tests/efds_graph_driver.exe"

if [ ! -f "$DRIVER" ]; then
    echo "Driver not found: $DRIVER"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Input directory not found: $1"
    exit 1
fi

if [ ! -d "$2" ]; then
    echo "Config directory not found: $2"
    exit 1
fi

INPUT_DIR="$(cd "$1" && pwd)"
CONFIG_DIR="$(cd "$2" && pwd)"
OUTPUT_DIR="$3"

mkdir -p "$OUTPUT_DIR"
OUTPUT_DIR="$(cd "$OUTPUT_DIR" && pwd)"

# Run from output_dir so nf_stage*.txt files are written there
cd "$OUTPUT_DIR"
rm -f nf_stage*.txt temp_face.txt face.txt performance.txt

"$DRIVER" "$INPUT_DIR" temp_face.txt "$CONFIG_DIR" > performance.txt

cat temp_face.txt nf_stage*.txt 2>/dev/null | sort -n > face.txt
rm -f temp_face.txt

echo "Done. Results written to $OUTPUT_DIR"
