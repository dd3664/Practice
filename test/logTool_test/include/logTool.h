#ifndef __LOGTOOL_H_
#define __LOGTOOL_H_

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define LOG_DIR                                 "/tmp"
#define LOG_NAME                                "TPLOG"
#define TPLOG_PATH                              "/tmp/TPLOG"
#define TAR_SUFFIX                              ".tar.gz"
#define MAX_SIXE                                1024 * 500 
#define MAX_ROTATE                              3
#define LOCK_FILE                               "/tmp/log.lock"

#define SUFFIX_LEN                              5
#define MAX_PATH_LEN                            64
#define MAX_CMD_LEN                             128
#define MAX_TIME_LEN                            24
#define MAX_TRY_GET_LCK                         10
#define TRY_GET_LCK_INTERVAL                    1000 * 10

#define SYSTEMCMD(cmd) do \
					   { \
					  		int pid = 0; \
							int status = 0; \
							char *argv[4]; \
							pid = fork(); \
							if (pid == 0) \
							{ \
								argv[0] = "sh"; \
								argv[1] = "-c"; \
								argv[2] = cmd; \
								argv[3] = NULL; \
								execve("/bin/sh", argv, NULL); \
								exit(127); \
							} \
							waitpid(pid, &status, 0); \
					   } while(0);

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
									size_t base = strlen(TPLOG_PATH) + strlen(TAR_SUFFIX); \
									fclose(fp); \
									snprintf(tarCmd, MAX_CMD_LEN, "cd %s && tar -czvf %s%s %s > /dev/null", LOG_DIR, LOG_NAME, TAR_SUFFIX, LOG_NAME); \
									SYSTEMCMD(tarCmd) \
									snprintf(oldPath, base + 1, "%s%s", TPLOG_PATH, TAR_SUFFIX); \
									snprintf(newPath, base + 1, "%s%s", TPLOG_PATH, TAR_SUFFIX); \
									for (n = MAX_ROTATE - 1; n >= 0; n--) \
									{ \
										snprintf(oldPath + base, SUFFIX_LEN, n ? ".%d" : "", n - 1); \
										snprintf(newPath + base, SUFFIX_LEN, ".%d", n); \
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
							int count = 0; \
							struct flock fl; \
							lckfd = open(LOCK_FILE, O_WRONLY | O_CREAT); \
							fl.l_type = F_WRLCK; \
							fl.l_whence = SEEK_SET; \
							fl.l_start = 0; \
							fl.l_len = 0; \
							do \
							{ \
								if (fcntl(lckfd, F_SETLK, &fl) == -1) \
								{ \
									count++; \
									usleep(TRY_GET_LCK_INTERVAL); \
								} \
								else \
									break; \
							} while(count < MAX_TRY_GET_LCK); \
							if (count == MAX_TRY_GET_LCK) \
							{ \
								close(lckfd); \
								break; \
							} \
							fcntl(lckfd, F_SETLKW, &fl); \
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
							fprintf(fp, "[%s] [%s:%s:%d] "fmt"",timeStr, __FILE__,  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							fclose(fp); \
							fl.l_type = F_UNLCK; \
							fcntl(lckfd, F_SETLK, &fl); \
							close(lckfd); \
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
