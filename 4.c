#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 516

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

    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sd < 0) {
        perror("Erreur lors de la création du socket");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[3], O_CREAT | O_WRONLY, S_IRWXU);
    if (fd < 0) {
        perror("Erreur lors de l'ouverture du fichier");
        close(sd);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    char rrq[128] = {0};
    rrq[1] = 1; // Opcode pour RRQ
    sprintf(rrq + 2, "%s", argv[3]);
    sprintf(rrq + 3 + strlen(argv[3]), "octet");

    if (sendto(sd, rrq, strlen(argv[3]) + 9, 0, result->ai_addr, result->ai_addrlen) < 0) {
        perror("Erreur lors de l'envoi de la requête RRQ");
        close(fd);
        close(sd);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr data_connexion;
    socklen_t data_connexion_length = sizeof(data_connexion);
    int bytes;

    do {
        bytes = recvfrom(sd, buffer, BUFFER_SIZE, 0, &data_connexion, &data_connexion_length);
        if (bytes < 0) {
            perror("Erreur lors de la réception des données");
            break;
        }

        if (buffer[1] == 5) {
            printf("Erreur TFTP : %s\n", buffer + 4);
            break;
        }

        write(fd, buffer + 4, bytes - 4);

        char ack[4] = {0};
        ack[1] = 4; // Opcode pour ACK
        ack[2] = buffer[2];
        ack[3] = buffer[3];

        if (sendto(sd, ack, sizeof(ack), 0, &data_connexion, data_connexion_length) < 0) {
            perror("Erreur lors de l'envoi de l'ACK");
            break;
        }

        printf("Reçu %d octets\n", bytes);
    } while (bytes == BUFFER_SIZE);

    close(fd);
    close(sd);
    freeaddrinfo(result);
    return 0;
}
