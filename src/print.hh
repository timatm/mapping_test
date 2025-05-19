#ifndef __PRINT_HH__
#define __PRINT_HH__

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdarg>
#include <cstdio>

#define DEBUG

namespace logger {

enum class Level { Info, Debug };

// 時間戳記
// inline std::string timestamp() {
//     using namespace std::chrono;
//     auto now   = system_clock::now();
//     auto tt    = system_clock::to_time_t(now);
//     auto local = *std::localtime(&tt);

//     std::ostringstream os;
//     os << std::put_time(&local, "%F %T");
//     return os.str();
// }

// 輸出統一格式：時間 + 等級 + 訊息
inline void print(Level lvl, const std::string& msg) {
    // std::cout << "[" << timestamp() << "] ";
    switch (lvl) {
        case Level::Info:  std::cout << "[INFO]  "; break;
        case Level::Debug: std::cout << "[DEBUG] "; break;
    }
    std::cout << msg << std::endl;
}

// C-style format 包裝：安全地格式化字串
template<typename... Args>
inline std::string fmt_str(const char* fmt, Args... args) {
    constexpr size_t size = 1024;
    char buffer[size];
    std::snprintf(buffer, size, fmt, args...);  // 安全限制長度
    return std::string(buffer);
}

// 封裝成 info/debug 格式化介面
template<typename... Args>
inline void infof(const char* fmt, Args... args) {
    print(Level::Info, fmt_str(fmt, args...));
}

template<typename... Args>
inline void debugf(const char* fmt, Args... args) {
    print(Level::Debug, fmt_str(fmt, args...));
}

} // namespace logger

// ────────────── 使用者介面 ──────────────
#define pr_info(...)  logger::infof(__VA_ARGS__)

#ifdef DEBUG
  #define pr_debug(...) logger::debugf(__VA_ARGS__)
#else
  #define pr_debugf(...) ((void)0)
#endif

#endif // __PRINT_HH__
