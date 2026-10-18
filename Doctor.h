/*
Doctor class
Description:
   -Base class representing general information about doctors' name, ID, consultation duration, and hourly rate.
   -Provides virtual functions for calculating the total fee 
   -Formatting output for display
*/

#ifndef DOCTOR_H
#define DOCTOR_H


#include <iostream>
using namespace std;
#include <fstream>




class Doctor {

    
protected:
    string id;
    string name;
    double duration;
    double rate; 

public:

    
    Doctor() {
        name = "";
        id = "0";
        duration = 0;
        rate = 0;
    }

    
    Doctor(string name, string id, double duration, double rate) {
        this->name = name;
        this->id = id;
        this->duration = duration;
        this->rate = rate;
    }

    
    virtual double calculateTotalFee(){ return 0.0; }

    
    virtual string prettyPrint() {

        return "Dr Id is "+ id + ", " + "and name is Dr. "+name;
    }


};





#endif 
