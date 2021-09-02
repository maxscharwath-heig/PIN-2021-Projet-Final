#ifndef PIN_2021_SERIE_1_SCALE_H
#define PIN_2021_SERIE_1_SCALE_H

class Scale {
public:
   Scale(unsigned xWorld, unsigned nPixels, unsigned tWorld, unsigned nRefresh);

   unsigned getXWorld() const;

   unsigned getNPixels() const;

   double getScaleRatio() const;

private:
   unsigned xWorld;
   unsigned nPixels;
   unsigned tWorld;
   unsigned nRefresh;

   double getTimeRatio() const;
};


#endif //PIN_2021_SERIE_1_SCALE_H
