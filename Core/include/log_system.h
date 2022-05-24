/*!
 * @author  chenghua.wang
 * @brief   A simple wrapper for spdlog
 */

#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif //! end if msc version

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "core_base.h"

///< parameter s hould be a std::string. Qt-string is not support.
#define LOG_INFO(s) p_logger::get_instance()->info(s);
#define LOG_DEBUG(s) p_logger::get_instance()->debug(s);
#define LOG_ERROR(s) p_logger::get_instance()->error(s);
#define LOG_WARN(s) p_logger::get_instance()->warn(s);
#define LOG_BUFFER_FLUSH p_logger::get_instance()->m_log_->flush();

class p_logger{
public:
    p_logger();
    p_logger(const std::string &file_path);
    ~p_logger() {
        m_log_->flush();
    }

    static p_logger *get_instance();

//    template<typename ...Args>
//    static void info(const Args&... args) {
//        m_log_->info(args);
//    }

    static void info(const std::string &rhs);
    static void debug(const std::string &rhs);
    static void error(const std::string &rhs);
    static void warn(const std::string &rhs);

public:
    static REF(spdlog::logger) m_log_;

private:
    std::string         m_file_path;
    static p_logger    *m_s_instance;
};

#endif // LOG_SYSTEM_H
