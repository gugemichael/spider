/*
 * Library Framework For C
 *
 * Copyright 2012 , Michael LiuXin <guguemichael@gmail.com>
 *
 * Contributors can discuss or commit code at
 *
 *  http://blog.csdn.net/gugemichael (Blog)
 *  https://github.com/gugemichel/libnimo (Git)
 *
 * Version : 1.0
 * Created : 09/19/2012 09:25:52 PM
 * Filename : nimo_log.h
 * Description : log with split files
 * Compiler : g++ 4.6.1
 * Author : Michael LiuXin
 *
 */

#ifndef __NIMO_LOG_H__
#define __NIMO_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define __USE_GNU 1
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>

#include <sys/time.h>

#define FILE_NAME_MAX 1024

#define NORMAL_LOG (1)
#define ERROR_LOG (2)

#ifndef likely
#define likely(x) __builtin_expect(!!(x),1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x),0)
#endif

#define MAX_THREADS 65535

// syscall fsync
#ifdef __linux__
#define os_fdatasync fdatasync
#elif (defined(__APPLE__) && defined(__MACH__))
#define os_fdatasync fsync
#else
#define os_fdatasync __compile_error__
#endif

// syscall gettid
#ifdef __linux__
#define os_gettid() ((unsigned int)syscall(SYS_gettid))
#elif (defined(__APPLE__) && defined(__MACH__))
#define os_gettid() ((unsigned int)pthread_self())
#else
#define os_gettid __compile_error__
#endif

// O_DIRECT and O_SYNC
#ifndef __linux__
#ifndef O_DIRECT
#define O_DIRECT 0
#endif
#ifndef O_SYNC
#define O_SYNC 0
#endif
#endif

#define SPLIT_ONE_DAY (3600)

typedef enum __log_level {
    DEBUG = 0,
    INFO,
    WARNNING,
    ERROR,
    UNKOWN
} log_level;


typedef struct __buffer_t {
    size_t size;
    size_t cursor;
    unsigned char mem[];
} buffer_t;


typedef struct __log_t {
    log_level level;            /* Log level , lower level string will ignore */
    char file_name[FILE_NAME_MAX];
    char *last_error;
    unsigned int mask;
    int normal_file;                /* output log file */
    int error_file;                /* output error and warning file */
    int buffer_cache;
    size_t normal_file_size;        /* file size record */
    size_t error_file_size;        /* file size record */
    size_t split_size;                /* one piece(file) max size with split */
    size_t buffer_page_cap;        /* buffer capacity */
    time_t split_time;  /* one piece(file) time with split */
    time_t touch_time;    /* total time */
    struct tm *timep;
    buffer_t *dirty_page[MAX_THREADS];
    pthread_mutex_t split_lock;
} log_t;

/**
 * Initial log handler with path , indicate use direct io or not
 * 
 * @params filepath, which file to be written . if NULL "stdout" will be set
 *
 * @return : pointer to the log_t structure . if failed , it's NULL
 */
log_t *nimo_log_init(const char *filepath);

/**
 * Initial log handler with path , direct io , and split strategy 
 * 
 * @params filepath, which file to be written . if NULL "stdout" will be set
 * @params ts, span time of split file , 0 means no split 
 * @params ss, content size of split file , 0 means no split 
 *
 * @return : pointer to the log_t structure . if failed , it's NULL
 */
log_t *nimo_log_split_init(const char *filepath, unsigned long long ts, size_t ss);

/* Destroy log handle 
 * 
 * */
void nimo_log_destroy();


/* Do actual write disk (may be cached) . DON'T call it directly 
 * 
 * @params level, specified level to be write
 * @params file, file name from __FILE__
 * @params func_name, function name from __FUNCTION__
 * @params line, line number from __LINE__
 * @params varlist..., variable strings
 *
 * */
void log_write(log_level level, const char *file, const char *func_name, unsigned int line, const char *__format, ...);

/* Reduce write() call times , if possible save into buffer
 * first . When buffer is full , will cause write()
 * 
 * @params log, log handle
 * @params size, specified buffer size 
 *
 * return : 0 if buffer allocate successfully
 *
 * */
int nimo_log_buffer(log_t *log, int size);

/* log level macro, recorded log will be losed and ignored that
 * below than the log level setted
 *
 * @params log, log handle 
 * @params buf, buffer to be written 
 *
 * */
void nimo_log_level(log_t *log, log_level);


/* NON_USE_NIMO_LOG will remove all the log statement
 * 
 * */
#ifndef NON_USE_NIMO_LOG
#define LogDebug(buf, ...) do{\
log_write(DEBUG,__FILE__,__FUNCTION__,__LINE__,buf,##__VA_ARGS__);\
}while(0)
#else
#define LogDebug(buf,...) do{}while(0)
#endif

#ifndef NON_USE_NIMO_LOG
#define LogInfo(buf, ...) do{\
log_write(INFO,__FILE__,__FUNCTION__,__LINE__,buf,##__VA_ARGS__);\
}while(0)
#else
#define LogInfo(buf,...) do{}while(0)
#endif

#ifndef NON_USE_NIMO_LOG
#define LogWarning(buf, ...) do{\
log_write(WARNNING,__FILE__,__FUNCTION__,__LINE__,buf,##__VA_ARGS__);\
}while(0)
#else
#define LogWarning(buf,...) do{}while(0)
#endif

#ifndef NON_USE_NIMO_LOG
#define LogError(buf, ...) do{\
log_write(ERROR,__FILE__,__FUNCTION__,__LINE__,buf,##__VA_ARGS__);\
}while(0)
#else
#define LogError(buf,...) do{}while(0)
#endif

/* flush all the buffered content in memory or in os cache
 * 
 * @params log : log handle 
 *
 * */
void nimo_log_flush(log_t *log);

/* get the lastet error message 
 *
 * @return : error message
 *
 * */
const char *nimo_log_last_error(log_t *log);

#ifdef __cplusplus
}
#endif


#endif
