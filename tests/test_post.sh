#!/bin/bash

# Variables
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
run_post_test() {
	local description="$1"
	local file="$2"
	local url="$3"
	local expected_code="$4"
	local file_option
	if [[ -n "$file" ]]; then
		file_option="-F file=@$file"
	else
		file_option=""
	fi

	echo -e "\n$description"
	echo "curl -s -X POST $file_option "$url" --output -"
	response=$(curl -s -X POST $file_option -o /dev/null -w "%{http_code}" "$url" --output -)
	if [ "$response" == "$expected_code" ]; then
		echo -e $GREEN "Test passed ! $RESET"
		((PASSED++))
	else
		echo -e $RED "Test failed... (Expected: $expected_code, Got: $response)" $RESET
		((FAILED++))
	fi
	sleep 0.1
}



#TESTS UPLOADS
# ROUTE ENABLES POST
# 	ROUTE ENABLES UPLOADS
run_post_test "Test 1: upload valid text file" 				"/tmp/www_archive/test_files/test.txt"		"http://localhost:9999/uploads/test.txt"	"201"
run_post_test "Test 2: upload valid image" 					"/tmp/www_archive/test_files/picture.jpeg"	"http://localhost:9999/uploads/image.jpg"	"201"
run_post_test "Test 3: upload a directory (405)"			"/tmp/www_archive/test_files/test.txt"		"http://localhost:9999/uploads/"			"405"
# run_post_test "Test 3.2: upload a new directory/ (405)"		"/tmp/www_archive/test_files/test.txt"		"http://localhost:9999/uploads/test/"		"405"
run_post_test "Test 4: upload an existing file (409)" 		"/tmp/www_archive/test_files/test.txt"		"http://localhost:9999/uploads/test.txt"	"409"
run_post_test "Test 5: upload bad media type (415)" 		"/tmp/www_archive/test_files/test.txxxxt"	"http://localhost:9999/uploads/test.txxxxt"	"415"
chmod 000 /tmp/www/images
run_post_test "Test 6: upload without  permission (403)"	"/tmp/www_archive/test_files/test.txt"		"http://localhost:9999/uploads/forbidden/test1.txt" "403"	
chmod 755 /tmp/www/images
# 	ROUTE DOES NOT ENABLE UPLOADS


# ROUTE DOES NOT ENABLE POST
# curl -v -X POST -F "file=@/home/lpaquatt/Downloads/ft_printf.c" localhost:9999/uploads 

# post in route

#TESTS CGI




# no content type 400
# wrong content type 415

# curl -X POST -d "name=John&mail=john@example.com&message=Hello" http://localhost:9999/kapouet/test-post.php
# curl -X POST -F "name=John" -F "mail=john@example.com" -F "message=Hello" -F "file=@/path/to/file" http://localhost:9999/kapouet/test-post-upload.php
# curl -X POST -H "Content-Type: application/json" -d '{"name": "John", "mail": "john@example.com", "message": "Hello"}' http://localhost:9999/kapouet/test-post-json.php
# tester dans dossier sans droit d'ecriture
# tester quand uploads pas enabled