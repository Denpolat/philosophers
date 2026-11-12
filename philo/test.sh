#!/bin/bash

GREEN="\033[32m"; RED="\033[31m"; YELLOW="\033[33m"; RESET="\033[0m"
PHILO=./philo

pass() { echo -e "${GREEN}[OK]${RESET} $1"; }
fail() { echo -e "${RED}[KO]${RESET} $1"; }

# Lance philo avec un timeout, retourne la sortie dans /tmp/philo_out
run() {
	timeout "$1" $PHILO "${@:2}" > /tmp/philo_out 2>&1
	return $?
}

echo -e "${YELLOW}=== 1. Arguments invalides (doit refuser, pas de crash) ===${RESET}"
for args in "" "1 2 3" "0 800 200 200" "-5 800 200 200" "abc 800 200 200" \
			"5 800 200 abc" "5 -800 200 200" "5 800 200 200 0" "5 800 200 200 -1" \
			"5 800 200 200 3 4" "5 99999999999 200 200" "" "5 800 200 200 +"; do
	timeout 2 $PHILO $args > /tmp/philo_out 2>&1
	code=$?
	if [ $code -ge 124 ]; then fail "args='$args' (code $code: crash/timeout)"
	else pass "args='$args' (code $code)"; fi
done

echo -e "\n${YELLOW}=== 2. Un seul philo : doit mourir à ~800ms ===${RESET}"
run 3 1 800 200 200
if grep -q "died" /tmp/philo_out; then pass "1 philo meurt"; cat /tmp/philo_out
else fail "1 philo ne meurt pas"; fi

echo -e "\n${YELLOW}=== 3. Personne ne doit mourir (5 800 200 200, 10s) ===${RESET}"
run 10 5 800 200 200
if grep -q "died" /tmp/philo_out; then fail "mort inattendue"; grep died /tmp/philo_out
else pass "aucune mort"; fi

echo -e "\n${YELLOW}=== 4. Personne ne doit mourir (4 410 200 200, 10s) ===${RESET}"
run 10 4 410 200 200
if grep -q "died" /tmp/philo_out; then fail "mort inattendue"; grep died /tmp/philo_out
else pass "aucune mort"; fi

echo -e "\n${YELLOW}=== 5. Un philo doit mourir (4 310 200 100) ===${RESET}"
run 10 4 310 200 100
if grep -q "died" /tmp/philo_out; then pass "mort détectée"; grep died /tmp/philo_out
else fail "personne n'est mort"; fi

echo -e "\n${YELLOW}=== 6. Arrêt après N repas (5 800 200 200 7) ===${RESET}"
run 20 5 800 200 200 7
code=$?
if [ $code -eq 124 ]; then fail "ne s'arrête pas (timeout)"
elif grep -q "died" /tmp/philo_out; then fail "mort pendant le test"
else
	for i in 1 2 3 4 5; do
		n=$(grep -c "^[0-9]* $i is eating" /tmp/philo_out)
		[ "$n" -ge 7 ] && pass "philo $i a mangé $n fois" || fail "philo $i a mangé $n fois (<7)"
	done
fi

echo -e "\n${YELLOW}=== 7. Gros nombre (200 800 200 200, 10s) ===${RESET}"
run 10 200 800 200 200
if grep -q "died" /tmp/philo_out; then fail "mort à 200 philos"; grep died /tmp/philo_out | head -1
else pass "aucune mort à 200 philos"; fi

echo -e "\n${YELLOW}=== 8. Pas de message après la mort ===${RESET}"
run 10 4 310 200 100
last=$(tail -1 /tmp/philo_out)
if echo "$last" | grep -q "died"; then pass "died est le dernier message"
else fail "message après la mort: $last"; fi

echo -e "\n${YELLOW}=== 9. Valgrind (fuites) ===${RESET}"
if command -v valgrind >/dev/null; then
	valgrind --leak-check=full --error-exitcode=42 -q $PHILO 4 800 200 200 3 > /dev/null 2>/tmp/vg
	[ $? -eq 42 ] && { fail "fuites/erreurs"; head /tmp/vg; } || pass "valgrind propre"
else echo "valgrind absent"; fi

echo -e "\n${YELLOW}=== 10. Helgrind (data races) ===${RESET}"
if command -v valgrind >/dev/null; then
	valgrind --tool=helgrind --error-exitcode=42 -q $PHILO 4 800 200 200 3 > /dev/null 2>/tmp/hg
	[ $? -eq 42 ] && { fail "data races"; head -20 /tmp/hg; } || pass "helgrind propre"
fi