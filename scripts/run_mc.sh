ROOT="$HERO_ROOT"
EXE=${ROOT}/bin/mc
# If you have download the full dataset, please uncomment the script below to obtain the complete experimental results.
GRAPHS=(
    twitter \
    # google \
    # youtube \
    # berkstan \
    # skitter \
    # pokec \
    # wiki
)

cd ${ROOT}
mkdir -p exp
mkdir -p exp/mc
for G in "${GRAPHS[@]}"; do
    echo processing graph ${G}
    ${EXE} ${G} > ${ROOT}/exp/mc/${G}.log
done
