#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

struct Msg {
    char name[32];
    int   age; 
};

struct Response {
    int result;
    char res_info[16];
};

// 信号处理函数
void signal_handler(int sig) {
    printf("\n=== Received signal %d ===\n", sig);
    switch(sig) {
        case SIGPIPE:
            printf("SIGPIPE: Broken pipe (client disconnected)\n");
            break;
        case SIGINT:
            printf("SIGINT: Interrupt (Ctrl+C)\n");
            break;
        case SIGTERM:
            printf("SIGTERM: Termination request\n");
            break;
        default:
            printf("Unknown signal: %d\n", sig);
            break;
    }
    printf("Server terminating due to signal...\n");
    exit(128 + sig);  // 标准的信号退出码
}

int main() {
    // 注册信号处理器
    // signal(SIGPIPE, signal_handler);
    // signal(SIGINT, signal_handler);
    // signal(SIGTERM, signal_handler);
    
    printf("Server starting with signal handlers registered...\n");
    
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
        Msg person_info;
        printf("Waiting for client data...\n");
        int recv_res = recv(accept_res, &person_info, sizeof(person_info), 0);
        
        if (recv_res < 0) {
            printf("recv failed: %s (errno=%d)\n", strerror(errno), errno);
            return 1;
        }
        // if (recv_res == 0) {
        //     printf("Client disconnected (recv returned 0)\n");
        //     break;  // 客户端正常关闭连接
        // }
        
        printf("recv [persion_info.name=%s, persion_info.age=%d] success (received %d bytes)\n", 
               person_info.name, person_info.age, recv_res);
        
        Response response = Response{0, "ok"};
        printf("Sending response...\n");
        int send_res = send(accept_res, &response, sizeof(response), 0);
        if (send_res < 0) {
            printf("send failed: %s (errno=%d)\n", strerror(errno), errno);
            if (errno == EPIPE) {
                printf("Broken pipe - client disconnected during send\n");
            }
            return 1;
        }
        printf("send [response.result=%d, response.res_info=%s] success (sent %d bytes)\n", 
               response.result, response.res_info, send_res);
    }
    // 关闭客户端连接
    close(accept_res);
    close(sockfd);

    return 0;
}