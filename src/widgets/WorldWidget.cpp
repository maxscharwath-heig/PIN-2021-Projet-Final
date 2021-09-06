#include "WorldWidget.h"
#include "Fl_Center_Scroll.h"
#include "MainWindow.h"
#include "../utils/Validator.h"
#include <FL/fl_draw.H>
#include <iostream>
#include <fstream>
#include <FL/Fl.H>
#include <Fl/fl_ask.H>
#include <iomanip>
#include <chrono>

double WorldWidget::refreshCycle = 1. / 30.; // 30FPS
double WorldWidget::timeMultiplier = 1.;

WorldWidget::WorldWidget(int x, int y, int w, int h) : Fl_Widget(x, y, w, h, "") {
   world = new World();
   canvas = nullptr;
   scale = nullptr;
   this->lastTickTime = std::chrono::high_resolution_clock::now();
}

void WorldWidget::perform(void* userdata) {
   auto* o = (WorldWidget*) userdata;
   auto now = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = now - o->lastTickTime;
   o->lastTickTime = now;
   double timeCorrector = elapsed.count() / refreshCycle;

   if (!o->simulationPaused) {
      double deltaTime = refreshCycle * timeMultiplier * timeCorrector;
      o->world->update(o, deltaTime);

      if (o->world->particules.empty()) {
         o->setPaused(true);
         std::stringstream ss;
         ss << "Partie terminée avec " << std::setprecision(2)
            << o->world->getCleanedEnergyRatio() << "% de particules détruites.";
         fl_alert("%s", ss.str().c_str());
         MainWindow::erase_dp(o->parent()->window(), o->parent()->window());
      }
   }
   Fl::repeat_timeout(refreshCycle, perform, userdata);
}

void WorldWidget::draw() {
   if (canvas == nullptr || scale == nullptr) return;

   fl_push_clip(x(), y(), w(), h());
   canvas->draw(this, scale);

   for (Particule* item: world->particules) {
      item->draw(this);
   }

   for (const auto& item: world->robots) {
      item->draw(this);
   }
   fl_pop_clip();
}

void WorldWidget::refreshView(void* userdata) {
   auto* o = (WorldWidget*) userdata;
   o->updateTimeLabel();
   o->redraw();

   Fl::repeat_timeout(refreshCycle, refreshView, userdata);
}

void WorldWidget::parseFile(const std::string& fileName) {

   std::ifstream file(fileName);
   if (!file.is_open()) {
      throw std::invalid_argument("Oops file not found.");
   }

   std::string line;
   int nbLine = 0;

   while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::vector<int> args;
      int num;
      while (ss >> num) {
         args.push_back(num);
      }

      switch (nbLine) {
         case 0://get scale
            if (args.size() != 4) {
               file.close();
               throw std::invalid_argument(
                     "Error: wrong number of arguments for scale");
            }
            delete scale;
            scale = new Scale(args[0], args[1], args[2], args[3]);
            break;
         case 1://get canvas
            if (args.size() < 8 || args.size() > 9) {
               file.close();
               throw std::invalid_argument(
                     "Error: wrong number of arguments for canvas");
            }
            delete canvas;
            canvas = new Canvas(args[0], args[1], args[2], args[3],
                                args[4],
                                args[5],
                                args[6],
                                args[7],
                                args.size() == 9 ? colors[args[8]] : FL_WHITE);
            break;
         case 2://get constraint
            if (args.size() != 4) {
               file.close();
               throw std::invalid_argument(
                     "Error: wrong number of arguments for constraints");
            }
            world->constraint = {args[0], args[1], args[2], args[3]};
            break;
         default://get objects
            switch (args[0]) {
               case 0: {
                  world->addRobot(new Robot(
                        {double(args[1]), double(args[2])},
                        args[3], args[4],
                        args[5], args[6]
                  ));
               }
                  break;
               case 1: {
                  if (args[4] < 0 || args[4] > MAX_COLOR - 1)
                     throw std::invalid_argument("Invalid color");

                  world->addParticule(new Particule(
                        {double(args[1]), double(args[2])},
                        args[3],
                        colors[args[4]],
                        args[5],
                        args[6],
                        canvas->TMAXE(),
                        scale->time(),
                        Particule::getBaseEnergy(args[3], scale->distance())
                  ));
               }
                  break;
               default:
                  std::cout << "Oops this object id doesnt exist" << std::endl;
            }
            break;
      }
      ++nbLine;
   }
   file.close();
   size(canvas->w(scale->distance()), canvas->h(scale->distance()));

   // Validate spacial constraints
   if (Validator::itemsOverlapping(world->getAllObjects())) {
      throw std::invalid_argument("Overlapping objects");
   }

   world->ready();
   // TODO: Change cycles values
   Fl::add_timeout(refreshCycle, perform, (void*) this);
   Fl::add_timeout(refreshCycle, refreshView, (void*) this);
}

void WorldWidget::setPaused(bool paused) {
   simulationPaused = paused;
}

bool WorldWidget::getPaused() const {
   return simulationPaused;
}

void WorldWidget::updateTimeLabel() {
   std::ostringstream os;
   os << "Timer: " << std::setprecision(2) << std::fixed << world->simulationTime;
   timeLabel = os.str();
   parent()->label(timeLabel.c_str());

   if (!getPaused()) {
      MainWindow::updateScoreLabel(parent()->window(), parent()->window(),(int) world->getCleanedEnergyRatio());
   }
}