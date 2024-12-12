#!/bin/bash

# Variables
TEST_DIR="/tmp/www"
CONFIG_FILE="tests/config.json"
LOG_FILE="tests/logs"
LOG_ERR_FILE="tests/error_logs"
SERVER_BINARY="./webserv"

GREEN="\033[1;32m"
RED="\033[1;31m"
BLUE="\033[1;34m"
RESET="\033[0m"

# Fonction pour nettoyer le serveur après le script
cleanup() {
    echo -e $RED "\nStopping web server..." $RESET
    kill "$SERVER_PID" 2>/dev/null
    wait "$SERVER_PID" 2>/dev/null
    echo -e $RED "Server stopped." $RESET
}

# Assure le nettoyage en cas d'interruption (Ctrl+C, erreur)
trap cleanup INT TERM ERR

# Copier les fichiers de test
echo -e $BLUE "Setting up test environment..." $RESET
cp -r tests/www "$TEST_DIR"

# Compilation et lancement du serveur avec Valgrind
echo -e $BLUE "Compiling and starting the web server..." $RESET
make
valgrind --leak-check=full --show-leak-kinds=all "$SERVER_BINARY" "$CONFIG_FILE" > "$LOG_FILE" 2>"$LOG_ERR_FILE" &
SERVER_PID=$!
sleep 3

echo -e $GREEN "Launching tests..." $RESET

# Tests
PASSED=0
FAILED=0

# Fonction pour effectuer un test
run_test() {
	local description="$1"
	local url="$2"
	local expected_code="$3"

	echo -e "\n$description"
	response=$(curl -s -o /dev/null -w "%{http_code}" "$url")
	if [ "$response" == "$expected_code" ]; then
		echo -e $GREEN "Test Passed $RESET"
		((PASSED++))
	else
		echo -e $RED "Test Failed (Expected: $expected_code, Got: $response)" $RESET
		((FAILED++))
	fi
	sleep 1
}

# Exemple de tests
run_test "Test 1: Valid GET request" "http://localhost:9999/kapouet/test.html" "200"
run_test "Test 2: Invalid GET request (404)" "http://localhost:9999/kapouet/nonexistent.html" "404"

# Affichage des résultats
echo -e $BLUE "\nTests Summary:" $RESET
echo -e $GREEN "$PASSED tests passed." $RESET
echo -e $RED "$FAILED tests failed." $RESET


# Nettoyage
cleanup

# Afficher les logs
echo -e $BLUE "\nLog errors:" $RESET
cat "$LOG_ERR_FILE"