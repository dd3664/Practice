#ifndef __LOGTOOL_H_
#define __LOGTOOL_H_

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define TPLOG_PATH                              "/tmp/TPLOG"
#define TAR_SUFFIX                              ".tar.gz"
#define MAX_SIXE                                1024 * 100 
#define MAX_ROTATE                              2
#define SUFFIX_LEN                              10
#define MAX_PATH_LEN                            64
#define MAX_CMD_LEN                             128
#define ROTATE_FILE(fp) do \
						{ \
							int n = 0; \
							char oldPath[MAX_PATH_LEN] = {0}, newPath[MAX_PATH_LEN] = {0}, tarCmd[MAX_CMD_LEN] = {0}; \
							size_t base = strlen(TPLOG_PATH); \
							fclose(fp); \
							memcpy(oldPath, TPLOG_PATH, base); \
							memcpy(newPath, TPLOG_PATH, base); \
							sprintf(tarCmd, "tar -czvPf %s%s %s", TPLOG_PATH, TAR_SUFFIX, TPLOG_PATH); \
							printf("TPT tarCmd:%s\n", tarCmd); \
							system(tarCmd); \
							for (n = MAX_ROTATE - 1; n >= 0; n--) \
							{ \
								snprintf(oldPath + base, SUFFIX_LEN, n ? "%s.%d" : "%s",TAR_SUFFIX,  n - 1); \
								snprintf(newPath + base, SUFFIX_LEN, "%s.%d",TAR_SUFFIX, n); \
								printf("TPT oldPath:%s, newPath:%s\n", oldPath, newPath); \
								if (access(newPath, F_OK) == 0) \
									remove(newPath); \
								if (access(oldPath, F_OK) == 0) \
									rename(oldPath, newPath); \
							} \
							remove(TPLOG_PATH); \
							fp = fopen(TPLOG_PATH, "a+"); \
						} while (0);

#define TPLOG(fmt, ...) do \
						{ \
							FILE *fp = fopen(TPLOG_PATH, "a+"); \
							size_t file_size = 0; \
							if (fp == NULL) \
								break; \
							fseek(fp, 0L, SEEK_END); \
							file_size = ftell(fp); \
							if (file_size >= MAX_SIXE) \
							{ \
								ROTATE_FILE(fp) \
							} \
							fprintf(fp, "[%s:%d] "fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							fclose(fp); \
						} while(0);
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/

#endif /* __LOGTOOL_H_ */
