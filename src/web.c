#include "web.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int studio_connect(const char* ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[studio.web] socket failed");
        return -1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        perror("[studio.web] invalid address");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[studio.web] connect failed");
        close(sock);
        return -1;
    }

    return sock;
}

int studio_send_packet(int sock, uint8_t opcode, const void* payload, uint16_t payload_len) {
    uint16_t total_len = 1 + payload_len; // 1 byte opcode + payload
    uint16_t net_len = htons(total_len);

    if (send(sock, &net_len, 2, 0) != 2) return -1;
    if (send(sock, &opcode, 1, 0) != 1) return -1;
    if (payload_len > 0 && send(sock, payload, payload_len, 0) != payload_len) return -1;

    return 0;
}

void studio_close(int sock) {
    close(sock);
}
