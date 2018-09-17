/*************************************************************************
	> File Name: client.h
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月11日 星期日 11时52分14秒
 ************************************************************************/

#ifndef _SERVER_H
#define _SERVER_H
#include "head.h"
int socket_creat(int port);
int client_send(int sockfd, char *path, int port);

int socket_creat(int port) {
    int sockfd;
    int flag = 1;
    struct sockaddr_in sock_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        DBG("socket error\n")
        return -1;
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {
        close(sockfd);
        perror("setsockpot() error\n");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        close(sockfd);
        DBG("bind error\n")
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        close(sockfd);
        DBG("listen error\n")
        return -1;
    }
    DBG("sockfd creat success\n")
    return sockfd;
}

int client_send(int sockfd, char *path, int port) {
    int temp_sockfd, temp_listen_sockfd;
    struct sockaddr_in temp_addr;
    socklen_t len = sizeof(temp_addr);
    send(sockfd, "200", 3, 0);
    for (int i = 0; i < 3; ++i) {
        if ((temp_listen_sockfd = socket_creat(port)) < 0) {
            DBG("temp creat error\n")
            if (i == 2) {
                DBG("temp creat failed\n")
                close(temp_listen_sockfd);
                break;
            }
        } else {
            DBG("temp creat success\n")
            break;
        }
    }
    fflush(stdout);
    while (1) {
        if ((temp_sockfd = accept(temp_listen_sockfd, (struct sockaddr *) &temp_addr, &len)) < 0) {
            break;
        }
        DBG("accept ok\n")
        FILE *input = popen(path, "r");
        DBG("input is open\n")
        DBG("%p\n", input)
            //fflush(stdout);
        char *buf = (char *)malloc(sizeof(char) * 5000);
        memset(buf, 0, 5000);
        while (fgets(buf, 4500, input) != NULL) {
                //printf("enter while\n");
                //fflush(stdout);
            int size = strlen(buf);
            DBG("size = %d\n", size)
            fflush(stdout);
            send(temp_sockfd, buf, size, 0);
            //printf("enter send\n");
            //fflush(stdout);
            memset(buf, 0, 5000);
        }
        pclose(input);
        DBG("input is closed\n")
        free(buf);
        close(temp_sockfd);
        DBG("temp_sockfd closed\n")
        break;
    }
    close(temp_listen_sockfd);
    DBG("temp_listen_sockfd closed")
    return 1;
}
#endif
