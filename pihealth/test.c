/*************************************************************************
	> File Name: test.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月16日 星期五 18时15分41秒
 ************************************************************************/

#include "head.h"

/*char *search(char *path, char *request) {
    char *buffer = (char *)malloc(sizeof(char) * 100);
    FILE *input = fopen(path, "r");
    int flag;
    int length = strlen(request);
    while (fgets(buffer, 95, input) != NULL) {
        if ((flag = strncmp(buffer, request, length)) == 0) {
            int len = strlen(buffer);
            if (buffer[length] == '=') {
                memset(request, 0, sizeof(request));
                memcpy(request, buffer + length + 1, len - length - 1);
                printf("%s", request);
                return request;
            }
            continue;
        }
    }
    memset(request, 0, strlen(request));
    return NULL;
}*/

int main() {
    char *path = (char *)malloc(sizeof(char) * 10);
    char *value = (char *)malloc(sizeof(char) * 20);
    memset(path, 0, sizeof(path));
    memset(value, 0, sizeof(path));
    sprintf(path, "%s", "./master.conf");
    search(path, "pi10", value);

    return 0;
}
