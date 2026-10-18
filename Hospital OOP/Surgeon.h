/* 
Surgeon class
Description:
   -Inherits from Doctor and specializes in surgeons
   -Manages multiple scheduled surgeries
   -Calculates fees based on the number of 60-minute intervals spent with patient
   -Displays surgery details
*/

#ifndef SURGEON_H
#define SURGEON_H

#include "Doctor.h"
#include "SurgerySchedules.h"


class Surgeon: public Doctor {

private:
    int numSurgeriesScheduled; 
    SurgerySchedules *schedule; 

public:

   
    Surgeon() {
        numSurgeriesScheduled = 0;
        schedule = new SurgerySchedules();
    }

    
    Surgeon(string id, string name, double duration, double rate, int numSurgeries)
      : Doctor(id, name, duration, rate), numSurgeriesScheduled(numSurgeries) {
        schedule = new SurgerySchedules[numSurgeries]; 
    }

   
    Surgeon(const Surgeon& surgeon): Doctor(surgeon), numSurgeriesScheduled(surgeon.numSurgeriesScheduled) {
        schedule = new SurgerySchedules[numSurgeriesScheduled]; 
        for (int i = 0; i < numSurgeriesScheduled; ++i) {
            schedule[i] = surgeon.schedule[i]; 
        }
    }

    
    double calculateTotalFee() {
        return (duration/60)*rate;
    }

    
    string prettyPrint() {
        return "Dr. "+name+" is a surgeon whose total fee is "+to_string(calculateTotalFee())+" with total number of surgeries "+to_string(getNumofSurgeriesScheduled()); 
    }

    
    string printSurgeries() {
        string output = ""; 
        for (int i = 0; i < numSurgeriesScheduled; i++) {
            output += schedule[i].printSurgerySchedules(); 
        }
        return output;
    }

    
    void setSurgeryDetails(int index, string patientName, string date) {
        if (index >= 0 && index < numSurgeriesScheduled) {
            schedule[index].setPatientName(patientName); 
            schedule[index].setDate(date); 
        }
    }

    
    ~Surgeon(){
        delete []schedule;
    }

    int getNumofSurgeriesScheduled() {
        return numSurgeriesScheduled;
    }


    
    friend ostream & operator<<(ostream &out, const Surgeon &surgeon) {
        out << "Dr. " << surgeon.name << " Surgeries\n";
        for (int i = 0; i < surgeon.numSurgeriesScheduled; i++) {
            out << surgeon.schedule[i].printSurgerySchedules(); 
        }
        return out; 
    }

};




#endif 
