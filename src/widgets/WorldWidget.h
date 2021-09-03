#ifndef PIN_2021_SERIE_1_WORLDWIDGET_H
#define PIN_2021_SERIE_1_WORLDWIDGET_H

#include <FL/Fl_Widget.H>
#include <chrono>
#include "../core/Scale.h"
#include "../core/Canvas.h"
#include "../core/World.h"

class WorldWidget : public Fl_Widget {

private:
   int timer = 0;
   bool simulationPaused = true;
   static const int MAX_COLOR = 8;
   const std::array<int, MAX_COLOR> colors = {FL_BLACK, FL_WHITE, FL_RED, 90,
                                              FL_YELLOW, FL_GREEN, FL_BLUE, 161};

   void updateTimeLabel();

   std::string timeLabel;
public:
   WorldWidget(int x, int y, int w, int h);

   void draw() override;

   void parseFile(const std::string& fileName);

   void setPaused(bool paused);

   bool getPaused() const;

   World* world;

   void static perform(void* userdata);

   void static refreshView(void* userdata);

   static double refreshCycle;
   static double timeMultiplier;
   std::chrono::time_point<std::chrono::high_resolution_clock> lastTickTime;
   Scale* scale;
   Canvas* canvas;
};

#endif //PIN_2021_SERIE_1_WORLDWIDGET_H
