#include <FL/Fl.H>
#include "widgets/MainWindow.h"

int main(int argc, char** argv) {
   MainWindow mainWindow;
   mainWindow.show();
   return Fl::run();
}
