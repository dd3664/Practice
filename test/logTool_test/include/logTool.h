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
#define LOCK_FILE                               "/tmp/log.lock"

#define SUFFIX_LEN                              10
#define MAX_PATH_LEN                            64
#define MAX_CMD_LEN                             128
#define MAX_TIME_LEN                            24

#define GET_TIME(timeStr) do \
						 { \
						 	time_t tt = time(NULL); \
							struct tm *ptm = localtime(&tt); \
							strftime(timeStr, MAX_TIME_LEN, "%Y-%m-%d %H:%M:%S", ptm); \
						 } while (0);
#define TAR_AND_ROTATE_FILE(fp) do \
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
							int lckfd; \
							struct flock fl; \
							lckfd = open(LOCK_FILE, O_WRONLY | O_CREAT); \
							fl.l_type = F_WRLCK; \
							fl.l_whence = SEEK_END; \
							fl.l_start = 0; \
							fl.l_len = 0; \
							printf("Try get lock.\n"); \
							fcntl(lckfd, F_SETLKW, &fl); \
							printf("Got lock.\n"); \
							FILE *fp = fopen(TPLOG_PATH, "a+"); \
							size_t file_size = 0; \
							char timeStr[MAX_TIME_LEN] = {0}; \
							if (fp == NULL) \
								break; \
							fseek(fp, 0L, SEEK_END); \
							file_size = ftell(fp); \
							if (file_size >= MAX_SIXE) \
							{ \
								TAR_AND_ROTATE_FILE(fp) \
							} \
							GET_TIME(timeStr) \
							fprintf(fp, "[%s] [%s:%s:%d] "fmt"\n",timeStr, __FILE__,  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							sleep(10); \
							fclose(fp); \
							fl.l_type = F_UNLCK; \
							printf("UNLOCK\n"); \
							fcntl(lckfd, F_SETLK, &fl); \
							close(lckfd); \
							sleep(1); \
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
