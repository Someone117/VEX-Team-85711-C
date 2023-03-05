class Main_PID {
private:
  double e_cur;
  double e_prev = 0;
  double integral = 0;
  double derivative = 0;
  double kP = 1;
  double kI = 1;
  double kD = 1;

public:
  // Constructor
  Main_PID(double e_cur) : e_cur(e_cur) {}

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
    return ( (kP*e_cur) + (kI*integral) + (kD*derivative) ); //returns a value in terms of V
  }

  // Getter
  double get_e_cur(){
    return e_cur;
  }
};