import time
import socket
import sys
import select

# Variables
GREEN="\033[1;32m"
RED="\033[1;31m"
BLUE="\033[1;34m"
RESET="\033[0m"

PASSED = 0
FAILED = 0


# Connexion à un serveur via Telnet
def send_chunked_request(host, port, chunks, delay_between_chunks=1):
    # Création de la connexion au serveur
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((host, port))

    # Envoi des chunks
    for chunk in chunks:
        client.sendall(chunk.encode('utf-8'))
        print(f"Sendind a chunk")
        # Donne un délai avant d'envoyer le prochain chunk
        time.sleep(1)

    # Attente de la réponse
    readable, _, _ = select.select([client], [], [], 3) #dernier arg = timeout
    if readable:
        response = client.recv(4096).decode('utf-8')  # Taille de buffer
        return response
    else:
        print(f"{RED}No response or timed out.\n{RESET}")
        return None

# Comparer le code de réponse HTTP et le contenu
def compare_response(response, expected_code):
    global PASSED, FAILED
    # Extraire le code de statut HTTP
    if not response:
        FAILED += 1
        return 
    status_code = response.split<t_strVec>(' ')[1]
    
    if status_code == expected_code:
        print(f"{GREEN}Test passed!\n{RESET}")
        PASSED += 1
    else:
        print(f"{RED}Test failed... (Expected: {expected_code}, Got: {status_code}){RESET}\n")
        FAILED += 1

def test1():
    print(f"{BLUE}Test 1: Valid chunk request{RESET}")
    expected_status_code = '200'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n",
        "4\r\nWiki\r\n",
        "5\r\npedia\r\n",
        "0\r\n\r\n"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

def test2():
    print(f"{BLUE}Test 2: Valid chunk request{RESET}")
    expected_status_code = '200'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nwesh\r\n0\r\n\r\n"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

# @leon: nginx accepts this case (to be more robust / resiliant to small mistakes..? but not the strict application of RFC?)
def test3():
    print(f"{BLUE}Test 3: Invalid chunk request (wrong chunk size){RESET}")
    expected_status_code = '400'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWik\r\n",
        "5\r\npedia\r\n",
        "0\r\n\r\n"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

def test4():
    print(f"{BLUE}Test 4: Invalid chunk request (wrong chunk size){RESET}")
    expected_status_code = '400'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWikipedia\r\n",
        "0\r\n\r\n"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

def test5():
    print(f"{BLUE}Test 5: Invalid chunk request (wrong delimiter){RESET}")
    expected_status_code = '400'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWiki\r5\r\npedia\r\n",
        "0\r\n\r\n"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

def test6():
    print(f"{BLUE}Test 6: Invalid chunk request (wrong delimiter){RESET}")
    expected_status_code = '400'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n9\r\n\r\nWikipedia\r\n",
        "0\r\n\r\n"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

def test7():
    print(f"{BLUE}Test 7: Invalid chunk request (data after endind){RESET}")
    expected_status_code = '400'
    request_chunks = [
        "GET /kapouet/test.html HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n9\r\n\r\nWikipedia\r\n",
        "0\r\n\r\nabc"
    ]
    response = send_chunked_request(host, port, request_chunks)
    compare_response(response, expected_status_code)

if __name__ == "__main__":
    host = 'localhost'
    port = 9999

    test1()
    test2()
    test3()
    test4()
    test5()
    test6()
    test7()

    # Afficher le nombre de tests passés et échoués
    print(f"{BLUE}Tests summary:{RESET}")
    print(f"{GREEN}- Tests passed: {PASSED}{RESET}")
    print(f"{RED}- Tests failed: {FAILED}{RESET}")