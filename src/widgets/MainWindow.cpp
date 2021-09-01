#include <iostream>
#include <string>
#include <iomanip>
#include "MainWindow.h"
#include "Fl_Center_Scroll.h"

const int MainWindow::MIN_WIDTH = 750;
const int MainWindow::MIN_HEIGHT = 300;
const char MainWindow::DEFAULT_LABEL[] = "PIN-2021 Série 2";
const double MainWindow::DEFAULT_SPEED = 1.0;

MainWindow::MainWindow() : Fl_Window(MIN_WIDTH, MIN_HEIGHT, DEFAULT_LABEL),
                           speedInputValue(DEFAULT_SPEED) {
   size_range(MIN_WIDTH, MIN_HEIGHT);
   begin();

   registerFileLoadButtons(this, this);
   registerSimulationButtons(this, this);

   quitButton = new Fl_Button(w() - 100, 25, 50, 50, "quitter");
   quitButton->callback(quit_cb, this);

   scrollbars = new Fl_Center_Scroll(25, 100, w() - 50, h() - 125);
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

   window->fileLoadGroup = new Fl_Group(window->w() - 725, 25, 400, 50);
   window->inputFile = new Fl_Input(25, 25, 225, 50, "Fichier");
   window->inputFile->align(FL_ALIGN_TOP_LEFT);

   window->inputFile->value("demo_fltk.dat"); // Temporary for testing

   window->openButton = new Fl_Button(window->w() - 480, 25, 50, 50, "ouvrir");
   window->openButton->callback(openFile, window);

   window->eraseButton = new Fl_Button(window->w() - 410, 25, 50, 50, "effacer");
   window->eraseButton->callback(erase_dp, window);
   window->fileLoadGroup->end();
}

void MainWindow::registerSimulationButtons(Fl_Widget* widget, void* data) {
   auto* window = (MainWindow*) data;

   window->simButtonsGroup = new Fl_Group(window->w() - 350, 25, 300, 50);
   {
      window->fastLowerSpeedButton = new Fl_Button(window->w() - 350, 25, 15, 50,
                                                   "@<<");
      window->lowerSpeedButton = new Fl_Button(window->w() - 335, 25, 15, 50, "@<");
      window->fastLowerSpeedButton->callback(fastLowerSpeed, window);
      window->lowerSpeedButton->callback(lowerSpeed, window);

      window->speedInput = new Fl_Input(window->w() - 320, 25, 30, 50, "Vitesse");
      window->speedInput->align(FL_ALIGN_TOP_LEFT);
      updateSpeed(window, window);

      window->greaterSpeedButton = new Fl_Button(window->w() - 290, 25, 15, 50, "@>");
      window->fastGreaterSpeedButton = new Fl_Button(window->w() - 275, 25, 15, 50,
                                                     "@>>");
      window->fastGreaterSpeedButton->callback(fastGreaterSpeed, window);
      window->greaterSpeedButton->callback(greaterSpeed, window);

      window->resetButton = new Fl_Button(window->w() - 240, 25, 50, 50, "reset");
      window->resetButton->callback(reset_speed, window);

      window->playPauseButton = new Fl_Button(window->w() - 170, 25, 50, 50, "go");
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
      std::cout << "\a" << e.what() << std::endl;
      return;
   }

   window->scrollbars->show();
   window->label(inputPath.substr(inputPath.find_last_of("/\\") + 1).data());

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
   } else {
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


