#include "web.h"
#include "jmj_engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAX_CLIENTS 32
#define READ_BUF_SIZE 4096

static int server_fd = -1;
static volatile int server_running = 1;

static void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0) {
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
}

static void* server_loop(void* arg) {
    (void)arg;

    struct sockaddr_in addr;
    int client_fds[MAX_CLIENTS] = {0};

    while (server_running) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        int max_fd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_fds[i] > 0) {
                FD_SET(client_fds[i], &read_fds);
                if (client_fds[i] > max_fd)
                    max_fd = client_fds[i];
            }
        }

        struct timeval timeout = {1, 0};  // 1 秒 timeout
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (activity < 0) continue;

        if (FD_ISSET(server_fd, &read_fds)) {
            int new_fd = accept(server_fd, NULL, NULL);
            if (new_fd >= 0) {
                set_nonblocking(new_fd);
                for (int i = 0; i < MAX_CLIENTS; ++i) {
                    if (client_fds[i] == 0) {
                        client_fds[i] = new_fd;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int fd = client_fds[i];
            if (fd <= 0) continue;

            if (FD_ISSET(fd, &read_fds)) {
                uint8_t buffer[READ_BUF_SIZE];
                ssize_t len = read(fd, buffer, sizeof(buffer));
                if (len <= 0) {
                    close(fd);
                    client_fds[i] = 0;
                } else {
                    // === 直接将接收到的数据作为字节码推送进引擎 ===
                    jmj_push_bytes(buffer, len);
                    jmj_execute();

                    jmj_result_t result = jmj_get_result();
                    write(fd, result.data, result.size);  // 返回执行结果
                }
            }
        }
    }

    // 关闭所有连接
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_fds[i] > 0)
            close(client_fds[i]);
    }
    close(server_fd);
    return NULL;
}

int jmj_web_server_start(int port) {
    jmj_init(4096);  // 固定页面大小初始化 VM

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("[jmj_web] socket failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[jmj_web] bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("[jmj_web] listen failed");
        close(server_fd);
        return -1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, server_loop, NULL);
    pthread_detach(tid);
    printf("[jmj_web] Listening on port %d...\n", port);

    return 0;
}

void jmj_web_server_stop(void) {
    server_running = 0;
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }
}
