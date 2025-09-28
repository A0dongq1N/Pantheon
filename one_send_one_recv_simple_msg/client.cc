#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }
    printf("client socket fd: %d\n", sockfd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connect_res = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (connect_res < 0) {
        perror("connect");
        return 1;
    }
    printf("connect success\n");

    while (true) {
        char send_buf[1024] = {0};
        scanf("%s", send_buf);

        int send_res = send(sockfd, send_buf, sizeof(send_buf), 0);
        if (send_res < 0) {
            perror("send");
            return 1;
        }
        printf("send [%s]success\n", send_buf);

        char recv_buf[1024] = {0};
        int recv_res = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
        if (recv_res < 0) {
            perror("recv");
            return 1;
            }
            printf("recv [%s]success\n", recv_buf);
    }

    close(sockfd);
    
    return 0;
}