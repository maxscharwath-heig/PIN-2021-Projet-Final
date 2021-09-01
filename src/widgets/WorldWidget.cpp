#include "WorldWidget.h"
#include "Fl_Center_Scroll.h"
#include <FL/fl_draw.H>
#include <iostream>
#include <fstream>
#include <FL/Fl.H>
#include <iomanip>
#include <chrono>

double WorldWidget::refreshCycle = 1. / 30.; // 30FPS
double WorldWidget::timeMultiplier = 1.;

WorldWidget::WorldWidget(int x, int y, int w, int h) : Fl_Widget(x, y, w, h, "") {
   world = new World();
   canvas = nullptr;
   scale = nullptr;
   this->lastTickTime = std::chrono::high_resolution_clock::now();
   Fl::add_timeout(refreshCycle, tick, (void*) this);
}

void WorldWidget::tick(void* userdata) {
   auto* o = (WorldWidget*) userdata;
   auto now = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = now - o->lastTickTime;
   o->lastTickTime = now;
   double timeCorrector = elapsed.count() / refreshCycle;

   if (!o->simulationPaused) {
      double deltaTime = refreshCycle * timeMultiplier * timeCorrector;
      o->world->update(o,deltaTime);
   }
   o->redraw();
   Fl::repeat_timeout(refreshCycle, tick, userdata);
}

void WorldWidget::draw() {
   if (canvas == nullptr || scale == nullptr) return;

   fl_push_clip(x(), y(), w(), h());
   canvas->draw(this, canvas, scale);

   for (const auto& item: world->robots) {
      item->draw(this);
   }

   for (Particule* item: world->particules) {
      item->draw(this);
   }
   fl_pop_clip();

   fl_color(FL_BLACK);
   std::ostringstream os;
   os << "Timer: " << std::setprecision(2) << std::fixed << world->simulationTime;
   fl_draw(os.str().data(), x() + 5, y() + 15);
}

void WorldWidget::parseFile(const std::string& fileName) {
   // TODO: Refactor

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
                        world->simulationTime,
                        canvas->TMAXE(),
                        1
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
   std::cout << "Parsing success for " << fileName << std::endl;
   file.close();
   size(
         int(canvas->w() * scale->getRatio()),
         int(canvas->h() * scale->getRatio())
   );
}

void WorldWidget::setPaused(bool paused) {
   simulationPaused = paused;
}

bool WorldWidget::getPaused() const {
   return simulationPaused;
}

