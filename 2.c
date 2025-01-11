#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage : ./client <serveur> <port> <fichier>\n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPV4
    hints.ai_protocol = IPPROTO_UDP; // UDP

    if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
        perror("Erreur lors de la résolution de l'adresse");
        exit(EXIT_FAILURE);
    }

    printf("Résolution de l'adresse réussie pour le serveur %s, port %s\n", argv[1], argv[2]);
    freeaddrinfo(result);
    return 0;
}


//Résout le nom d'hôte (adresse du serveur) et le port 
//getaddrinfo traduit ces informations en une structure utilisable par le programme.
