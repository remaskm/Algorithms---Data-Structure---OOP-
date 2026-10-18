/*
Purpose: Ordered Integers using Single Linked List
Description:
- Implements a unique ordered linked list (OrderedLL) where node values are sorted in ascending order.
- Duplicate values are not allowed; instead, each node maintains a counter (ctr) to track occurrences.
- Provides functionalities for insertion, deletion, searching, and displaying values in both ascending and descending order.
*/
#include <iostream>
using namespace std;

class Node {
    public:
    int info;      
    Node *next;     
    int ctr;         


    Node() {
        next = NULL;
    }

    
    Node (int n, Node *p= NULL) {
        info = n; 
        next = p; 
        ctr = 1;  
    }
};

class OrderedLL {
    private:
    Node *head, *tail;  

    public:
    OrderedLL() {
        head = NULL;
        tail = NULL;
    }

    ~OrderedLL() {
        clear();
    }

    void clear();
    bool search(int item);
    bool isEmpty();
    void addToHead(int item);
    void deleteFromHead();
    void addInOrder(int item);
    void display();
    void generateFromArray(int arr[], int size);
    void removeMax();
    void deleteValue(int item);
    void displayDescending(Node* node);
};

void OrderedLL :: clear() {
    Node* temp;
    while (!isEmpty()) { 
        temp = head;
        head = head->next;
        delete temp; 
    }
    head = NULL;
    tail = NULL;
}

bool OrderedLL :: search(int value) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->info == value)
            return true; 
        temp = temp->next;
    }
    return false; 
}

bool OrderedLL :: isEmpty() {
    return head == NULL;
}

void OrderedLL :: addToHead(int item) {
    Node *n = new Node(item, head); 
    head = n; 

    if (tail == NULL) {
        tail = n;
    }
}

void OrderedLL :: deleteFromHead() {
    if (isEmpty()) 
        return;

    if (head->ctr > 1) {
        head->ctr--; 
        return;
    }

    Node* temp = head;
    head = head->next;

    if (head == NULL) {
        tail = NULL;
    }
    delete temp; 
}

void OrderedLL::addInOrder(int item) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->info == item) {
            temp->ctr++; 
            return;
        }
        temp = temp->next;
    }

    Node* n = new Node(item);

    if (head == NULL || item < head->info) {
        addToHead(item);
        return;
    }

    Node* prev = NULL;
    temp = head;
    while (temp != NULL && temp->info < item) {
        prev = temp;
        temp = temp->next;
    }

    n->next = temp;
    prev->next = n;

    if (temp == NULL) {
        tail = n;
    }
}

void OrderedLL :: generateFromArray(int arr[], int size) {
    clear();
    for(int i = 0; i < size; i++) {
        addInOrder(arr[i]); 
    }
}

void OrderedLL::display() {
    Node* temp = head;
    while (temp != NULL) {
        for (int i = 0; i < temp->ctr; i++) {
            cout << temp->info << " ";
        }
        temp = temp->next;
    }
    cout << endl;
}

void OrderedLL :: removeMax(){
    if (isEmpty())
        return;

    if (head == tail) { 
        delete head;
        head = tail = NULL;
        return;
    }

    Node* ptr = head;
    while (ptr->next != tail) {
        ptr = ptr->next;
    }

    delete tail;
    tail = ptr;
    tail->next = NULL;
}


void OrderedLL :: deleteValue(int item) {
    if (isEmpty()) {
        cout << "List is empty: There are no values to delete\n";
        return;
    }

    while (!search(item)) {
        cout << "Value not found! Please enter an existing value to delete: ";
        cin.clear();
        cin >> item;
    }

    if (head->info == item) {
        deleteFromHead();
        return;
    }

    Node* pred = head;
    while (pred->next != NULL && pred->next->info != item) {
        pred = pred->next;
    }

    if (pred->next != NULL) {
        Node* temp = pred->next; 

        if (temp->ctr > 1)
            temp->ctr--;

        else {
            pred->next = temp->next; 

            if (temp == tail) { 
                tail = pred;
                tail->next = NULL;
            }
            delete temp;
        }
    }
}

void OrderedLL::displayDescending(Node* node = NULL) {
    if (node == NULL) {

        if (isEmpty()) {
            cout << "The list is empty." << endl;
            return;
        }
        node = head;  
        }

    if (node->next != NULL) {
        displayDescending(node->next);  
        }

    for (int i = 0; i < node->ctr; i++) {
        cout << node->info << " ";
    }

    if (node == head) {
        cout << endl;
    }
}



int main() {
    cout << "\n ====================================\n";
    cout << "  Ordered Linked List Program\n";
    cout << "====================================\n";
    cout << "This program creates and manages an ordered linked list.\n";
    cout << "You can insert integers, remove elements, display the list in ascending \n";
    cout << "or descending order, and perform other operations interactively.\n\n";

    char generateAgain = 'y'; 

    while (generateAgain == 'y' || generateAgain == 'Y') {
        OrderedLL list; 
        int size;

        cout << "Enter the number of elements to add:";
        while (!(cin >> size) || size <= 0) {
            cout << "\nPlease enter a valid positive integer:";
            cin.clear(); 
            cin.ignore(); 
        }

        int *arr = new int[size];
        cout << "\nEnter elements:";

        for (int i = 0; i < size; i++) {
            while (!(cin >> arr[i])) {
                cout << "\nInvalid input. Please enter an integer:";
                cin.clear();
                cin.ignore();
            }
        }
        cin.ignore(); 

        list.generateFromArray(arr, size);
        delete[] arr; 
        cout << "\nList after ordered insertion:";
        list.display();

        int choice; 

        while (!list.isEmpty()) {
            cout << "\nMenu:\n1. Display Descending\n2. Delete Value\n3. Remove Max\n4. Clear List\nEnter your choice:";

            while (!(cin >> choice)) {
                cout << "\nInvalid input. Please enter a number between 1 and 4:";
                cin.clear();
                cin.ignore();
            }

            switch (choice) {
                case 1:
                    cout << "\nDescending order:";
                    list.displayDescending();
                    break;
                case 2: {
                    int val;
                    cout << "\nEnter value to delete:";

                    while (!(cin >> val) || !list.search(val)) {
                        cout << "\nInvalid input or value not found. Please enter an existing value to delete:";
                        cin.clear();
                        cin.ignore();
                    }

                    list.deleteValue(val);
                    cout << "\nValue " << val << " deleted successfully.";
                    list.display();
                    break;
                }
                case 3:
                    list.removeMax();
                    cout << "\nCurrent list:";
                    list.display();
                    break;
                case 4:
                    list.clear();
                    break;
                default:
                    cout << "\nInvalid choice. Please select from the menu.";
                    break;
            }

            if (list.isEmpty()) {
                cout << "\nAll values have been deleted or removed.";
                break;
            }
        }

        cout << "\nDo you want to generate another linked list? (y/n):";
        cin >> generateAgain;
        cin.ignore();
    }

    cout << "\nProgram ended.";
    return 0;
}

