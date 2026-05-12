#!/bin/bash
# runme_latency.sh
# Measures avg latency/face for each k-prefix of a cascade.
# Usage: ./runme_latency.sh <config_name>

if [ $# -ne 1 ]; then
    echo "Usage: $0 <config_name>"
    exit 1
fi

CONFIG_NAME="$1"
INPUT_DIR="../../dataset/project-dataset/gray/testing"
CONFIG_DIR="../../configs/${CONFIG_NAME}"

if [ ! -d "$CONFIG_DIR" ]; then
    echo "Config directory not found: $CONFIG_DIR"
    exit 1
fi

SUMMARY_DIR="./Per_stage_summary"
mkdir -p "$SUMMARY_DIR"

# --- Cleanup from previous run ---
rm -f "${SUMMARY_DIR}/latency_${CONFIG_NAME}.csv"
rm -f nf_stage*.txt
find . -maxdepth 1 -name "tmp_config_k*" -type d -exec rm -rf {} + 2>/dev/null

L=$(ls "$CONFIG_DIR"/config[0-9]*.txt 2>/dev/null | wc -l)
if [ "$L" -eq 0 ]; then
    echo "No config files found in $CONFIG_DIR"
    exit 1
fi
echo "Chain $CONFIG_NAME has L=$L stages"

SUMMARY="${SUMMARY_DIR}/latency_${CONFIG_NAME}.csv"
echo "config,k,Tpos_ms,Npos,avg_latency_ms" > "$SUMMARY"

for ((k=1; k<=L; k++)); do
    echo "Timing k=$k..."

    # Build truncated config dir with first k files only
    TMPDIR="./tmp_config_k${k}"
    mkdir -p "$TMPDIR"
    ls "$CONFIG_DIR"/config[0-9]*.txt | sort | head -n "$k" | xargs -I{} cp {} "$TMPDIR/"

    # Run driver on face input, capture output
    OUT=$(./efds_graph_driver.exe "${INPUT_DIR}/face" /dev/null "$TMPDIR")

    Tpos=$(echo "$OUT" | grep "Tpos" | awk '{print $4}')
    Npos=$(echo "$OUT" | grep "Npos" | awk '{print $4}')
    Latency=$(echo "$OUT" | grep "Avg"  | awk '{print $3}')

    echo "${CONFIG_NAME},${k},${Tpos},${Npos},${Latency}" >> "$SUMMARY"
    echo "  Tpos=${Tpos} ms, Npos=${Npos}, Avg=${Latency} ms"

    rm -rf "$TMPDIR"
    rm -f nf_stage*.txt 
done

# --- Cleanup temp dirs (safety net in case loop exited early) ---
find . -maxdepth 1 -name "tmp_config_k*" -type d -exec rm -rf {} + 2>/dev/null
rm -f nf_stage*.txt  
echo ""
echo "Done. Summary written to $SUMMARY"
cat "$SUMMARY"
