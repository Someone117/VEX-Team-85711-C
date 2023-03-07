#include <cmath>
class Main_PID {
private:
  double e_cur;
  double e_prev = 0;
  double integral = 0;
  double derivative = 0;
  double kP;
  double kI;
  double kD;
  double dT;

public:
  // Constructor
  Main_PID(double e_cur, double kP, double kI, double kD, double dT) : e_cur(e_cur), kP(kP), kI(kI), kD(kD), dT(dT) {}

  void set_e_cur(double e){
    e_prev = e_cur;
    e_cur = e;
  }
  void sum_int(){
    integral += e_cur;
  }
  void find_deriv(){
    derivative = e_cur - e_prev; 
  }

  double output(){
    return ( (kP*e_cur) + (kI * integral * dT) + (kD*derivative/dT) ); //returns a value in terms of V
  }

  // Getter
  double get_e_cur(){
    return e_cur;
  }
};