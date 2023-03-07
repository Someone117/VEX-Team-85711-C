#include "Math_Vector.h"

Comp_Vector operator+(const Comp_Vector &a, const Comp_Vector &b) {
  return Comp_Vector(a.x + b.x, a.y + b.y);
}