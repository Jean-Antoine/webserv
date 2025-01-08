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
    echo -e $BLUE "\nStopping web server..." $RESET
    kill "$SERVER_PID" 2>/dev/null
    wait "$SERVER_PID" 2>/dev/null
    echo -e $BLUE "Server stopped." $RESET
}

# Assure le nettoyage en cas d'interruption (Ctrl+C, erreur)
trap cleanup INT TERM ERR

# Copier les fichiers de test
echo -e $BLUE "Setting up test environment..." $RESET
cp -r ./tests/www "$TEST_DIR"
chmod -r "$TEST_DIR"/www/nopermission.html


# Compilation et lancement du serveur avec Valgrind
#  #dernier arg = timeout

echo -e $GREEN "\nLAUNCHING CHUNK TESTS" $RESET

# script python
python tests/test_chunk.py

# Nettoyage
cleanup

# Afficher les logs
echo -e $BLUE "\nLog errors:" $RESET
cat "$LOG_ERR_FILE"

