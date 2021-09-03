#include "Scale.h"

Scale::Scale(unsigned int xWorld, unsigned int nPixels, unsigned tWorld,
             unsigned nRefresh) : xWorld(xWorld),
                                  nPixels(nPixels), tWorld(tWorld),
                                  nRefresh(nRefresh) {}

double Scale::time() const {
   return double(tWorld) / double(nRefresh);
}

double Scale::distance() const {
   return double(nPixels) / double(xWorld);
}
