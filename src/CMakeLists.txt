cmake_minimum_required(VERSION 3.17)
project(PIN-2021-Serie-2)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion -pedantic")

FIND_PACKAGE(FLTK REQUIRED NO_MODULE)

add_executable(Serie2 WIN32 MACOSX_BUNDLE main.cpp widgets/WorldWidget.cpp widgets/WorldWidget.h widgets/MainWindow.cpp widgets/MainWindow.h core/World.cpp core/World.h core/Scale.cpp core/Scale.h core/Canvas.cpp core/Canvas.h objects/Particule.cpp objects/Particule.h objects/Robot.cpp objects/Robot.h objects/Object.cpp objects/Object.h widgets/Fl_Center_Scroll.h utils/Defaults.h utils/Logger.h utils/Logger.cpp utils/Coordinator.cpp utils/Coordinator.h utils/Utils.h)

if (APPLE)
    target_link_libraries(Serie2 "-framework cocoa")
endif (APPLE)

target_include_directories(Serie2 PUBLIC ${FLTK_INCLUDE_DIRS})
target_link_libraries(Serie2 fltk)
