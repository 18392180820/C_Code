#include "log.h"

static const char* TAG = "main";

void main(void)
{	
	LOG_I(TAG, "info");
	LOG_W(TAG, "warn");
	LOG_E(TAG, "error");
	//LOG_N(TAG, "NNN");
}



