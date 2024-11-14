#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Créer le socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Erreur : échec de la création du socket" << std::endl;
        return -1;
    }
    
    // Attacher le socket au port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Erreur : échec de l'attachement du socket" << std::endl;
        return -1;
    }
    
    // Mettre le serveur en mode écoute
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Erreur : écoute échouée" << std::endl;
        return -1;
    }
    
    std::cout << "Serveur HTTP en écoute sur le port 8080" << std::endl;
    
    while (true) {
        // Accepter une nouvelle connexion
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            std::cerr << "Erreur : échec d'acceptation de la connexion" << std::endl;
            continue;
        }
        
        // Lire la requête HTTP du client
        char buffer[30000] = {0};
        read(client_socket, buffer, 30000);
        std::cout << "Requête reçue :\n" << buffer << std::endl;
        
        // Construire une réponse HTTP
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                               "<html><body><h1>Hello, world!</h1></body></html>";
        
        // Envoyer la réponse HTTP au client
        send(client_socket, response.c_str(), response.size(), 0);
        
        // Fermer la connexion avec le client
        close(client_socket);
    }
    
    return 0;
}