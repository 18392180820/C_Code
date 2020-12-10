#include "log.h"

void _error_check_failed(int rc, const char *file, int line, const char *function, const char *expression)
{
	printf("ERROR_CHECK failed: err_t 0x%x at %p\n", rc, __builtin_return_address(0));
	printf("file: \"%s\" line %d\nfunc: %s\nexpression: %s\n", file, line, function, expression);
}

