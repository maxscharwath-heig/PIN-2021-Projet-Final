//
// Created by Maxime on 04.09.2021.
//

#ifndef PIN_2021_SERIE_2_UTILS_H
#define PIN_2021_SERIE_2_UTILS_H

inline double to_rad(double deg) {
   return deg * M_PI / 180.0;
}

inline double to_deg(double rad) {
   return rad / M_PI * 180.0;
}

inline double norm_angle(double x){
   x = fmod(x + 180,360);
   if (x < 0)
      x += 360;
   return x - 180;
}

#endif //PIN_2021_SERIE_2_UTILS_H
