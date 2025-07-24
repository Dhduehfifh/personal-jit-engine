#ifndef STUDIO_WEB_H
#define STUDIO_WEB_H

#ifdef __cplusplus
extern "C" {
#endif

// 启动 TCP 接口（阻塞）
int jmj_web_server_start(int port);

// 终止 TCP 服务（可选扩展）
void jmj_web_server_stop(void);

#ifdef __cplusplus
}
#endif

#endif // STUDIO_WEB_H
