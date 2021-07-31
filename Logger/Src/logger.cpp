#include "logger.hpp"


Logger* Logger::_logger = nullptr;

Logger::Logger()
:_save_log_to_file(false), _print_log_to_terminal(true)
{};

Logger::~Logger() {
    if (this->_save_log_to_file) {
        this->_log.close();
    }
}

void Logger::enable_save_log_to_file(const std::string& log_file_path) {
    this->_save_log_to_file = true;
    this->_log_file_path = log_file_path;
    this->_log.open(log_file_path);
}

void Logger::close_print_log_to_terminal() {
    this->_print_log_to_terminal = false;
}

void Logger::add_ignored_call_tag(std::string ignored_call_tag) {
    this->_ignored_call_tags.insert(ignored_call_tag);
}

void Logger::save_image(cv::Mat& img, const std::string& path, const std::string& tag) {
    /* TODO: 异步保存图片？ 使用主线程保存可能会很耗时间 */
    if (!this->_ignored_call_tags.count(tag))
        cv::imwrite(path, img);    
}