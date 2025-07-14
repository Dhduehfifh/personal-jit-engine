#ifndef STUDIO_WEB_H
#define STUDIO_WEB_H

#include <stdint.h>

int studio_connect(const char* ip, int port);
int studio_send_packet(int sock, uint8_t opcode, const void* payload, uint16_t payload_len);
void studio_close(int sock);

#endif
