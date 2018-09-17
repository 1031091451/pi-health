/*************************************************************************
	> File Name: DEBUG.h
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年03月11日 星期日 08时20分38秒
 ************************************************************************/

#define DEBUG
#ifdef DEBUG
#define DBG(s, va_arg...) {\
    printf("FILE:%s LINE:[%d] return = ", __FILE__, __LINE);\
    printf(s, ##va_arg);\
}
#else
#define DBG(s, va_arg)
#endif
