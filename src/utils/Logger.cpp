#include "Logger.h"
#include <fstream>
#include <iostream>
#include <utility>

std::string Logger::fileInputName;

Logger::Logger(LoggerType loggerType, const std::string& message) : loggerType(
        loggerType), message(message) {

   std::string fileExtension = ".dat";
   std::string::size_type i = fileInputName.find(fileExtension);
   if (i != std::string::npos)
      fileInputName.erase(i, fileExtension.length());

   std::fstream logFile;
   logFile.open(fileInputName + "_" + LOGGER_TYPE_NAMES[static_cast<size_t>(loggerType)] + ".log",
                std::fstream::app);
   logFile << message << std::endl;
   logFile.close();
}

void Logger::Log(LoggerType loggerType, const std::string& message) {
   new Logger(loggerType, message);
}

void Logger::setFileInputName(const std::string &fileName) {
   fileInputName = fileName;
}