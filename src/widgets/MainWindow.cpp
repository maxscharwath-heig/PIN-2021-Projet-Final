#include <iostream>
#include <string>
#include <iomanip>
#include <FL/Fl.H>
#include "FL/fl_ask.H"
#include "MainWindow.h"
#include "Fl_Center_Scroll.h"
#include "../utils/Defaults.h"

const int MainWindow::MIN_WIDTH = 750;
const int MainWindow::MIN_HEIGHT = 300;
const char MainWindow::DEFAULT_LABEL[] = "PIN-2021 Projet Final";
const double MainWindow::DEFAULT_SPEED = 1.0;

MainWindow::MainWindow() : Fl_Window(MIN_WIDTH, MIN_HEIGHT, DEFAULT_LABEL),
                           speedCounterValue(DEFAULT_SPEED) {
   size_range(MIN_WIDTH, MIN_HEIGHT);
   begin();

   registerFileLoadButtons(this, this);
   registerSimulationButtons(this, this);

   quitButton = new Fl_Button(w() - 100, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "quitter");
   quitButton->callback(quit_cb, this);

   scrollbars = new Fl_Center_Scroll(25, 100, w() - 50, h() - 125);
   scrollbars->align(FL_ALIGN_TOP_LEFT);
   scrollbars->begin();
   {
      worldWidget = new WorldWidget(scrollbars->x(), scrollbars->y(), 0, 0);
   }
   scrollbars->end();
   toggle_sim_buttons(this, this, false);

   end();
   resizable(scrollbars);
}

void MainWindow::registerFileLoadButtons(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->fileLoadGroup = new Fl_Group(window->w() - 725, TOOLBAR_Y, 400, DEFAULT_BTN_H);
   window->inputFile = new Fl_Input(TOOLBAR_Y, TOOLBAR_Y, INPUT_W, DEFAULT_BTN_H, "Fichier");
   window->inputFile->align(FL_ALIGN_TOP_LEFT);

   window->inputFile->value("demo.dat"); // Temporary for testing

   window->openButton = new Fl_Button(window->w() - 480, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "ouvrir");
   window->openButton->callback(openFile, window);

   window->eraseButton = new Fl_Button(window->w() - 410, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "effacer");
   window->eraseButton->callback(erase_dp, window);
   window->fileLoadGroup->end();
}

void MainWindow::registerSimulationButtons(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->simButtonsGroup = new Fl_Group(window->w() - 350, TOOLBAR_Y, 300, DEFAULT_BTN_H);
   {
      window->speedCounter = new Fl_Counter(window->w() - 350, TOOLBAR_Y, LARGE_BTN_W, DEFAULT_BTN_H, "Vitesse");
      window->speedCounter->align(FL_ALIGN_TOP_LEFT);
      window->speedCounter->value(DEFAULT_SPEED);
      window->speedCounter->minimum(MIN_SPEED_COUNTER);
      window->speedCounter->maximum(MAX_SPEED_COUNTER);
      window->speedCounter->callback(updateSpeed, window);
      window->speedCounter->when(1);
      updateSpeed(window, window);

      window->resetButton = new Fl_Button(window->w() - 240, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "reset");
      window->resetButton->callback(reset_speed, window);

      window->playPauseButton = new Fl_Button(window->w() - 170, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "go");
      window->playPauseButton->callback(toggle_start_pause, window);
   }
   window->simButtonsGroup->end();
}

void MainWindow::erase_dp(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;
   toggle_sim_buttons(window, data, false);
   window->worldWidget->world->clear();
   window->scrollbars->hide();
   window->loadedFilename.clear();
   window->label(DEFAULT_LABEL);
}

void MainWindow::quit_cb(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;
   window->hide();
}

void MainWindow::resize(int X, int Y, int W, int H) {
   Fl_Window::resize(X, Y, W, H);
}

void MainWindow::openFile(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->worldWidget->world->clear();
   std::string inputPath = window->inputFile->value();

   try {
      window->worldWidget->parseFile(inputPath);
   }
   catch (const std::exception& e) {
      window->label(DEFAULT_LABEL);
      std::cerr << e.what() << std::endl;
      fl_beep(FL_BEEP_ERROR);
      return;
   }

   WorldWidget::timeMultiplier = window->speedCounterValue;
   window->scrollbars->show();

   window->loadedFilename = inputPath.substr(inputPath.find_last_of("/\\") + 1);

   updateScoreLabel(window, data, 0);

   window->resize(window->x(), window->y(), window->getFittedW(), window->getFittedH());

   toggle_sim_buttons(window, data, true);
}

void MainWindow::toggle_sim_buttons(Fl_Widget* widget, void* data, bool shouldShow) {
   auto* window = (MainWindow*) data;

   shouldShow ? window->simButtonsGroup->show() : window->simButtonsGroup->hide();
}

void MainWindow::toggle_start_pause(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->worldWidget->setPaused(!window->worldWidget->getPaused());

   if (window->worldWidget->getPaused()) {
      window->playPauseButton->label("go");
      window->fileLoadGroup->activate();
   }
   else {
      window->playPauseButton->label("pause");
      window->fileLoadGroup->deactivate();
   }
}

void MainWindow::reset_speed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->speedCounter->value(DEFAULT_SPEED);
   updateSpeed(window, window);
}


void MainWindow::updateSpeed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->speedCounterValue = window->speedCounter->value();
   WorldWidget::timeMultiplier = window->speedCounter->value();
}

int MainWindow::getFittedH()
{
   if (Fl::w() < worldWidget->w()) {
      return Fl::h();
   }
   return MIN_HEIGHT >= worldWidget->h() ? MIN_HEIGHT : worldWidget->y() + worldWidget->h() + MARGIN * w();
}

int MainWindow::getFittedW()
{
   if (Fl::w() < worldWidget->w()) {
      return Fl::w();
   }
   return MIN_WIDTH >= worldWidget->w() ? MIN_WIDTH : worldWidget->w() + MARGIN * w();
}

void MainWindow::updateScoreLabel(Fl_Widget *widget, void *data, unsigned newScore)
{
   auto* window = (MainWindow*) data;

   std::stringstream ss;
   ss << window->loadedFilename + " - " << std::setfill('0') << std::setw(3) << newScore << "%";

   window->label(ss.str().data());
}


