#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage : ./client <serveur> <port> <fichier>\n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
        perror("Erreur lors de la résolution de l'adresse");
        exit(EXIT_FAILURE);
    }
/////////////////////////////////////////////////////////////////////////////////////
    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // création d'un socket
    if (sd < 0) {
        perror("Erreur lors de la création du socket");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    printf("Socket créé avec succès\n");
    close(sd);
    freeaddrinfo(result);
    return 0;
}


//Crée un socket basé sur les paramètres obtenus via getaddrinfo.
//Le socket permet d'envoyer et recevoir des données via le protocole UDP.
