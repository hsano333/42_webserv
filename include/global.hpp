#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <string>
#include "log.hpp"

#define MAX_FD (200)
#define MAX_BUF (4095)
#define MAX_READ (1024)
#define LOG_FILE "./webserv.log"
#define LOG_LEVEL_NOT_LOG 5
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL LOG_LEVEL_INFO
#define INFO(STR)  (Log::write(__FILE__, __LINE__, "INFO:", STR, LOG_LEVEL_INFO))
#define DEBUG(STR)  (Log::write(__FILE__, __LINE__, "DEBUG:",  STR, LOG_LEVEL_DEBUG))
#define WARNING(STR)  (Log::write(__FILE__, __LINE__, "WARNING:" , STR, LOG_LEVEL_WARNING))
#define ERROR(STR)  (Log::write(__FILE__, __LINE__,  "ERROR", STR, LOG_LEVEL_ERROR))



#endif
