#include "Logger.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>

Logger::Logger(LoggerType loggerType, const std::string& message):loggerType(loggerType), message(message) {
   eventTime = time(nullptr);

   std::fstream logFile;
   // TODO: add .dat filename before loggertype
   logFile.open("1_" + LOGGER_TYPE_NAMES[static_cast<size_t>(loggerType)] + ".log", std::fstream::app);

   std::stringstream ss;
   tm *gmTime = gmtime(&eventTime);
   ss << std::setw(2) << std::setfill('0') << gmTime->tm_hour << ":"
      << std::setw(2) << std::setfill('0') << gmTime->tm_min << ":"
      << std::setw(2) << std::setfill('0') << gmTime->tm_sec << " "
      << std::setw(2) << std::setfill('0') << 1 + gmTime->tm_mday << "."
      << std::setw(2) << std::setfill('0') << gmTime->tm_mon << "."
      << 1900 + gmTime->tm_year;

   logFile << ss.str() << " " << message << std::endl;
   logFile.close();
}

void Logger::Log(LoggerType loggerType, const std::string& message) {
   new Logger(loggerType, message);
}