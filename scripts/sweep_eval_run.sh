#!/usr/bin/env bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="$(cd "$SCRIPT_DIR/../efds/tests" && pwd)"
OUTPUT="$SCRIPT_DIR/Sweep_accuracy_metrics.txt"

> "$OUTPUT"

run_eval() {
    local label="$1"
    local dir="$2"

    echo "====${label}=====" >> "$OUTPUT"

    if [ ! -d "$dir" ]; then
        echo "Directory $dir not found, skipping." >> "$OUTPUT"
        echo "" >> "$OUTPUT"
        return
    fi

    if [ ! -f "$dir/runme_eval" ]; then
        echo "runme_eval not found in $dir, skipping." >> "$OUTPUT"
        echo "" >> "$OUTPUT"
        return
    fi

    (cd "$dir" && bash runme_eval)

    if [ -f "$dir/evalEfds.txt" ]; then
        cat "$dir/evalEfds.txt" >> "$OUTPUT"
        rm "$dir/evalEfds.txt"
    else
        echo "evalEfds.txt not generated for ${label}." >> "$OUTPUT"
    fi

    echo "" >> "$OUTPUT"
}

for i in 0 1 2 3 4 5 6; do
    run_eval "train${i}" "$TESTS_DIR/test_train${i}"
done

run_eval "narrow" "$TESTS_DIR/test_train_narrow"
run_eval "normal" "$TESTS_DIR/test_train_normal"
run_eval "wide" "$TESTS_DIR/test_train_wide"
echo "Done. Results written to $OUTPUT"
