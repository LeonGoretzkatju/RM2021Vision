#ifndef __LOG_FORMATTER_H__
#define __LOG_FORMATTER_H__

#include <unistd.h>
#include <sys/syscall.h>
#include <iostream> /* for debug. */
#include "fmt/format.h"
#include "log_type.hpp"


class LogFormatter
{
public:
    LogFormatter();
    ~LogFormatter();
    const std::string format(const std::string& format, const LogType log_type, const std::string& file, const int line, bool to_terminal);

private:
    const std::string _error_tag;
    const std::string _error_tag_terminal;

    const std::string _warning_tag;
    const std::string _warning_tag_terminal;

    const std::string _msg_tag;
    const std::string _msg_tag_terminal;

    const std::string _format_error(const std::string& format, bool to_terminal);
    const std::string _format_warning(const std::string& format, bool to_terminal);
    const std::string _format_msg(const std::string& format, bool to_terminal);
    const std::string _append_info(const std::string& format, const std::string& file, const int line);
};


#endif