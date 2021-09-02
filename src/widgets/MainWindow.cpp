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
                           speedInputValue(DEFAULT_SPEED) {
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
   window->inputFile = new Fl_Input(25, TOOLBAR_Y, 225, DEFAULT_BTN_H, "Fichier");
   window->inputFile->align(FL_ALIGN_TOP_LEFT);

   window->inputFile->value("demo-particule.dat"); // Temporary for testing

   window->openButton = new Fl_Button(window->w() - 480, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "ouvrir");
   window->openButton->callback(openFile, window);

   window->eraseButton = new Fl_Button(window->w() - 410, TOOLBAR_Y, DEFAULT_BTN_W, DEFAULT_BTN_H, "effacer");
   window->eraseButton->callback(erase_dp, window);
   window->fileLoadGroup->end();
}

void MainWindow::registerSimulationButtons(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->simButtonsGroup = new Fl_Group(window->w() - 350, TOOLBAR_Y, 300, 50);
   {
      window->fastLowerSpeedButton = new Fl_Button(window->w() - 350, TOOLBAR_Y, 15, DEFAULT_BTN_H,
                                                   "@<<");
      window->lowerSpeedButton = new Fl_Button(window->w() - 335, TOOLBAR_Y, 15, DEFAULT_BTN_H, "@<");
      window->fastLowerSpeedButton->callback(fastLowerSpeed, window);
      window->lowerSpeedButton->callback(lowerSpeed, window);

      window->speedInput = new Fl_Input(window->w() - 320, TOOLBAR_Y, 30, DEFAULT_BTN_H, "Vitesse");
      window->speedInput->align(FL_ALIGN_TOP_LEFT);
      updateSpeed(window, window);

      window->greaterSpeedButton = new Fl_Button(window->w() - 290, TOOLBAR_Y, 15, DEFAULT_BTN_H, "@>");
      window->fastGreaterSpeedButton = new Fl_Button(window->w() - 275, TOOLBAR_Y, 15, DEFAULT_BTN_H,
                                                     "@>>");
      window->fastGreaterSpeedButton->callback(fastGreaterSpeed, window);
      window->greaterSpeedButton->callback(greaterSpeed, window);

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
      WorldWidget::timeMultiplier = window->speedInputValue;
   }
   catch (const std::exception& e) {
      window->label(DEFAULT_LABEL);
      std::cerr << e.what() << std::endl;

      fl_beep(FL_BEEP_ERROR);
      return;
   }

   window->scrollbars->show();
   window->label(inputPath.substr(inputPath.find_last_of("/\\") + 1).data());

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

   window->speedInputValue = DEFAULT_SPEED;
   updateSpeed(window, window);
}

void MainWindow::fastLowerSpeed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   if (window->speedInputValue <= 1.0) {
      window->speedInputValue = 0.1;
   } else {
      --window->speedInputValue;
   }
   updateSpeed(window, window);
}

void MainWindow::lowerSpeed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   if (window->speedInputValue <= 0.2) {
      window->speedInputValue = 0.1;
   } else {
      window->speedInputValue -= 0.1;
   }
   updateSpeed(window, window);
}

void MainWindow::fastGreaterSpeed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;
   window->speedInputValue++;    // Is there a max speed ?
   updateSpeed(window, window);
}

void MainWindow::greaterSpeed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;
   window->speedInputValue += 0.1;

   updateSpeed(window, window);
}

void MainWindow::updateSpeed(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   std::ostringstream os;
   os << std::setprecision(4) << window->speedInputValue;

   WorldWidget::timeMultiplier = window->speedInputValue;
   window->speedInput->value(os.str().data());
}

int MainWindow::getFittedH()
{
   if (Fl::w() < worldWidget->w()) {
      return Fl::h();
   } else if (MIN_HEIGHT >= worldWidget->h()) {
      return MIN_HEIGHT;
   }
   return worldWidget->y() + worldWidget->h() + MARGIN * w();
}

int MainWindow::getFittedW()
{
   if (Fl::w() < worldWidget->w()) {
      return Fl::w();
   }
   else if (MIN_WIDTH >= worldWidget->w()) {
      return MIN_WIDTH;
   }
   return worldWidget->w() + MARGIN * w();
}


