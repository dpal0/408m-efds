#!/bin/bash
# runme_subchain
#
# Runs the EFDS graph once on face testing set and once on non-face testing set
# for a given config directory, preserves per-stage abort files, then evaluates
# sub-chains k=1..L by reconstructing what each k-stage cascade would classify.
#
# Usage: ./runme_subchain <config_name>
# Example: ./runme_subchain train0

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <config_name>"
    exit 1
fi

CONFIG_NAME="$1"
INPUT_DIR="../../../dataset/project-dataset/gray/testing"
CONFIG_DIR="../../configs/${CONFIG_NAME}"

if [ ! -d "$CONFIG_DIR" ]; then
    echo "Config directory not found: $CONFIG_DIR"
    exit 1
fi

SUMMARY_DIR="./Per_stage_summary"
mkdir -p "$SUMMARY_DIR"

# Count number of stages L by counting .txt files in the config dir.
L=$(ls "$CONFIG_DIR"/*.txt 2>/dev/null | wc -l)
if [ "$L" -eq 0 ]; then
    echo "No config files found in $CONFIG_DIR"
    exit 1
fi
echo "Chain $CONFIG_NAME has L=$L stages"

# Clean previous artifacts.
rm -f temp_face.txt temp_non-face.txt nf_stage*.txt performance*.txt
rm -f face_nf_stage*.txt nonface_nf_stage*.txt
rm -f face.txt non-face.txt
rm -f "subchain_${CONFIG_NAME}_k"*.txt
rm -f "subchain_${CONFIG_NAME}_summary.csv"

# --- Run on face input ---
echo "Running on face input..."
./efds_graph_driver.exe "${INPUT_DIR}/face" temp_face.txt "$CONFIG_DIR" \
    > performance-face.txt

# Preserve face-run per-stage aborts.
for f in nf_stage*.txt; do
    [ -e "$f" ] || continue
    mv "$f" "face_${f}"
done

# --- Run on non-face input ---
echo "Running on non-face input..."
./efds_graph_driver.exe "${INPUT_DIR}/non-face" temp_non-face.txt "$CONFIG_DIR" \
    > performance-non-face.txt

# Preserve non-face-run per-stage aborts.
for f in nf_stage*.txt; do
    [ -e "$f" ] || continue
    mv "$f" "nonface_${f}"
done

# --- Build sub-chain inputs and evaluate ---
#SUMMARY="subchain_${CONFIG_NAME}_summary.csv"
SUMMARY="${SUMMARY_DIR}/subchain_${CONFIG_NAME}_summary.csv"
echo "config,k,TP,FP,TN,FN,FPR,TPR,Precision,F1" > "$SUMMARY"

for ((k=1; k<=L; k++)); do
    echo "Evaluating sub-chain k=$k..."

    # Face-input reconstruction for k-stage cascade:
    #   - Aborts at stage j with j < k: still aborted -> NF (FN)
    #   - Aborts at stage j with j >= k: would have survived -> F (TP)
    #   - temp_face.txt: survived all L stages -> F (TP)
    : > face.txt
    for ((j=0; j<k; j++)); do
        [ -e "face_nf_stage${j}.txt" ] && cat "face_nf_stage${j}.txt" >> face.txt
    done
    for ((j=k; j<L; j++)); do
        if [ -e "face_nf_stage${j}.txt" ]; then
            awk '{print $1, "F"}' "face_nf_stage${j}.txt" >> face.txt
        fi
    done
    cat temp_face.txt >> face.txt
    sort -n -o face.txt face.txt

    # Non-face-input reconstruction (symmetric):
    : > non-face.txt
    for ((j=0; j<k; j++)); do
        [ -e "nonface_nf_stage${j}.txt" ] && cat "nonface_nf_stage${j}.txt" >> non-face.txt
    done
    for ((j=k; j<L; j++)); do
        if [ -e "nonface_nf_stage${j}.txt" ]; then
            awk '{print $1, "F"}' "nonface_nf_stage${j}.txt" >> non-face.txt
        fi
    done
    cat temp_non-face.txt >> non-face.txt
    sort -n -o non-face.txt non-face.txt

    # Run evaluator.
    OUT="subchain_${CONFIG_NAME}_k${k}.txt"
    ./evalTrain_driver.exe > "$OUT"

    # Extract metrics into CSV row.
    TP=$(grep -m1 "^TP:" "$OUT" | awk '{print $2}')
    FP=$(grep -m1 "^FP:" "$OUT" | awk '{print $2}')
    TN=$(grep -m1 "^TN:" "$OUT" | awk '{print $2}')
    FN=$(grep -m1 "^FN:" "$OUT" | awk '{print $2}')
    FPR=$(grep -m1 "^FPR:" "$OUT" | awk '{print $2}')
    TPR=$(grep -m1 "^TPR" "$OUT" | awk '{print $2}')
    PREC=$(grep -m1 "^Precision:" "$OUT" | awk '{print $2}')
    F1=$(grep -m1 "^F1-Score:" "$OUT" | awk '{print $2}')

    echo "${CONFIG_NAME},${k},${TP},${FP},${TN},${FN},${FPR},${TPR},${PREC},${F1}" \
        >> "$SUMMARY"
done

echo ""
echo "Done. Summary written to $SUMMARY"
cat "$SUMMARY"
# Clean up intermediate per-stage and per-k files; keep the summary CSV.
rm -f face_nf_stage*.txt nonface_nf_stage*.txt
rm -f nf_stage*.txt
rm -f temp_face.txt temp_non-face.txt
rm -f face.txt non-face.txt
rm -f performance-face.txt performance-non-face.txt
rm -f "subchain_${CONFIG_NAME}_k"*.txt
