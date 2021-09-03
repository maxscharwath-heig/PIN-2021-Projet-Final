#ifndef PIN_2021_SERIE_2_LOGGER_H
#define PIN_2021_SERIE_2_LOGGER_H

#include <vector>
#include <string>
#include <ctime>

enum class LoggerType {
   SCORE, R_VELOCITY, R_COLLISION, P_COLLISION, WARNING
};
const std::vector<std::string> LOGGER_TYPE_NAMES = {"scores", "vitessesR",
                                                    "collisionsR", "collisionsP",
                                                    "warning"};

class Logger {
public:
   Logger(LoggerType loggerType, const std::string& message);

   static void Log(LoggerType loggerType, const std::string& message);

private:
   LoggerType loggerType;
   std::string message;
   time_t eventTime;
};


#endif //PIN_2021_SERIE_2_LOGGER_H
