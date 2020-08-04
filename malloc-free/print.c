#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdarg.h>
#include "print.h"

typedef enum _info_level{
	IPRINT_DEBUG = 0,
	IPRINT_TRACE = 1,
	IPRINT_INFO = 2,
	IPRINT_WARN = 3,
	IPRINT_ERROR = 4,
	IPRINT_FATAL = 5
}info_level;

static int print_level = IPRINT_DEBUG;

#define MAX_PRINT_SIZE 4096
	
#define PrintInfo(color, type, format)  \
	do \
	{ \
		char __printBuf[MAX_PRINT_SIZE] = {0}; \
		if (strlen(format) < MAX_PRINT_SIZE) \
		{ \
			struct tm *ptm;\
			struct timeb stTimeb;\
			time_t ts = time(NULL);\
			ftime(&stTimeb);\
			ptm = localtime(&ts);\
			snprintf(__printBuf, MAX_PRINT_SIZE, "%s%04d-%02d-%02d %02d:%02d:%02d.%03d | %s ", \
				color, ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday, \
				ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm, type); \
			size_t __offset = strlen(__printBuf); \
			va_list  __va; \
			va_start(__va, format); \
			vsnprintf(__printBuf+__offset, MAX_PRINT_SIZE-__offset, format, __va);\
			va_end(__va); \
			__offset = strlen(__printBuf); \
			const char* pColor = strlen(color) > 0 ? "\033[0m" : ""; \
			snprintf(__printBuf+__offset, MAX_PRINT_SIZE-__offset, "%s", pColor); \
			printf("%s", __printBuf); \
			printf("\n"); \
		}\
	} while(0)

void set_verbose(int iVerbose)
{
	print_level = iVerbose;
}

int get_verbose(int iVerbose)
{
	return print_level <= iVerbose;
}
	
// ��ӡ������־
void LogDebug(const char* file, const char* function, int line, const char* fmt, ...)
{ 
	if(get_verbose(IPRINT_DEBUG))
	{
		char szTypeInfo[256] = { 0 };
		snprintf(szTypeInfo, sizeof(szTypeInfo), "debug %s::%s<%d>", file, function, line);
		#ifdef WIN32
			PrintInfo("", szTypeInfo, fmt); 
		#else
			PrintInfo("\033[0;36m", szTypeInfo, fmt); 	//����
		#endif // WIN32
	}
}
	
// ��ӡ������־
void LogTrace(const char* file, const char* function, int line, const char* fmt, ...)
{ 
	if(get_verbose(IPRINT_DEBUG))
	{
		char szTypeInfo[256] = { 0 };
		snprintf(szTypeInfo, sizeof(szTypeInfo), "trace %s::%s<%d>", file, function, line);
		#ifdef WIN32
			PrintInfo("", szTypeInfo, fmt); 
		#else
			PrintInfo("\033[0;37m", szTypeInfo, fmt); 	//��ɫ
		#endif // WIN32
	}
}
	
// ��ӡ��Ҫ��Ϣ    
void LogInfo(const char* file, const char* function, int line, const char* fmt, ...)
{
	if(get_verbose(IPRINT_DEBUG))
	{
		char szTypeInfo[256] = { 0 };
		snprintf(szTypeInfo, sizeof(szTypeInfo), "info %s::%s<%d>", file, function, line);
		#ifdef WIN32
			PrintInfo("", szTypeInfo, fmt);
		#else
			PrintInfo("\033[0;32m", szTypeInfo, fmt);	//��ɫ
		#endif // WIN32
	}
}
	
// ��ӡ�澯��Ϣ
void LogWarn(const char* file, const char* function, int line, const char* fmt, ...)
{
	if(get_verbose(IPRINT_DEBUG))
	{
		char szTypeInfo[256] = { 0 };
		snprintf(szTypeInfo, sizeof(szTypeInfo), "warn %s::%s<%d>", file, function, line);
		#ifdef WIN32
			PrintInfo("", szTypeInfo, fmt);
		#else
			PrintInfo("\033[0;33m", szTypeInfo, fmt);	//��ɫ
		#endif // WIN32
	}
}
	
// ��ӡ������Ϣ
void LogError(const char* file, const char* function, int line, const char* fmt, ...)
{
	if(get_verbose(IPRINT_DEBUG))
	{
		char szTypeInfo[256] = { 0 };
		snprintf(szTypeInfo, sizeof(szTypeInfo), "error %s::%s<%d>", file, function, line);
		#ifdef WIN32
			PrintInfo("", szTypeInfo, fmt);
		#else
			PrintInfo("\033[0;31m", szTypeInfo, fmt);	//��ɫ
		#endif // WIN32
	}
}
	
// ��ӡ������Ϣ
void LogFatal(const char* file, const char* function, int line, const char* fmt, ...)
{
	if(get_verbose(IPRINT_DEBUG))
	{
		char szTypeInfo[256] = { 0 };
		snprintf(szTypeInfo, sizeof(szTypeInfo), "fatal %s::%s<%d>", file, function, line);
		#ifdef WIN32
			PrintInfo("", szTypeInfo, fmt); 
		#else
			PrintInfo("\033[0;35m", szTypeInfo, fmt);	//��ɫ
		#endif // WIN32
	}
}
	
void print_hexbuf(const char* file, const char* func, const int line, const unsigned char * data, int len)
{		
	int i = 0;
	int j = 0;
	if(get_verbose(IPRINT_DEBUG))
	{
		struct tm *ptm;
		struct timeb stTimeb;
		time_t ts = time(NULL);
		ftime(&stTimeb);
		ptm = localtime(&ts);
		printf("\n%04d-%02d-%02d %02d:%02d:%02d.%03d | HexBuf %s::%s<%d> len[%d] :\n", \
			ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm, \
			file, func, line, len);

		int lineNum = 16; /// ÿ����ʾ���ַ�����Ĭ��16��
		int rows = len/lineNum; /// ������

		for( i = 0; i < rows; i++)
		{
			printf("%04x    ", i * lineNum); /// ��ӡ�к�
			int num = i * lineNum;

			for( j = 0; j < lineNum; j++) /// ��ӡ���е�16��������
			{
				printf("%02x ",data[num + j]);
			}

			printf("    ");
			for( j = 0; j < lineNum; j++) /// ��ӡ���е��ַ�
			{
				if(data[num + j] <= 32 || data[num + j] >= 127) /// �����ɼ��ַ���Ϊ�ո����
				{
					printf(" ");
				}
				else
				{
					printf("%c",data[num + j]);
				}
			}

			printf("\n");
		}

		int leftlen = len % lineNum;
		if(leftlen != 0)
		{
			printf("%04x    ",len - leftlen);
			for( j = 0; j < leftlen; j++)
			{
				printf("%02x ",data[len - leftlen + j]);
			}

			for( j = leftlen; j < lineNum; j++)
			{
				printf("   ");
			}

			printf("    ");
			for( j = 0; j < leftlen; j++)
			{
				if(data[len - leftlen + j] <= 32 || data[len - leftlen + j] >= 127)
				{
					printf(" ");
				}
				else
				{
					printf("%c",data[len-leftlen + j]);
				}
			}

			printf("\n");
		}

		printf("End of PrintHexBuf  --------------------------------------------------------\n\n");
	}
}

