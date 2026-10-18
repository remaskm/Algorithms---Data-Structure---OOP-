/* 
Specialist class
Description:
   -Inherits from Doctor and represents specialists in various fields
   -Includes an additional property for the specialization
   -Calculates fees based on the number of 15-minute intervals spent with patient
*/

#ifndef SPECIALIST_H
#define SPECIALIST_H

#include "Doctor.h"


class Specialist: public Doctor {
private:
    string speciality; 

public:

    
    Specialist() {
        speciality = "";
    }

    
    Specialist(string name, string id, double duration, double rate,string speciality) : Doctor (name, id, duration, rate) {
        this->speciality = speciality;
    }

    
    double calculateTotalFee() {
        return (duration/15)*rate;
    }

    
    string prettyPrint() {
        return "Dr. "+ name + " is a specialist whose total fee is " + to_string(calculateTotalFee()); 
    }

};



#endif 
