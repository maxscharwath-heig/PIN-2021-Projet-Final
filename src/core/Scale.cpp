#include "Scale.h"

Scale::Scale(unsigned int xWorld, unsigned int nPixels, unsigned tWorld,
             unsigned nRefresh) : xWorld(xWorld),
                                  nPixels(nPixels), tWorld(tWorld),
                                  nRefresh(nRefresh) {}

double Scale::getScaleRatio() const {
   return double(xWorld) / double(nPixels);
}

double Scale::getTimeRatio() const {
   return double(tWorld) / double(nRefresh);
}