#include "vex.h"

float reduce_0_to_360(float angle) {
  while(!(angle >= 0 && angle < 360)) {
    if( angle < 0 ) { angle += 360; }
    if(angle >= 360) { angle -= 360; }
  }
  return(angle);
}

float reduce_negative_180_to_180(float angle) {
  while(!(angle >= -180 && angle < 180)) {
    if( angle < -180 ) { angle += 360; }
    if( angle >= 180 ) { angle -= 360; }
  }
  return(angle);
}

float reduce_negative_90_to_90(float angle) {
  while(!(angle >= -90 && angle < 90)) {
    if( angle < -90 ) { angle += 180; }
    if( angle >= 90 ) { angle -= 180; }
  }
  return(angle);
}

int scurve(double speed) { 
  return ((12 * pow(fabs(speed), M_E)) / (pow(100, M_E))) * (speed / fabs(speed)); 
}