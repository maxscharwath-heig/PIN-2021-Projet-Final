#include "Scale.h"

Scale::Scale(unsigned int xWorld, unsigned int nPixels, unsigned tWorld,
             unsigned nRefresh) : xWorld(xWorld),
                                  nPixels(nPixels), tWorld(tWorld),
                                  nRefresh(nRefresh) {}

double Scale::time() const noexcept{
   return nRefresh ? double(tWorld) / double(nRefresh) : 0;
}

double Scale::distance() const noexcept{
   return xWorld ? double(nPixels) / double(xWorld) : 0;
}
