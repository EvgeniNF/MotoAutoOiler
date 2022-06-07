#pragma once

#include <iostream>

#define LOG_INFO(logger, message) std::cout << "[INFO] [" << logger << "] {" << message << "}" << std::endl

#define LOG_ERROR(logger, message) std::cout << "[ERROR] [" << logger << "] {" << message << "}" << std::endl

#define LOG_WARNIG(logger, message) std::cout << "[WARNING] [" << logger << "] {" << message << "}" << std::endl
