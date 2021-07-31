#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <mutex>
#include <fstream>
#include <iostream> /* for debug. */
#include <set>
#include <opencv2/opencv.hpp>

#include "fmt/format.h"
#include "log_type.hpp"
#include "log_formatter.hpp"


/* Logger version 1.0
 * 已完成：
 *      (1) 提供3种级别的log，并且可以在编译期关闭，即在程序运行时无函数调用。
 * 
 *      (2) 完成前端最基本的对log的格式化。
 * 
 *      (3) log可以同时输出到终端与文件，并且考虑到同时有多个线程调用logger的
 *          写入api，因此输出到终端与文件已实现线程安全。
 *
 *      (4) 支持非线程安全的图片保存，并且设置了tag机制，可以在main函数开头处设置
 *          保留哪些图片。
 * 
 * 存在的问题：
 *      (1) 采用的是同步写入, 即让调用logger api的线程同时负责写入。
 *          优点是：保证log写入及时，对找段错误等即时性问题有好处。
 *          缺点是：可能会对程序性能带来一定影响，可以通过关闭log输出来缓解。
 *          如果有必要，需要添加异步写入，并且同步异步可选。
 *      
 *      (2) save_image不是线程安全的。但是考虑到只有主线程会调用，因此在不影响
 *          主线程性能的情况下不需要保证线程安全。
 */


class Logger
{
private:
    static Logger* _logger;
    Logger();
    ~Logger();

    /* Decide whether save log into local. */
    bool _save_log_to_file;

    /* Log file's path if _save_log_to_file is true. */
    std::string _log_file_path;

    /* File object. */
    std::ofstream _log;

    /* Decide whether print log into terminal. */
    bool _print_log_to_terminal;

    /* Lock for  output. */
    std::mutex _terminal_lock;
    std::mutex _log_file_lock;

    /* Formatter, formatter and macro header are both logger's frontend. */
    LogFormatter _formatter;

    /* Tags of funtion which call save_image */
    std::set<std::string> _ignored_call_tags;

public:
    static Logger* get_logger() {
        if (!Logger::_logger) {
            Logger::_logger = new Logger();
        }
        return Logger::_logger;
    }

    /* Synchronous printf with std::mutex. */
    template<typename... args>
    void _sync_print(const std::string& format, args... va_args);

    template<typename... args>
    void _sync_flush(const std::string& format, args... va_args);

    /* Save log to terminal and file (if enabled). */
    template<typename... args>
    void save_log(const std::string& format, const LogType log_type, const std::string& file, const int line, args... va_args);

    /* Enable logger to output log to a specific path. */
    void enable_save_log_to_file(const std::string& log_file_path);

    /* Disable logger to output log to terminal. */
    void close_print_log_to_terminal();

    /* Add ignore function tags. */
    void add_ignored_call_tag(std::string ignored_call_tag);

    /* Save image with tag and file path. */
    void save_image(cv::Mat& img, const std::string& path, const std::string& tag);
};

template<typename... args>
void Logger::_sync_print(const std::string& format, args... va_args) {
    this->_terminal_lock.lock();
    fmt::print(format, va_args...);
    this->_terminal_lock.unlock();
}

template<typename... args>
void Logger::_sync_flush(const std::string& format, args... va_args) {
    this->_log_file_lock.lock();
    this->_log << fmt::format(format, va_args...);
    this->_log.flush();
    this->_log_file_lock.unlock();
}

template<typename... args>
void Logger::save_log(const std::string& format, const LogType log_type, const std::string& file,
                    const int line, args... va_args) {
    if (this->_print_log_to_terminal) {
        const std::string& internal_format = this->_formatter.format(
            format, log_type, file, line, true
        );
        this->_sync_print(internal_format, va_args...);
    }
    
    // TODO: 如果存在性能瓶颈，则添加异步写入。
    if (this->_save_log_to_file) {
        const std::string& internal_format = this->_formatter.format(
            format, log_type, file, line, false
        );
        this->_sync_flush(internal_format, va_args...);
    }
}

#endif