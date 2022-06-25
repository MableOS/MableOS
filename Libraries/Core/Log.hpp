#pragma once

#include <cassert>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

namespace Core
{
    class Log
    {
    public:
        template <typename... Args> static void inline info(const char *format, Args... args)
        {
            write(info_type, format, args...);
        }

        template <typename... Args> static void inline warning(const char *format, Args... args)
        {
            write(warning_type, format, args...);
        }

        template <typename... Args> static void inline error(const char *format, Args... args)
        {
            write(error_type, format, args...);
        }

        template <typename... Args> static void inline perror(const char *format, Args... args)
        {
            int p_errno = errno;
            auto final_format = "%s - %s";
            std::vector<char> buffer(std::snprintf(nullptr, 0, final_format, format, std::strerror(p_errno)) + 1);
            std::snprintf(buffer.data(), buffer.size(), final_format, format, std::strerror(p_errno));

            write(error_type, buffer.data(), args...);
        }

    private:
        enum Type
        {
            info_type,
            warning_type,
            error_type
        };

        static constexpr const char *colors[] = {"\x1b[32m", "\x1b[33m", "\x1b[31m"};

        static void inline write(Type type, const char *format, ...)
        {
            va_list args1;
            va_start(args1, format);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> vsn_buffer(std::vsnprintf(nullptr, 0, format, args1) + 1);
            va_end(args1);
            std::vsnprintf(vsn_buffer.data(), vsn_buffer.size(), format, args2);
            va_end(args2);

            auto final_format = "\x1b[1m%s[%s]\x1b[0m: %s\n";
            auto color = colors[type];
            std::vector<char> buffer(
                std::snprintf(nullptr, 0, final_format, color, program_invocation_short_name, vsn_buffer.data()) + 1);
            std::snprintf(buffer.data(), buffer.size(), final_format, color, program_invocation_short_name,
                          vsn_buffer.data());

            std::fstream file("/dev/kmsg", std::ios_base::out);
            std::copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(file));
            file.close();
        }
    };
} // namespace Core