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
#define MAX_SIXE                                1024 * 5 
#define MAX_ROTATE                              1
#define SUFFIX_LEN                              10
#define MAX_PATH_LEN                            64

#define ROTATE_FILE(fp) do \
						{ \
							int n = 0; \
							char oldPath[MAX_PATH_LEN] = {0}, newPath[MAX_PATH_LEN] = {0}; \
							size_t base = strlen(TPLOG_PATH); \
							memcpy(oldPath, TPLOG_PATH, base); \
							memcpy(newPath, TPLOG_PATH, base); \
							fclose(fp); \
							for (n = MAX_ROTATE - 1; n >= 0; n--) \
							{ \
								snprintf(oldPath + base, SUFFIX_LEN, n ? ".%d" : "", n - 1); \
								snprintf(newPath + base, SUFFIX_LEN, ".%d", n); \
								if (access(newPath, F_OK) == 0) \
									remove(newPath); \
								if (access(oldPath, F_OK) == 0) \
									rename(oldPath, newPath); \
							} \
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
