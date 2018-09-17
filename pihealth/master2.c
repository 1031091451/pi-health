/*************************************************************************
	> File Name: master2.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月14日 星期三 15时39分45秒
 ************************************************************************/

#include "master.h"

int main() {
    int pids;
    if ((pids = fork()) < 0) {
        DBG("fork() error\n")
    }
    if (pids == 0) {
        int host_nums, file_nums, a, port, new_port, start, end, warn_port;
        int sockfd, short_sockfd, warn_sockfd, warn_listen;
        char *value = (char *)calloc(100, sizeof(char));
        char *pathname = (char *)calloc(50, sizeof(char));
        strcpy(pathname, "./master.conf");

        search(pathname, "host_nums", value);
        host_nums = atoi(value);
        DBG("host_nums = %d\n", host_nums)
        memset(value, 0, sizeof(value)); 

        search(pathname, "file_nums", value);
        file_nums = atoi(value);
        DBG("file_nums = %d\n", file_nums)
        memset(value, 0, sizeof(value));

        search(pathname, "port", value);
        port = atoi(value);
        DBG("port = %d\n", port)
        memset(value, 0, sizeof(value));

        search(pathname, "new_port", value);
        new_port = atoi(value);
        DBG("new_port = %d\n", new_port)
        memset(value, 0, sizeof(value));

        search(pathname, "warn_port", value);
        port = atoi(value);
        DBG("warn_port = %d\n", warn_port)
        memset(value, 0, sizeof(value));

        search(pathname, "start", value);
        start = atoi(value);
        DBG("start = %d\n", start)
        memset(value, 0, sizeof(value));

        search(pathname, "end", value);
        end = atoi(value);
        DBG("end = %d\n", end);
        memset(value, 0, sizeof(value));

        int *pid = (int *)calloc(12, sizeof(int));
        int i;
        for (i = 0; i < 12; ++i) {
            if ((pid[i] = fork()) < 0) {
                DBG("fork error\n")
            }
            if (pid[i] == 0) {
                break;
            }
        }
        /*if (i == 11) {
            for (int j = 0; j < 3; ++j) {
                if ((warn_listen = socket_creat(warn_port)) > 0) {
                    DBG("warn_listen creat success\n")
                    break;
                } else {
                    DBG("warn_listen creat error\n")
                    if (j == 2) {
                        DBG("warn listen creat failed\n")
                        break;
                    }
                }
            }
            DBG("warn listen is open\n")
            DBG("warn listen = %d\n", warn_listen)
            struct sockaddr_in warn_client;
            socklen_t warn_len = sizeof(warn_client);
            while (1) {
                if ((warn_sockfd = accept(warn_listen, (struct sockaddr *)&warn_client, &warn_len)) < 0) {
                    DBG("accept error\n")
                    break;
                }
                char *warning = (char *)calloc(50, sizeof(char));
                a = master_receve(warn_sockfd, warning, "./log/warning");
                DBG("a=%d\n", a)
                break;
            }
        }*/
        if (pid[i] == 0) { 
            char *buffer = (char *)calloc(5000, sizeof(char));
            char request[6][10] = {"101", "102", "103", "104", "105", "106"};
            char file_path[5][20] = {"cpu.log", "mem.log", "disk.log", "system.log", "user.log"};
            while (1) {
                char *name = (char *)calloc(30, sizeof(char)); 
                search(pathname, "pre_name", value);
                if (i >= start && i <= end) {
                    sprintf(value + strlen(value), "%d", i);
                    DBG("value = %s\n", value)
                    sprintf(name, "%s", value);
                    DBG("name = %s\n", name)
                } else {
                    exit(0);
                }
                search(pathname, name, value);
                memset(name, 0, sizeof(name));
                sprintf(name, "%s", value);
                DBG("new name = %s\n", name)
                memset(value, 0, sizeof(value));
                for (int j = 0; j < 3; ++j) {
                    if ((sockfd = socket_connect(port, name)) < 0) {
                        DBG("socket connect error\n")
                        perror("socket_connect");
                        if (j == 2) {
                            DBG("socket connect wrong\n")
                            return -1;
                        }
                    } else {
                        DBG("socket connect success\n")
                        break;
                    }
                }
                DBG("sockfd = %d\n", sockfd) 
                for (int k = 0; k < file_nums; ++k) {
                    master_send(sockfd, request[k]);
                    DBG("request = %s\n", request[k])
                    if ((a = recv(sockfd, buffer, 3, 0)) > 0) {
                        DBG("receve = %s\n", buffer)
                        if (strncmp(buffer, "200", 3) == 0) {
                            for (int j = 0; j < 3; ++j) {
                                if ((short_sockfd = socket_connect(new_port, name)) < 0) {
                                    DBG("short_sockfd connect error\n")
                                    if (j == 2) {
                                        DBG("short_sockfd connect failed\n")
                                        close(short_sockfd);
                                        break;
                                    }
                                } else {
                                    DBG("short_sockfd connect success\n")
                                    memset(buffer, 0, 5000);
                                    search(pathname, "pubpath", value);
                                    char *new_path = (char *)calloc(400, sizeof(char));
                                    sprintf(new_path, "%s", value);
                                    memset(value, 0, sizeof(value));
                                    sprintf(new_path + strlen(new_path), "%d", i);
                                    sprintf(new_path + strlen(new_path), "%s", "/");
                                    sprintf(new_path + strlen(new_path), "%s", file_path[k]);
                                    DBG("new_path = %s\n", new_path);
                                    master_receve(short_sockfd, buffer, new_path);
                                    free(new_path);
                                    break;
                                }
                            }
                        } else {
                            DBG("request error\n")
                        }
                    } else {
                        DBG("a = %d\n", a)
                    }
                    sleep(1);
                }
                close(sockfd);
                DBG("sockfd closed\n");
                sleep(10);
            }
        }
    }
    exit(0);
}
