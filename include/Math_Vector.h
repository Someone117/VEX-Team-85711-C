#include <cmath>
#include <string>
#include <sstream>

class Comp_Vector{
    private:
        double x, y, angle, mag;

    public:
        //Constructor
        Comp_Vector(double x, double y) : x(x), y(y){
            angle = atan2(y, x);
            mag = hypot(x, y); 
        }

        //Base methods
        void rotate(double rotation){
            set_angle(angle + rotation);
        }

        //Setters
        void set_x(double x1){
            x = x1;
            angle = atan2(y, x);
            mag = hypot(x, y); 
        }
        void set_y(double y1){
            y = y1;
            angle = atan2(y, x);
            mag = hypot(x, y); 
        }
        void set_angle(double angle1){
            angle = angle1;
            x = mag*cos(angle);
            y = mag*sin(angle);
        }
        void set_mag(double mag1){
            mag = mag1; 
            x = mag*cos(angle);
            y = mag*sin(angle);
        }

        //Getters
        double get_x(){
            return x;
        }
        double get_y(){
            return y;
        }
        double get_angle(){
            return angle;
        }
        double get_mag(){
            return mag; 
        }
        std::basic_string<char> print() {
          std::stringstream ss;
          ss << x << " " << y;
          return ss.str();
        }
};

class Mag_Vector : public Comp_Vector{

    public:
        //Constructor
        Mag_Vector(double mag, double angle) : Comp_Vector(mag*cos(angle), mag*sin(angle)){}
};