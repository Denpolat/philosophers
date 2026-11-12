#!/bin/bash

PHILO="${1:-./philo}"
TIMEOUT=20

GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
RESET="\033[0m"

PASS=0
FAIL=0

if [ ! -x "$PHILO" ]; then
    echo -e "${RED}Erreur : '$PHILO' introuvable.${RESET}"
    exit 1
fi

# vg_test <outil> <description> <args...>
vg_test()
{
    local tool="$1"
    local desc="$2"
    shift 2
    local opts

    if [ "$tool" = "memcheck" ]; then
        opts="--leak-check=full --show-leak-kinds=all --track-origins=yes"
    else
        opts="--tool=$tool"
    fi

    timeout "$TIMEOUT" valgrind $opts --error-exitcode=42 \
        "$PHILO" "$@" > /dev/null 2> /tmp/vg_log.txt
    local ret=$?

    if [ $ret -eq 42 ]; then
        echo -e "${RED}[FAIL]${RESET} [$tool] $desc"
        echo "       args: $*"
        grep -E "ERROR SUMMARY|definitely lost|Possible data race" \
            /tmp/vg_log.txt | head -3 | sed 's/^/       /'
        ((FAIL++))
    else
        echo -e "${GREEN}[PASS]${RESET} [$tool] $desc"
        ((PASS++))
    fi
}

echo -e "${BLUE}=== Memcheck : erreurs de parsing (leaks ?) ===${RESET}"
vg_test memcheck "Aucun argument"
vg_test memcheck "Arguments manquants"       5 800 200
vg_test memcheck "Lettres"                   abc 800 200 200
vg_test memcheck "Négatif"                   -5 800 200 200
vg_test memcheck "Overflow"                  99999999999 800 200 200
vg_test memcheck "0 philosophes"             0 800 200 200

echo -e "\n${BLUE}=== Memcheck : exécutions valides ===${RESET}"
vg_test memcheck "1 philo (meurt)"           1 800 200 200
vg_test memcheck "5 philos, 3 repas"         5 800 200 200 3
vg_test memcheck "4 philos 310 (un meurt)"   4 310 200 100

echo -e "\n${BLUE}=== Helgrind : data races ===${RESET}"
vg_test helgrind "5 philos, 3 repas"         5 800 200 200 3
vg_test helgrind "4 philos 310 (un meurt)"   4 310 200 100
vg_test helgrind "2 philos"                  2 800 200 200 3

echo -e "\n${BLUE}=== DRD : autre détecteur de races ===${RESET}"
vg_test drd "5 philos, 3 repas"              5 800 200 200 3

echo
echo -e "Résultat : ${GREEN}$PASS réussis${RESET} / ${RED}$FAIL échoués${RESET}"
[ $FAIL -eq 0 ] && exit 0 || exit 1