/*************************************************************************
	> File Name: client2.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月14日 星期三 16时35分39秒
 ************************************************************************/

#include "client2.h"

int main() {
    int pids;
    if((pids = fork()) < 0) {
        DBG("fork() error\n")
    }
    if (pids == 0) {
        while (1) {
            int *pid = (int *)calloc(6, sizeof(int));
            char bash[5][20] = {"./CPU.sh", "./MEM.sh", "./DISK.sh", "./system.sh", "./user.sh"};
            char path[5][20] = {"./log/cpu.log", "./log/mem.log", "./log/disk.log", "./log/system.log", "./log/user.log"};
            int i;
            for (i = 0; i < 6; ++i) {
                if ((pid[i] = fork()) < 0) {
                    DBG("fork() error\n")
                }
                if (pid[i] == 0) {
                    break;
                }
            }
            if (pid[i] == 0) {
                if (i == 5) {                
                    int sockfd, listen_sockfd, a;
                    int port = 8873, new_port = 8875, warn_port= 8879;
                    struct sockaddr_in client_addr;
                    socklen_t len1 = sizeof(client_addr);
                    char *buffer = (char *)malloc(sizeof(char) * 50);
                    while (1) {
                        for (int i = 0; i < 3; ++i) {
                            if ((listen_sockfd = socket_creat(port)) < 0) {
                                DBG("listen_sockfd error\n")
                                if (i == 2) {
                                    DBG("listen_sockfd creat failed")
                                    return -1;
                                }
                            } else {
                                DBG("listen_sockfd creat sucess\n")
                                break;
                            }
                        }
                        DBG("liten sockfd is open\n")
                        DBG("listen sockfd = %d\n", listen_sockfd)
                        memset(buffer, 0, 50);
                        while (1) {
                            if ((sockfd = accept(listen_sockfd, (struct sockaddr *)&client_addr, &len1)) < 0) {
                                DBG("accept error\n")
                                break;
                            }
                            DBG("accept ok\n")
                            for (int i = 0; i < 5; ++i) {
                                if ((a = recv(sockfd, buffer, 3, 0)) <= 0) {
                                    break;
                                }
                                DBG("buffer = %s\n", buffer)

                                if (strncmp(buffer, "101" ,3) == 0) {
                                    DBG("path = %s\n", path[0])
                                    client_send(sockfd, path[0], new_port);
                                    continue;
                                }
                                if (strncmp(buffer, "102", 3) == 0) {
                                    DBG("path = %s\n", path[1])
                                    client_send(sockfd, path[1], new_port);
                                    continue;
                                }
                                if (strncmp(buffer, "103", 3) == 0) {
                                    DBG("path = %s\n", path[2])
                                    client_send(sockfd, path[2], new_port);
                                    continue;
                                }
                                if (strncmp(buffer, "104", 3) == 0) {
                                    DBG("path = %s\n", path[3])
                                    client_send(sockfd, path[3], new_port);
                                    continue;
                                }
                                if (strncmp(buffer, "105", 3) == 0) {
                                    DBG("path = %s\n", path[4])
                                    client_send(sockfd, path[4], new_port);
                                    continue;
                                }
                            }
                        }
                        break;
                    }
                    close(sockfd);
                    close(listen_sockfd);
                } else {
                    char *str = (char *)malloc(100000);
                    while (1) {
                        for (int j = 0; j < 5; ++j) {
                            FILE *input = popen(bash[i], "r");
                            DBG("popen input");
                            char *pattern = (char *)malloc(sizeof(char)  * 300);
                            memset(pattern, 0, sizeof(pattern));
                            while (fgets(pattern, 300, input) != NULL) {
                                sprintf(str + strlen(str), "%s", pattern);
                                DBG("str = %s\n", str)
                                memset(pattern, 0, sizeof(pattern));
                            }
                            pclose(input);
                            DBG("pclose input\n")
                            sleep(3);
                        }
                        FILE *input = fopen(path[i], "a+");
                        fputs(str, input);
                        fclose(input);
                        memset(str, 0, 100000);
                        DBG("%s is write\n", bash[i]);
                    }
                    free(str);
                }
            }
        } 
    }
    exit(0);
}
