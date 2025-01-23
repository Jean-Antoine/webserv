#!/bin/bash

# Variables
TEST_DIR="/tmp/"
CONFIG_FILE="tests/config.json"
LOG_FILE="tests/logs"
LOG_ERR_FILE="tests/error_logs"
SERVER_BINARY="./webserv"
> "$LOG_FILE"
> "$LOG_ERR_FILE"

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
make cp_env_test


# Compilation et lancement du serveur avec Valgrind
echo -e $BLUE "Compiling and starting the web server..." $RESET
make
valgrind --leak-check=full --show-leak-kinds=all "$SERVER_BINARY" "$CONFIG_FILE" > "$LOG_FILE" &
SERVER_PID=$!
sleep 3

echo -e $GREEN "\nLAUNCHING GET TEST" $RESET

# Resultat des tests
PASSED=0
FAILED=0

# Fonction pour effectuer un test de GET request
run_get_test() {
	local description="$1"
	local url="$2"
	local expected_code="$3"

	echo -e "\n$description"
	response=$(curl -s -o /dev/null -w "%{http_code}" "$url" --output -)
	if [ "$response" == "$expected_code" ]; then
		echo -e $GREEN "Test passed ! $RESET"
		((PASSED++))
	else
		echo -e $RED "Test failed... (Expected: $expected_code, Got: $response)" $RESET
		((FAILED++))
	fi
	sleep 0.1
}

# Tests des codes de reponse
# GET REQUESTS ON FILES
run_get_test "Test 1: GET request: valid html" "http://localhost:9999/webserv_test/simple-get.html" "200"
run_get_test "Test 2: GET request: valid image" "http://localhost:9999/images/jww-wallpaper.jpg" "200"
run_get_test "Test 3: GET request: inexistant file (404)" "http://localhost:9999/webserv_test/nonexistent.html" "404"
run_get_test "Test 4: GET request: empty file (200)" "http://localhost:9999/webserv_test/edge_cases/empty.html" "200"
run_get_test "Test 5: GET request: forbidden (403)" "http://localhost:9999/webserv_test/edge_cases/nopermission.html" "403"

#GET REQUESTS ON DIR
run_get_test "Test 6: GET request: directory forbidden (403)" "http://localhost:9999/images/forbidden" "403"
run_get_test "Test 7: GET request: directory without index file and without listing (403)" "http://localhost:9999/styles/" "403"
run_get_test "Test 8: GET request: directory without index file and with listing (200)" "http://localhost:9999/images/" "200"
run_get_test "Test 9: GET request: directory with index file (200)" "http://localhost:9999/webserv_test/" "200"


# Nettoyage
cleanup

# Afficher les logs
echo -e $BLUE "\nLog errors:" $RESET
cat "$LOG_ERR_FILE"

# Résumé des résultats
echo -e $BLUE "\nTests Summary:" $RESET
echo -e $GREEN "$PASSED tests passed" $RESET
echo -e $RED "$FAILED tests failed" $RESET

