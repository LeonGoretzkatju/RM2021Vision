#ifndef __LOG_H__
#define __LOG_H__

#include "logger.hpp"

#ifdef __log_level__
    #pragma message ( \
        "[Warning from log.hpp]: Macro '__log_level__' has been defined, and it will be modified by log.hpp." \
    )
    #undef __log_level__
#endif

/* Define log level
 * 0: None.
 * 1: Error, output error log (red).
 * 2: Warning, output both error (red) and warning log (yellow).
 * 3: Message, output both error (red) and warning log (yellow) and message (blue).
 */
#define __log_level__   0

/* Macros for logger configuration. */
#define enable_save_log_to_file(log_file_path) logger_ptr->enable_save_log_to_file(log_file_path);
#define close_print_log_to_terminal()          logger_ptr->close_print_log_to_terminal();

/* Macros for log */
#define log_error(format, ...)                 __log_error__(format, ##__VA_ARGS__);
#define log_warning(format, ...)               __log_warning__(format, ##__VA_ARGS__);
#define log_msg(format, ...)                   __log_msg__(format, ##__VA_ARGS__);
#define sync_print(format, ...)                logger_ptr->_sync_print(format, ##__VA_ARGS__);
#define py_format(_format, ...)                fmt::format(_format, ##__VA_ARGS__)

/* Macros for image */
#define add_ignored_call_tag(tag)              logger_ptr->add_ignored_call_tag(tag);
#define save_image(cv_mat, path, tag)          __save_image__(cv_mat, path, tag)


/* =========================== Log implementation ========================== */
#define logger_ptr Logger::get_logger()

/* log levels (types) */
#define __NONE__    0
#define __ERROR__   1
#define __WARNING__ 2
#define __MSG__     3

/* check whether log level is valid */
#if __log_level__ < __NONE__
    #pragma message ( \
        "[Warning from log.hpp]: Detect log level < 0, set log level to 0 (None)" \
    )
    #undef __log_level__
    #define __log_level__ __NONE__
#elif __log_level__ > __MSG__
    #pragma message ( \
        "[Warning from log.hpp]: Detect log level > 3, set log level to 3 (Message)" \
    )
    #undef __log_level__
    #define __log_level__ __MSG__
#endif

/* log error */
#if __log_level__ >= __ERROR__
    #define __log_error__(format, ...) \
    do { \
        logger_ptr->save_log(format, LogType::ERROR, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0);
#else
    #define __log_error__(format, ...) ((void)0)
#endif

/* log warning */
#if __log_level__ >= __WARNING__
    #define __log_warning__(format, ...) \
    do { \
        logger_ptr->save_log(format, LogType::WARNING, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0);
#else
    #define __log_warning__(format, ...) ((void)0)
#endif

/* log message */
#if __log_level__ >= __MSG__
    #define __log_msg__(format, ...) \
    do { \
        logger_ptr->save_log(format, LogType::MSG, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0);
#else
    #define __log_msg__(format, ...) ((void)0)
#endif

/* save image */
#define __save_image__(cv_mat, path, tag) \
do { \
    logger_ptr->save_image(cv_mat, path, tag); \
} while (0);

#endif