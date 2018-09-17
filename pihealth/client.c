/*************************************************************************
	> File Name: client2.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月11日 星期日 11时59分18秒
 ************************************************************************/

#include "client.h"

int main() {
    int sockfd, listen_sockfd;
    int port = 8873, new_port = 8875;
    int a;
    struct sockaddr_in client_addr;
    socklen_t len1 = sizeof(client_addr);
    char *buffer = (char *)calloc(50, sizeof(char));
    while (1) {
        if ((listen_sockfd = socket_creat(port)) < 0) {
            DBG("listen_sockfd creat error\n")
            continue;
        }
        while(1) {    
            if ((sockfd = accept(listen_sockfd, (struct sockaddr *) &client_addr, &len1)) < 0) {
                DBG("accept error\n")
                break;
            }
            DBG("sockfd = %d\n", sockfd)
            char path[3][20] = {"CPU.log", "MEM.log", "DISK.log"};
            for (int i = 0; i < 3; ++i) {
                if ((a = recv(sockfd, buffer, 3, 0)) <= 0) {
                    break;
                }
                DBG("buffer = %s\n", buffer)
                sleep(5);
                if (strncmp(buffer, "101", 3) == 0) {
                    printf("%s\n", path[0]);
                    client_send(sockfd, path[0], new_port);
                    memset(buffer, 0, 50);
                    DBG("close temp_sockfd\n")
                    continue;
                }
                if (strncmp(buffer, "102", 3) == 0) {
                    printf("%s", path[1]);
                    client_send(sockfd, path[1], new_port);
                    memset(buffer, 0, 50);
                    DBG("close temp_sockfd\n")
                    continue;
                }
                if (strncmp(buffer, "103", 3) == 0) {
                    printf("%s", path[2]);
                    client_send(sockfd, path[2], new_port);
                    memset(buffer, 0, 50);
                    DBG("close temp_sockfd\n")
                    continue;
                }
                break;
            }
            break;
        }
    }
    return 0;
}
