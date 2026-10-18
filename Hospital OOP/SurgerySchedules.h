/* 
SurgerySchedules class 
Description:
   -Manages details of surgeries including the patient's name and the scheduled surgery date.
   -Provides methods to set and retrieve these details 
   -Format the output for surgery schedules. 
*/

#ifndef SURGERYSCHEDULES_H
#define SURGERYSCHEDULES_H



class SurgerySchedules {

private:
    string patientName;
    string date;

public:

    
    SurgerySchedules() {
        patientName = "";
        date = "";
    }

    
    SurgerySchedules(string patientName, string date) {
        this->patientName = patientName;
        this->date = date;
    }

    
    void setPatientName(string patientName) {
        this->patientName = patientName;
    }

    
    void setDate(string date) {
        this->date = date;
    }

    
    string getPatientName() {
        return this->patientName;
    }

    
    string getDate() {
        return this->date;
    }

    
    string printSurgerySchedules() {
        return "PatientName: "+this->patientName+"\nSurgery date: "+this->date+"\n";
    }
};




#endif 
