#include "Scale.h"

Scale::Scale(unsigned int xWorld, unsigned int nPixels, unsigned tWorld,
             unsigned nRefresh) : xWorld(xWorld),
                                  nPixels(nPixels), tWorld(tWorld),
                                  nRefresh(nRefresh) {}

unsigned int Scale::getXWorld() const {
   return xWorld;
}

unsigned int Scale::getNPixels() const {
   return nPixels;
}

double Scale::getRatio() const {
   return double(xWorld) / double(nPixels);
}
