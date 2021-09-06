#ifndef PIN_2021_SERIE_1_SCALE_H
#define PIN_2021_SERIE_1_SCALE_H

class Scale {
public:
   Scale(unsigned xWorld, unsigned nPixels, unsigned tWorld, unsigned nRefresh);

   double time() const noexcept;

   double distance() const noexcept;

private:
   unsigned xWorld;
   unsigned nPixels;
   unsigned tWorld;
   unsigned nRefresh;
};


#endif //PIN_2021_SERIE_1_SCALE_H
