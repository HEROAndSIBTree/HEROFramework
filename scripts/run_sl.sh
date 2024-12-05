ROOT="$HERO_ROOT"
EXE=${ROOT}/bin/sl
PATTERN=$1
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
mkdir -p exp/sl
mkdir -p exp/sl/${PATTERN}
for G in "${GRAPHS[@]}"; do
    echo processing graph ${G}
    ${EXE} ${G} ${PATTERN} > ${ROOT}/exp/sl/${PATTERN}/${G}.log
done
