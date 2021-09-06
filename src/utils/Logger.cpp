#include "Logger.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>

std::string Logger::fileInputName;

Logger::Logger(LoggerType loggerType, const std::string& message) : loggerType(
        loggerType), message(message) {
   eventTime = time(nullptr);

   std::string fileExtension = ".dat";
   std::string::size_type i = fileInputName.find(fileExtension);
   if (i != std::string::npos)
      fileInputName.erase(i, fileExtension.length());

   std::fstream logFile;
   logFile.open(fileInputName + "_" + LOGGER_TYPE_NAMES[static_cast<size_t>(loggerType)] + ".log",
                std::fstream::app);
/*
   // Format datetime
   std::stringstream ss;
   tm* gmTime = gmtime(&eventTime);
   ss << std::setw(2) << std::setfill('0') << gmTime->tm_hour << ":"
      << std::setw(2) << std::setfill('0') << gmTime->tm_min << ":"
      << std::setw(2) << std::setfill('0') << gmTime->tm_sec << " "
      << std::setw(2) << std::setfill('0') << gmTime->tm_mday << "."
      << std::setw(2) << std::setfill('0') << 1 + gmTime->tm_mon << "."
      << 1900 + gmTime->tm_year;
*/
   logFile << message << std::endl;
   logFile.close();
}

void Logger::Log(LoggerType loggerType, const std::string& message) {
   new Logger(loggerType, message);
}

void Logger::setFileInputName(const std::string &fileName) {
   fileInputName = fileName;
}