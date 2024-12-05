ROOT="$HERO_ROOT"
EXE=${ROOT}/bin/tc
# If you have download the full dataset, please uncomment the script below to obtain the complete experimental results.
GRAPHS=(
    twitter \
    # gplus \
    # google \
    # youtube \
    # berkstan \
    # flickr \
    # skitter \
    # pokec \
    # livejournal \
    # wiki
)

cd ${ROOT}
mkdir -p exp
mkdir -p exp/tc
for G in "${GRAPHS[@]}"; do
    echo processing graph ${G}
    ${EXE} ${G} > ${ROOT}/exp/tc/${G}.log
done
