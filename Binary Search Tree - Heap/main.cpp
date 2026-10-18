#include <fstream>
#include <filesystem>
#include "heap.h"

using namespace std;


void displayMenu() {
    cout << "\n====== FCAI Task Manager ======" << endl;
    cout << "1. Insert a task" << endl;
    cout << "2. Display all" << endl;
    cout << "3. Search for a task" << endl;
    cout << "4. Remove a task" << endl;
    cout << "5. Display more than" << endl;
    cout << "6. Display less than" << endl;
    cout << "7. Mark a task as completed by task duration and description" << endl;
    cout << "8. Display all completed tasks and the number of tasks completed per category" << endl;
    cout << "9. Exit" << endl;
    cout << "===============================" << endl;
    cout << "Enter number of option: ";
}


int main() {
    
    TaskManagerBST taskTree;     
    MinHeap completedTasks(100); 

    
    string filepath;
    cout << "Please enter the file path (interchange each single backslash with double backslashes): ";
    getline(cin, filepath);

    
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "\nERROR......Unable to open the file: '" << filepath << "'............\n";
        if (!filesystem::exists(filepath)) {
            cout << "→ Check that the given file exists at the specified path.\n";
        } else {
            cout << "→ The file exists but couldn't be opened.\n";
            cout << " • Check it isn't locked, protected, or inaccessible.\n";
        }
        cout << "try again.\n";
        return 1;
    }

    cout << "File opened successfully. Processing...\n";

    
    int numTasks;
    file >> numTasks;
    file.ignore(); 

    cout << "Reading " << numTasks << " tasks from file...\n";

    
    for (int i = 0; i < numTasks; i++) {
        string description, category;
        int duration;

        
        getline(file, description);

        string durationStr;
        getline(file, durationStr);
        try {
            duration = stoi(durationStr);
        } catch (const exception& e) {
            cout << "Error parsing duration for task " << i+1 << ": " << durationStr << endl;
            break;
        }

        getline(file, category);
        cout << "Read task " << (i+1) << ": [" << description << ", " << duration << ", " << category << "]" << endl;

        
        taskTree.addTaskToTree(description, duration, category);
    }

    cout << "All tasks loaded into Task Manager.\n";

    
    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1: { 
                
                string description, category;
                int duration;

                cout << "Enter tasks description: ";
                getline(cin, description);

                cout << "Enter duration: ";
                cin >> duration;
                cin.ignore();

                cout << "Enter Category: ";
                getline(cin, category);

                taskTree.addTaskToTree(description, duration, category);
                cout << "The task is added." << endl;
                break;
            }

            case 2: { 
                cout << "\n===== All Tasks =====\n" << endl;
                taskTree.showAllTasksSorted();
                break;
            }

            case 3: { 
                int duration;
                cout << "Enter the duration: ";
                cin >> duration;
                cin.ignore();

                taskTree.findTasksWithExactDuration(duration);
                break;
            }

            case 4: { 
                int duration;
                cout << "Enter the duration: ";
                cin >> duration;
                cin.ignore();

                taskTree.deleteTasksWithExactDuration(duration);
                break;
            }

            case 5: { 
                int duration;
                cout << "Enter the duration more than: ";
                cin >> duration;
                cin.ignore();

                taskTree.showTasksWithDurationAtLeast(duration);
                break;
            }

            case 6: { 
                int duration;
                cout << "Enter the duration less than: ";
                cin >> duration;
                cin.ignore();

                taskTree.showTasksWithDurationAtMost(duration);
                break;
            }

            case 7: { 
                int duration;
                string description;

                cout << "Task duration: ";
                cin >> duration;
                cin.ignore();

                cout << "Task description: ";
                getline(cin, description);

                
                Task completedTask = completedTasks.markAsCompleted(taskTree, duration, description);

                
                if (completedTask.getDuration() > 0) {
                    cout << "Task marked as completed:" << endl;
                    completedTask.display();

                    
                    completedTasks.insert(completedTask);
                } else {
                    cout << "No matching task found with duration " << duration
                         << " and description containing '" << description << "'" << endl;
                }
                break;
            }

            case 8: { 
                cout << "\n===== Completed Tasks =====\n" << endl;
                completedTasks.displayAll();
                completedTasks.displayCategoryReport();
                break;
            }

            case 9: { 
                cout << "Exiting Task Manager." << endl;
                running = false;
                break;
            }

            default: { 
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
    }

    return 0;
}
