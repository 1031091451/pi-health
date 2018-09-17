/*************************************************************************
	> File Name: master2.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月11日 星期日 10时58分51秒
 ************************************************************************/

#include "master.h"

int main() {
    int pids;
    if ((pids = fork()) < 0) {
        DBG("fork() error\n")
        return -1;
    }
    if (pids == 0) {
        int a, sockfd;
        int port = 8873, new_port = 8875;
        char *ip = (char *)malloc(sizeof(char) * 25);
        strcpy(ip, "192.168.1.181");
        printf("%s\n", ip);
        for (int i = 0; i < 3; ++i) {
            if ((a = socket_connect(port, ip)) < 0) {
                DBG("long connect error\n")
                if (i == 2) return -1;
            }
            DBG("long connect success\n")
            DBG("sockfd = %d\n", sockfd)
            break;
        }
        char *buffer = (char *)malloc(sizeof(char) * 5000);
        char request[3][5] = {"101", "102", "103"};
        char path[3][20] = {"CPU1.log", "MEM1.log", "DISK1.log"};
        while (1) {
            memset(buffer, 0, 5000);
            for (int i = 0; i < 3; ++i) {
                master_send(sockfd, request[i]);
                printf("request = %s\n", request[i]);
                if ((a = recv(sockfd, buffer, 3, 0)) > 0) {
                    DBG("receve = %s\n", buffer)
                    if (strncmp(buffer, "200", 3) == 0) {
                        int short_sockfd;
                        for (int j = 0; j < 3; ++j) {    
                            if ((short_sockfd = socket_connect(new_port, ip)) < 0) {
                                DBG("short_sockfd connect error\n")
                                if (j == 2) return -1;
                            } else {
                                DBG("short_sockfd = %d\n", short_sockfd)
                                DBG("short_sockfd connect success\n")
                                master_receve(short_sockfd, buffer, path[i]);
                                break;
                            }
                        }
                    } else { 
                        DBG("respect error\n")
                        continue;
                    }
                }
                sleep(1);
            }
            sleep(5);
        }
        free(buffer);
        exit(0);
    }
    return 0;
}
