#include "log_formatter.hpp"


LogFormatter::LogFormatter()
/* So ugly ? */
: _error_tag_terminal("\033[1;31m[Error]\033[0m"),     _error_tag("[Error]"),
  _warning_tag_terminal("\033[1;33m[Warning]\033[0m"), _warning_tag("[Warning]"),
  _msg_tag_terminal("\033[1;34m[Message]\033[0m"),     _msg_tag("[Message]")
{};

LogFormatter::~LogFormatter()
{};

const std::string LogFormatter::format(const std::string& format, const LogType log_type,
                       const std::string& file, const int line, bool to_terminal) {
    switch (log_type)
    {
    case LogType::ERROR:
        return this->_append_info(this->_format_error(format, to_terminal), file, line);
        break;

    case LogType::WARNING:
        return this->_append_info(this->_format_warning(format, to_terminal), file, line);
        break;

    case LogType::MSG:
        return this->_append_info(this->_format_msg(format, to_terminal), file, line);
        break;

    default:
        /* This branch will never be reached. */
        break;
    }
}

/*
 * If other kind of runtime information is requried, u just need
 * to add it in this function.
 */
const std::string LogFormatter::_append_info(
        const std::string& format, const std::string& file, const int line
    ) {
    return fmt::format("At {}: {:>5}, thread id:{:8}, {}", file, line, syscall(SYS_gettid), format);
}

/*
 * "20" and "9" are specially choiced.
 */
const std::string LogFormatter::_format_error(const std::string& format, bool to_terminal) {
    if (to_terminal)
        return fmt::format("type: {:<20}, {}\n", this->_error_tag_terminal, format);
    else
        return fmt::format("type: {:<9}, {}\n", this->_error_tag, format);
}

const std::string LogFormatter::_format_warning(const std::string& format, bool to_terminal) {
    if (to_terminal)
        return fmt::format("type: {:<20}, {}\n", this->_warning_tag_terminal, format);
    else
        return fmt::format("type: {:<9}, {}\n", this->_warning_tag, format);
}

const std::string LogFormatter::_format_msg(const std::string& format, bool to_terminal) {
    if (to_terminal)
        return fmt::format("type: {:<20}, {}\n", this->_msg_tag_terminal, format);
    else
        return fmt::format("type: {:<9}, {}\n", this->_msg_tag, format);
}