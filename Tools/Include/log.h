/*
 * Created by lxq, 2021-7-23
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#ifdef log_level
    #pragma message ( \
        "[Warning from log.h]: Macro 'log_level' has been defined, and it will be modified by log.h." \
    )
    #undef log_level
#endif

/* Define log level
 * 0: None
 * 1: Error, output error log (red) and variable log (white)
 * 2: Warning, output both error (red), variable (white), and warning log (yellow)
 * 3: Message, output all kinds of log for debug
 */
#define log_level 3

/* log apis */
#define log_error(format, ...)         __log_error__(format, ##__VA_ARGS__)
// #define log_warning(format, ...)       __log_warning__(format, ##__VA_ARGS__)
#define log_msg(format, ...)           __log_msg__(format, ##__VA_ARGS__)
#define log_var(var_name, format, ...) __log_var__(var_name, format, ##__VA_ARGS__)


/* =========================== Log implementation ========================== */

/* utils */
#define __location__            "At " __FILE__ ":" "%d "
#define __output__(format, ...) fprintf(stdout, format, ##__VA_ARGS__)

/* log levels */
#define __none__    0
#define __error__   1
// #define __warning__ 2
#define __msg__     3

/* check whether log level is valid */
#if log_level < __none__
    #pragma message ( \
        "[Warning from log.h]: Detect log level < 0, set log level to 0 (None)" \
    )
    #undef log_level
    #define log_level __none__
#elif log_level > __msg__
    #pragma message ( \
        "[Warning from log.h]: Detect log level > 3, set log level to 3 (Message)" \
    )
    #undef log_level
    #define log_level __msg__
#endif

/* set private macro "__log_level__" to avoid modification of macro "log_level" */
#if log_level == __none__
    #define __log_level__ __none__
#elif log_level == __error__
    #define __log_level__ __error__
// #elif log_level == __warning__
//     #define __log_level__ __warning__
#else
    #define __log_level__ __msg__
#endif

/* output string format
 * [file] [line] [color] [log type] [origin format string] [\n+]
 */
#define __to_error__(format) \
        __location__ "\033[1;31m" "[Error]  " "\033[0m: " format "\n"

#define __to_warning__(format) \
        __location__ "\033[1;33m" "[Warning]" "\033[0m: " format "\n"

#define __to_msg__(format) \
        __location__ "\033[1;34m" "[Message]" "\033[0m: " format "\n"

#define __to_var__(var_name, format) \
        __location__ "\033[1;37m" "[Var: " var_name "]" "\033[0m: " format "\n"

/* log error */
#if __log_level__ >= __error__
    #define __log_error__(format, ...) \
            __output__(__to_error__(format), __LINE__, ##__VA_ARGS__)
#else
    #define __log_error__(format, ...) ((void)0)
#endif

/* log warning */
// #if __log_level__ >= __warning__
//     #define __log_warning__(format, ...) \
//             __output__(__to_warning__(format), __LINE__, ##__VA_ARGS__)
// #else
//     #define __log_warning__(format, ...) ((void)0)
// #endif

/* log message */
#if __log_level__ >= __msg__
    #define __log_msg__(format, ...) \
            __output__(__to_msg__(format), __LINE__, ##__VA_ARGS__)
#else
    #define __log_msg__(format, ...) ((void)0)
#endif

/* log variable */
#if __log_level__ >= __error__
    #define __log_var__(var_name, format, ...) \
            __output__(__to_var__(var_name, format), __LINE__, ##__VA_ARGS__)
#else
    #define __log_var__(var_name, format, ...) ((void)0)
#endif

#endif
