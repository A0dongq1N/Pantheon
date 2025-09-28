#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// 生成32字节的时间+随机字符串
void generate_message(char* buffer) {
    // 获取当前时间
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    
    // 格式化时间字符串 (YYYY-MM-DD HH:MM:SS = 19字符)
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // 初始化随机数种子
    srand(time(NULL));
    
    // 清空缓冲区
    memset(buffer, 0, 32);
    
    // 复制时间字符串到缓冲区
    strncpy(buffer, time_str, 19);
    
    // 生成13字节的随机字符串 (19 + 13 = 32)
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 19; i < 32; i++) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("socket creation failed\n");
        return 1;
    }
    printf("server socket fd: %d\n", sockfd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int bind_res = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (bind_res < 0) {
        perror("bind fail");
        return 1;
    }
    printf("bind success\n");

    int listen_res = listen(sockfd, 128);
    if (listen_res < 0) {
        perror("listen fail");
        return 1;
    }
    printf("listen success\n");

    int accept_res = accept(sockfd, NULL, NULL);
    if (accept_res < 0) {
        perror("accept fail");
        return 1;
    }
    printf("accept success\n");

    

    while (true) {
        char send_buf[32] = {0};  // 32字节的发送缓冲区
        char recv_buf[1024] = {0};
        int recv_res = recv(accept_res, recv_buf, sizeof(recv_buf), 0);
        if (recv_res < 0) {
            perror("recv fail");
            return 1;
        }
        printf("recv [%s]success\n", recv_buf);
        
        // 生成时间+随机字符串的消息
        generate_message(send_buf);
        printf("generated message: %.32s\n", send_buf);
        
        int send_res = send(accept_res, send_buf, 32, 0);
        if (send_res < 0) {
            perror("send");
            return 1;
        }
        printf("send [%s]success\n", send_buf);

    }
    // 关闭客户端连接
    close(accept_res);
    close(sockfd);

    return 0;
}