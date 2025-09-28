#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Msg {
    char name[32];
    int   age; 
};

struct Response {
    int result;
    char res_info[16];
};

// 随机名字数组
const char* names[] = {
    "张三", "李四", "王五", "赵六", "钱七", "孙八", "周九", "吴十",
    "郑十一", "王十二", "冯十三", "陈十四", "褚十五", "卫十六", "蒋十七", "沈十八"
};

// 生成随机名字和年龄的函数
void generate_random_person(Msg* person) {
    int name_count = sizeof(names) / sizeof(names[0]);
    int random_index = rand() % name_count;
    strcpy(person->name, names[random_index]);
    person->age = rand() % 80 + 18; // 18-97岁之间的随机年龄
}

int main() {
    // 初始化随机数种子
    srand(time(NULL));
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("create socket fail");
        return 1;
    }
    printf("client socket fd: %d\n", sockfd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connect_res = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (connect_res < 0) {
        perror("connect fail");
        return 1;
    }
    printf("connect success\n");

    while (true) {
        Msg person_info;
        generate_random_person(&person_info);

        int send_res = send(sockfd, &person_info, sizeof(person_info), 0);
        if (send_res < 0) {
            perror("send fail");
            return 1;
        }
        printf("send [person_info.name=%s, persion_info.age=%d] success\n", person_info.name, person_info.age);

        Response response;
        int recv_res = recv(sockfd, &response, sizeof(response), 0);
        if (recv_res < 0) {
            perror("recv fail");
            return 1;
            }
            printf("recv [result=%d, res_info=%s] success\n", response.result, response.res_info);
        sleep(1);    
    }

    close(sockfd);
    
    return 0;
}