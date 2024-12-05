ROOT="$HERO_ROOT"
EXE=${ROOT}/bin/pt
OPT=$1

cd ${ROOT}
mkdir -p exp
mkdir -p exp/pt
echo ${ROOT}
${EXE} ${OPT} > ${ROOT}/exp/pt/${OPT}.log