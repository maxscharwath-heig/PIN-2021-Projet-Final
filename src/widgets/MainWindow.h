#ifndef PIN_2021_SERIE_1_MAINWINDOW_H
#define PIN_2021_SERIE_1_MAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Scroll.H>
#include <Fl/Fl_Counter.H>
#include "WorldWidget.h"
#include "../core/World.h"

class MainWindow : public Fl_Window {
   static const int MIN_WIDTH;
   static const int MIN_HEIGHT;
   static const char DEFAULT_LABEL[];
   static const double DEFAULT_SPEED;

   Fl_Group* fileLoadGroup, * simButtonsGroup;

   Fl_Input* inputFile;
   Fl_Counter* speedCounter;
   Fl_Button* openButton,
         * eraseButton,
         * resetButton,
         * playPauseButton,
         * quitButton;

   WorldWidget* worldWidget;
   Fl_Scroll* scrollbars;

public:
   MainWindow();

   void resize(int X, int Y, int W, int H) override;

   static void updateScoreLabel(Fl_Widget* widget, void* data, int newScore);

   static void erase_dp(Fl_Widget* widget, void* data);

private:
   static void openFile(Fl_Widget* widget, void* data);

   static void registerFileLoadButtons(Fl_Widget* widget, void* data);

   static void registerSimulationButtons(Fl_Widget* widget, void* data);

   static void quit_cb(Fl_Widget* widget, void* data);

   static void toggle_sim_buttons(Fl_Widget* widget, void* data, bool shouldShow);

   static void toggle_start_pause(Fl_Widget* widget, void* data);

   static void reset_speed(Fl_Widget* widget, void* data);

   static void updateSpeed(Fl_Widget* widget, void* data);

   int getFittedW();

   int getFittedH();

   double speedCounterValue;

   std::string loadedFilename;
};

#endif //PIN_2021_SERIE_1_MAINWINDOW_H