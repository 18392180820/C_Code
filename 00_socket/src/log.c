#include "log.h"

void printHex(unsigned char *buff, unsigned int len, char const *str)
{
    /*if(len > 0xFF)
    {
        return;
    }*/

    int i = 0;
    int templen = ((len > 0xFF) ? 0xFF : len);

    printf("%s = ", str);
    for(i = 0; i < templen; i++)
    {
        printf("%02x ", buff[i]);
    }

    printf("\n");
}


void _error_check_failed(int rc, const char *file, int line, const char *function, const char *expression)
{
	printf("ERROR_CHECK failed: err_t 0x%x at %p\n", rc, __builtin_return_address(0));
	printf("file: \"%s\" line %d\nfunc: %s\nexpression: %s\n", file, line, function, expression);
}

