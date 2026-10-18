
#include <iostream>
using namespace std;

class Node {
public:

    char data;
    Node* next;
    Node* prev;

    
    Node() {
        data = '\0';
        next = nullptr;
        prev = nullptr;
    }

    
    Node(char data) {
        this->data = data;
        next = nullptr;
        prev = nullptr;
    }

};

class DoublyLinkedList {

    Node* head;
    Node* tail;

public:

    
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    
    DoublyLinkedList(Node* head, Node* tail) {
        this->head = head;
        this->tail = tail;
    }

    
    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
    }

    
    void setHead(Node* newHead) {
        
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        
        head = newHead;

        
        tail = head;
        while (tail && tail->next) 
            tail = tail->next;
    }

    
    void setTail(Node* newTail) {
        tail = newTail;
    }

    
    Node *&getHead() {
        return head;
    }

    
    Node *&getTail() {
        return tail;
    }

    Node* listIntialiser(const string& str) {
        
        if (str.empty()) {
            return nullptr;
        }

        
        Node* newHead = new Node(str[0]);
        Node* currentTail = newHead;

        
        for (int i = 1; i < str.length(); i++) {
            Node* newNode = new Node(str[i]); 
            currentTail->next = newNode; 
            newNode->prev = currentTail; 
            currentTail = newNode; 
        }

        
        return newHead;
    }

    void insertAtMiddle(Node*& head, Node*& tail, int position, char value) {
        
        if (position == 1) {
            Node* newNode = new Node(value);
            newNode->next = head;  

            if (head!=nullptr) {
                head->prev = newNode; 
            }

            head = newNode; 

            
            if (tail == nullptr) {
                tail = head;
            }

            return;
        }

        Node* temp = head;

        
        for (int i = 1; i < position - 1 && temp != nullptr; i++) {
            temp = temp->next;
        }

        
        if (temp == nullptr) {
            cout << "Position out of range!" << endl;
            return;
        }

        
        Node* newNode = new Node(value);

        
        newNode->next = temp->next;
        newNode->prev = temp; 

        
        if (newNode->next != nullptr) {
            newNode->next->prev = newNode;
        }

        else tail = newNode;

        
        temp->next = newNode;
    }

void insertAfterLetter(Node*& head, Node*& tail, char target, char value, bool insertAfter) {
    if (head == nullptr) {
        cout << "List is empty!\n";
        return;
    }

    Node* temp = head;
    Node* targetNode = nullptr;
    int occurrenceCount = 0;
    int index = 1;

    
    while (temp != nullptr) {
        if (temp->data == target) {
            occurrenceCount++;
            if (occurrenceCount == 1) targetNode = temp;
            cout << occurrenceCount << ". '" << target << "' found at position " << index << endl;
        }
        temp = temp->next;
        index++;
    }

    
    if (occurrenceCount == 0) {
        cout << "Letter '" << target << "' not found in the list!\n";
        return;
    }

    
    if (occurrenceCount > 1) {
        int choice;
        cout << "Choose which occurrence to modify (1-" << occurrenceCount << "): ";
        cin >> choice;

        if (choice < 1 || choice > occurrenceCount) {
            cout << "Invalid choice!\n";
            return;
        }

        
        temp = head;
        int currentOccurrence = 0;
        while (temp != nullptr) {
            if (temp->data == target) {
                currentOccurrence++;
                if (currentOccurrence == choice) {
                    targetNode = temp;
                    break;
                }
            }
            temp = temp->next;
        }
    }

    
    Node* newNode = new Node(value);

    if (insertAfter) {
        
        newNode->next = targetNode->next;
        newNode->prev = targetNode;

        if (targetNode->next)
            targetNode->next->prev = newNode;
        else
            tail = newNode; 

        targetNode->next = newNode;
    } else {
        
        newNode->prev = targetNode->prev;
        newNode->next = targetNode;

        if (targetNode->prev)
            targetNode->prev->next = newNode;
        else
            head = newNode; 

        targetNode->prev = newNode;
    }
}
    void printList() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data;
            temp = temp->next;
        }
    }

    void concat(const DoublyLinkedList& obj) {
        
        if (obj.head == nullptr) return;

        
        if (head == nullptr) {
            Node* current = obj.head; 

            while (current) {
                
                Node* newNode = new Node(current->data);

                
                if (!head) {
                    head = tail = newNode;
                }

                current = current->next; 
            }
            return;
        }

        
        Node* objCurrent = obj.head; 

        while (objCurrent) {
            
            Node* newNode = new Node(objCurrent->data);

            
            tail->next = newNode; 
            newNode->prev = tail; 
            tail = newNode;       

            objCurrent = objCurrent->next; 
        }
    }

    void deleteAtPosition(Node*& head, Node*& tail, int position) {
        if (head == nullptr) {
            cout << "List is empty! Nothing to delete.\n";
            return;
        }

        Node* temp = head;
        int i = 1;

        
        while (temp != nullptr && i < position) {
            temp = temp->next;
            i++;
        }

        
        if (temp == nullptr) {
            cout << "Invalid position! No character at position " << position << ".\n";
            return;
        }

        
        if (temp == head) {
            head = temp->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
        }
        
        else if (temp == tail) {
            tail = temp->prev;
            tail->next = nullptr;
        }
        
        else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev; 
        }

        cout << "Deleted character: " << temp->data << " at position " << position << ".\n";
        delete temp;
    }

   void deleteAroundLetter(Node*& head, Node*& tail, char target, bool deleteAfter) {
    if (head == nullptr) {
        cout << "List is empty!\n";
        return;
    }

    Node* temp = head;
    int index = 1, foundIndex = -1;
    int occurrenceCount = 0;

    
    while (temp != nullptr) {
        if (temp->data == target) {
            occurrenceCount++;
            if (foundIndex != -1) cout << "\n"; 
            cout << occurrenceCount << ". '" << target << "' found at position " << index;
            foundIndex = index;
        }
        temp = temp->next;
        index++;
    }

    
    if (foundIndex == -1) {
        cout << "Letter '" << target << "' not found in the list!\n";
        return;
    }

    
    int choice = 1;
    if (occurrenceCount > 1) {
        cout << "\nChoose which occurrence to modify: ";
        cin >> choice;
    }

    
    temp = head;
    index = 1;
    int occurrencesFound = 0;

    while (temp != nullptr) {
        if (temp->data == target) {
            occurrencesFound++;  

            
            if (occurrencesFound == choice) {
                break;
            }
        }
        temp = temp->next;
        index++;
    }

    
    Node* nodeToDelete = nullptr;

    if (deleteAfter) {
        
        nodeToDelete = temp ? temp->next : nullptr;
    } else {
        
        nodeToDelete = temp ? temp->prev : nullptr;
    }

    
    if (nodeToDelete == nullptr) {
        cout << "No character to delete " << (deleteAfter ? "after" : "before") << " '" << target << "'.\n";
        return;
    }

    
    char deletedChar = nodeToDelete->data;

    
    if (nodeToDelete == head) {
        head = nodeToDelete->next;
        if (head) head->prev = nullptr;
    }

    
    if (nodeToDelete == tail) {
        tail = nodeToDelete->prev;
        if (tail) tail->next = nullptr;
    }

    
    if (nodeToDelete->prev) {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    if (nodeToDelete->next) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }

    
    string position = deleteAfter ? "after" : "before";

    
    cout << "Deleted character: '" << deletedChar << "' " << position << " '" << target << "'.\n";

    
    delete nodeToDelete;
}
    DoublyLinkedList substring(int start, int length) {
        DoublyLinkedList subList; 

        
        if (start < 1 || length <= 0 || head == nullptr) {
            return subList; 
        }

        Node* temp = head;

        
        for (int i = 1; i < start && temp != nullptr; i++) {
            temp = temp->next;
        }

        
        if (temp == nullptr) {
            return subList;
        }

        
        int position = 1; 
        for (int i = 0; i < length && temp != nullptr; i++) {
            subList.insertAtMiddle(subList.head, subList.tail, position++, temp->data);
            temp = temp->next;
        }

        return subList; 
    }

    int searchSubstring(Node* head, const string& substring) {
        if (head==nullptr || substring.empty()) return -1; 

        Node* temp = head; 
        int position = 1;  

        while (temp) {
            Node* current = temp; 
            int index = 0;        

            
            while (current && index < substring.length() && current->data == substring[index]) {
                current = current->next;
                index++;
            }

            
            if (index == substring.length()) {
                return position;
            }

            temp = temp->next; 
            position++;
        }

        return -1; 
    }

    void replaceSubstring(Node*& head, const string& target, const string& replacement) {
        
        if (!head || target.empty()) return;

        Node* current = head;  
        Node* prev = nullptr;  

        
        while (current) {
            Node* start = current;  
            Node* check = current;  
            int i = 0;  

            
            while (check && i < target.length() && check->data == target[i]) {
                check = check->next;  
                i++;
            }

            
            if (i == target.length()) {
                
                Node* toDelete = start;
                for (int j = 0; j < target.length(); j++) {
                    Node* nextNode = toDelete->next;  
                    delete toDelete;  
                    toDelete = nextNode;  
                }

                
                Node* newStart = nullptr;
                Node* newEnd = nullptr;

                for (char c : replacement) {
                    Node* newNode = new Node(c);  
                    if (!newStart) {
                        newStart = newEnd = newNode;  
                    } else {
                        newEnd->next = newNode;  
                        newEnd = newNode;  
                    }
                }

                
                if (prev) {
                    prev->next = newStart;  
                } else {
                    head = newStart;  
                }

                if (newEnd) {
                    newEnd->next = check;  
                }

                if (newEnd) {
                    
                    current = newEnd->next;
                } else {
                    
                    current = check;
                }

                
                if (!replacement.empty()) {
                    prev = newEnd;
                }
            }   else {
                
                prev = current;
                current = current->next;
            }
        }
    }
};
int main() {

    cout<<"\n***********************************************\n";
    cout<<"********* String Manipulation Program *********\n";
    cout<<"**** Implemented using Doubly linked lists ****\n";
    cout<<"***********************************************\n\n";

    char version;

    do {
        cout << "Choose which (main function) version you want to access\n A) Assignment's expected output\n B) Best Practice version\n";
        cout << "Enter choice: ";
        cin >> version;

        if (version == 'A' || version == 'a') {
            
            DoublyLinkedList list1, list2;
            string input1, input2;

            
            cout << "Enter string to add to list 1: ";
            cin >> input1;
            list1.setHead(list1.listIntialiser(input1));

            
            cout << "Enter string to add to list 2: ";
            cin >> input2;
            list2.setHead(list2.listIntialiser(input2));

            
            list1.concat(list2);
            cout << "Concatenated Lists: ";
            list1.printList();
            cout<<endl;

            
            int index;
            cout << "Choose a character by index to remove: ";
            cin >> index;
            list1.deleteAtPosition(list1.getHead(), list1.getTail(), index);
            cout << "List after removal: ";
            list1.printList();
            cout<<endl;

            
            int start, length;
            cout << "Enter index and length to get substring: ";
            cin >> start >> length;
            DoublyLinkedList substring = list1.substring(start, length);
            cout << "Substring: ";
            substring.printList();
            cout<<endl;

            
            string searchStr;
            cout << "Search for a string in the list: ";
            cin >> searchStr;
            int foundIndex = list1.searchSubstring(list1.getHead(), searchStr);
            cout << "Found at index: " << foundIndex << endl;
            cout<<endl;

            
            string target, replacement;
            cout << "Enter 2 substrings to replace one with another: ";
            cin >> target >> replacement;
            list1.replaceSubstring(list1.getHead(), target, replacement);
            cout << "List after replacement: ";
            list1.printList();
            cout<<endl;
        }
        else if (version == 'B' || version == 'b') {
            DoublyLinkedList list1, list2;
            string input1, input2;
            int strChoice;
            char continueChoice = 'y';

            
            while (continueChoice == 'y') {
                
                cout << "Do you want to enter one string or two? (1/2): ";
                cin >> strChoice;

                
                if (strChoice == 1) {
                    cout << "Type in your string: ";
                    cin >> input1;
                    list1.setHead(list1.listIntialiser(input1));

                    char singleStringChoice = 'y';
                    while (singleStringChoice == 'y') {
                        int firstMenuChoice;
                        cout << "Choose which operation you want to perform on your string:\n";
                        cout << "1) Insert new letter\n";
                        cout << "2) Delete current letter\n";
                        cout << "3) Extract substring from current string\n";
                        cout << "4) Replace substring from current string\n";
                        cout << "5) Exit\n";
                        cin >> firstMenuChoice;

                        switch (firstMenuChoice) {
                            case 1: { 
                                int firstInsertChoice;
                                cout << "Insert new letter \n1)Before or after exact letter \n2) At exact letter index\n";
                                cin >> firstInsertChoice;

                                if (firstInsertChoice == 1) {
                                    int insertBeforeAfterChoice;
                                    char letterChoice, newLetter;
                                    cout << "Do you want to insert before or after the letter? \n1) After \n2) Before\n";
                                    cin >> insertBeforeAfterChoice;

                                    cout << "Enter the letter around which you want to insert: ";
                                    cin >> letterChoice;
                                    cout << "Enter the new letter to insert: ";
                                    cin >> newLetter;
                                    list1.insertAfterLetter(list1.getHead(), list1.getTail(), letterChoice, newLetter, insertBeforeAfterChoice == 1);
                                } else if (firstInsertChoice == 2) {
                                    int position;
                                    char newLetter;
                                    cout << "Enter the position where you want to insert: ";
                                    cin >> position;
                                    cout << "Enter the new letter to insert: ";
                                    cin >> newLetter;
                                    list1.insertAtMiddle(list1.getHead(), list1.getTail(), position, newLetter);
                                }
                                break;
                            }

                            case 2: { 
                                int firstInsertChoice;
                                cout << "Delete current letter \n1)Before or after exact letter \n2) At exact letter index\n";
                                cin >> firstInsertChoice;

                                if (firstInsertChoice == 1) {
                                    int deleteBeforeAfterChoice;
                                    char letterChoice;
                                    cout << "Do you want to delete before or after the letter? \n1) After \n2) Before\n";
                                    cin >> deleteBeforeAfterChoice;

                                    cout << "Enter the letter around which you want to delete: ";
                                    cin >> letterChoice;
                                    list1.deleteAroundLetter(list1.getHead(), list1.getTail(), letterChoice, deleteBeforeAfterChoice == 1);
                                } else if (firstInsertChoice == 2) {
                                    int position;
                                    cout << "Enter the position of the letter to delete: ";
                                    cin >> position;
                                    list1.deleteAtPosition(list1.getHead(), list1.getTail(), position);
                                }
                                break;
                            }

                            case 3: { 

                                int start, length;
                                cout << "Enter index and length to get substring: ";
                                cin >> start >> length;
                                DoublyLinkedList substring = list1.substring(start, length);
                                cout << "Substring: ";
                                substring.printList();
                                cout<<endl;
                                break;
                            }

                            case 4: { 
                                string substringTarget, substringReplacement;
                                cout << "Enter substring to replace: ";
                                cin >> substringTarget;
                                cout << "Enter replacement substring: ";
                                cin >> substringReplacement;
                                list1.replaceSubstring(list1.getHead(), substringTarget, substringReplacement);
                                break;
                            }

                            case 5: 
                                singleStringChoice = 'n';
                                break;
                        }

                        if (firstMenuChoice != 5) {
                            cout << "Resulting string: ";
                            list1.printList();
                            cout << endl;

                            cout << "Do you want to continue operations on this string? (y/n): ";
                            cin >> singleStringChoice;
                        }
                    }
                }
                
                else if (strChoice == 2) {
                    cout << "Type in First string: ";
                    cin >> input1;
                    list1.setHead(list1.listIntialiser(input1));

                    cout << "Enter second string: ";
                    cin >> input2;
                    list2.setHead(list2.listIntialiser(input2));

                    char twoStringChoice = 'y';
                    while (twoStringChoice == 'y') {
                        int secondMenuChoice;
                        cout << "Choose which operation you want to perform on your strings:\n";
                        cout << "1) Insert new letter\n";
                        cout << "2) Delete current letter\n";
                        cout << "3) Extract substring from string\n";
                        cout << "4) Replace substring from string\n";
                        cout << "5) Concatenate both strings\n";
                        cout << "6) Exit\n";
                        cin >> secondMenuChoice;

                        switch (secondMenuChoice) {
                            case 1: { 
                                int listChoice, secondInsertChoice;
                                cout << "Which list do you want to modify? (1/2): ";
                                cin >> listChoice;

                                cout << "Insert new letter \n1)Before or after exact letter \n2) At exact letter index\n";
                                cin >> secondInsertChoice;

                                if (listChoice == 1) {
                                    if (secondInsertChoice == 1) {
                                        int insertBeforeAfterChoice;
                                        char letterChoice, newLetter;
                                        cout << "Do you want to insert before or after the letter? \n1) After \n2) Before\n";
                                        cin >> insertBeforeAfterChoice;

                                        cout << "Enter the letter after which you want to insert: ";
                                        cin >> letterChoice;
                                        cout << "Enter the new letter to insert: ";
                                        cin >> newLetter;
                                        list1.insertAfterLetter(list1.getHead(), list1.getTail(), letterChoice, newLetter, insertBeforeAfterChoice == 1);
                                    } else if (secondInsertChoice == 2) {
                                        int position;
                                        char newLetter;
                                        cout << "Enter the position where you want to insert: ";
                                        cin >> position;
                                        cout << "Enter the new letter to insert: ";
                                        cin >> newLetter;
                                        list1.insertAtMiddle(list1.getHead(), list1.getTail(), position, newLetter);
                                    }
                                } else {
                                    if (secondInsertChoice == 1) {
                                        int insertBeforeAfterChoice;
                                        char letterChoice, newLetter;
                                        cout << "Do you want to insert before or after the letter? \n1) After \n2) Before\n";
                                        cin >> insertBeforeAfterChoice;

                                        cout << "Enter the letter after which you want to insert: ";
                                        cin >> letterChoice;
                                        cout << "Enter the new letter to insert: ";
                                        cin >> newLetter;
                                        list2.insertAfterLetter(list2.getHead(), list2.getTail(), letterChoice, newLetter, insertBeforeAfterChoice == 1);
                                    } else if (secondInsertChoice == 2) {
                                        int position;
                                        char newLetter;
                                        cout << "Enter the position where you want to insert: ";
                                        cin >> position;
                                        cout << "Enter the new letter to insert: ";
                                        cin >> newLetter;
                                        list2.insertAtMiddle(list2.getHead(), list2.getTail(), position, newLetter);
                                    }
                                }
                                break;
                            }

                            case 2: { 
                                int listChoice, secondInsertChoice;
                                cout << "Which list do you want to modify? (1/2): ";
                                cin >> listChoice;

                                cout << "Delete current letter \n1)Before or after exact letter \n2) At exact letter index\n";
                                cin >> secondInsertChoice;

                                if (listChoice == 1) {
                                    if (secondInsertChoice == 1) {
                                        int deleteBeforeAfterChoice;
                                        char letterChoice;
                                        cout << "Do you want to delete before or after the letter? \n1) After \n2) Before\n";
                                        cin >> deleteBeforeAfterChoice;

                                        cout << "Enter the letter around which you want to delete: ";
                                        cin >> letterChoice;
                                        list1.deleteAroundLetter(list1.getHead(), list1.getTail(), letterChoice, deleteBeforeAfterChoice == 1);
                                    } else if (secondInsertChoice == 2) {
                                        int position;
                                        cout << "Enter the position of the letter to delete: ";
                                        cin >> position;
                                        list1.deleteAtPosition(list1.getHead(), list1.getTail(), position);
                                    }
                                } else {
                                    if (secondInsertChoice == 1) {
                                        int deleteBeforeAfterChoice;
                                        char letterChoice;
                                        cout << "Do you want to delete before or after the letter? \n1) After \n2) Before\n";
                                        cin >> deleteBeforeAfterChoice;

                                        cout << "Enter the letter around which you want to delete: ";
                                        cin >> letterChoice;
                                        list2.deleteAroundLetter(list2.getHead(), list2.getTail(), letterChoice, deleteBeforeAfterChoice == 1);
                                    } else if (secondInsertChoice == 2) {
                                        int position;
                                        cout << "Enter the position of the letter to delete: ";
                                        cin >> position;
                                        list2.deleteAtPosition(list2.getHead(), list2.getTail(), position);
                                    }
                                }
                                break;
                            }

                            case 3: { 
                                int listChoice;
                                cout << "Which list do you want to modify? (1/2): ";
                                cin >> listChoice;

                                
                                int start, length;
                                cout << "Enter index and length to get substring: ";
                                cin >> start >> length;

                                if (listChoice == 1) {
                                    DoublyLinkedList substring = list1.substring(start, length);
                                    cout << "Substring: ";
                                    substring.printList();
                                    cout<<endl;
                                } else {
                                    DoublyLinkedList substring = list2.substring(start, length);
                                    cout << "Substring: ";
                                    substring.printList();
                                    cout<<endl;
                                }
                                break;
                            }

                            case 4: { 
                                int listChoice;
                                string substringTarget, substringReplacement;
                                cout << "Which list do you want to modify? (1/2): ";
                                cin >> listChoice;

                                cout << "Enter substring to replace: ";
                                cin >> substringTarget;
                                cout << "Enter replacement substring: ";
                                cin >> substringReplacement;

                                if (listChoice == 1) {
                                    list1.replaceSubstring(list1.getHead(), substringTarget, substringReplacement);
                                } else {
                                    list2.replaceSubstring(list2.getHead(), substringTarget, substringReplacement);
                                }
                                break;
                            }

                            case 5: 
                                list1.concat(list2);
                                cout << "Concatenated list: ";
                                list1.printList();
                                cout << endl;
                                break;

                            case 6: 
                                twoStringChoice = 'n';
                                break;
                        }

                        if (secondMenuChoice != 5 && secondMenuChoice != 6) {
                            
                            cout << "First list: ";
                            list1.printList();
                            cout << endl;

                            cout << "Second list: ";
                            list2.printList();
                            cout << endl;

                            cout << "Do you want to continue operations on these strings? (y/n): ";
                            cin >> twoStringChoice;
                        }
                    }
                }

                cout << "Do you want to start over? (y/n): ";
                cin >> continueChoice;
            }
        }
    } while (!(version == 'A' || version == 'a' || version == 'B' || version == 'b'));

    return 0;
}
