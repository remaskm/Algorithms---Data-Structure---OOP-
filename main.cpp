/*
Purpose: A hospital management system to manage Doctors (Surgeons and Specialists), their fees, and surgeries.
Description:
- Manages details of Doctors such as their ID, name, duration spent with patients, rate, and specialization or surgeries.
- Prints formatted details to the screen and a file.
- Uses object-oriented principles with proper inheritance and assossiation.
*/

#include "Doctor.h"
#include "SurgerySchedules.h"
#include "Surgeon.h"
#include "Specialist.h"


int main() {

    int numDoctors; 
    int docChoice; 


    
    
    ofstream doctorsFile("C:\\Users\\samaa\\OneDrive\\Desktop\\Assign2_20235014_20235057\\doctors.txt");  

    
    if (!doctorsFile) {
        cout << "Failed to open the file!" << endl;
        return -1; 
    }

    cout << "** Hospital System ** \n \n";

    cout << "How many doctors are in the hospital \n";
    cin >> numDoctors;

    Doctor* doctorsArr[numDoctors]; 


    for (int i = 0; i < numDoctors; i++) {

        do {

            cout << "Is doctor \n 1-A surgeon? \n 2-A specialist? \n";
            cin >> docChoice;

            if (docChoice != 1 && docChoice != 2) {
                cout << "Invalid input. Please try again." << endl;
            }

        }while(docChoice != 1 && docChoice != 2);

        
        if (docChoice == 1) {

            
            string name;
            string id;
            double duration;
            double rate;
            int numOfSurgeries;

            
            cout << "Enter surgeon's name \n";
            cin >> name;
            cout << "Enter surgeon's ID\n";
            cin >> id;
            cout << "Enter total number of minutes surgeon spent with patients \n";
            cin >> duration;
            cout << "Enter surgeon's hourly paying rate \n";
            cin >> rate;
            cout << "Enter number of surgeries surgeon will be performing: \n";
            cin >> numOfSurgeries;

            
            Surgeon* surgeon = new Surgeon(name, id, duration, rate, numOfSurgeries);
            doctorsArr[i] = surgeon; 

            
            for (int j = 0; j < numOfSurgeries; j++) {
                string patientName, date;
                cout << "Enter patient name for surgery " << j + 1 << ": \n";
                cin >> patientName;
                cout << "Enter date for surgery (dd/mm/yyyy) " << j + 1 << ": \n";
                cin >> date;

                
                
                surgeon->setSurgeryDetails(j, patientName, date);

            }
        }

        
        else if (docChoice == 2) {

            
            string name;
            string id;
            double duration;
            double rate;
            string speciality;

            
            cout << "Enter specialist's name \n";
            cin >> name;
            cout << "Enter specialist's speciality \n";
            cin >> speciality;
            cout << "Enter specialist's ID\n";
            cin >> id;
            cout << "Enter total number of minutes specialist spent with patients\n";
            cin >> duration;
            cout << "Enter specialist's hourly paying rate \n";
            cin >> rate;

            
            doctorsArr[i] = new Specialist(name, id, duration, rate, speciality);
        }
        else{
            cout<<"Wrong input!"<<endl;
        }
    }

    
    for (int i = 0; i < numDoctors; i++) {
        doctorsFile << doctorsArr[i]->prettyPrint() << endl; 
        cout << doctorsArr[i]->prettyPrint() << endl; 

        
        Surgeon* surgeon = dynamic_cast<Surgeon*>(doctorsArr[i]); 
        if (surgeon) { 
            doctorsFile <<"Dr's surgeries: \n"<< surgeon->printSurgeries() << endl; 
            cout <<"Dr's surgeries: \n"<<surgeon->printSurgeries() << endl; 
        }
    }

    
    for (int i = 0; i < numDoctors; i++) {
        delete doctorsArr[i];
    }

    
    doctorsFile.close();

    return 0;
}
