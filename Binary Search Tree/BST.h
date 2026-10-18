#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>

using namespace std;


class Task {
private:
    string description;
    int duration;
    string category;

    string toLower(const string& str) const {
        string result = str;
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }
public:
    
    Task() : description(""), duration(0), category("") {}

    
    Task(const string& descp, int duration, const string& catg)
        : description(descp), duration(duration), category(catg) {}

    int getDuration() const {
        return duration;
    }

    string getDescription() const {
        return description;
    }

    string getCategory() const {
        return category;
    }

    void display() const {
        cout << "[" << description << ", " << duration << ", " << category << "]" << std::endl;
    }

    
    bool descriptionContains(const string& substr) const {
        string descLower = toLower(description);
        string subLower = toLower(substr);
        return descLower.find(subLower) != string::npos;
    }
};


class BstNode {
public:
    Task task;
    BstNode* left;
    BstNode* right;

    
    BstNode(const Task& t) : task(t), left(nullptr), right(nullptr) {}
};


class TaskManagerBST {
private:
    BstNode* root;

    
    void inOrderTraversal(BstNode* node) {
        if (node == nullptr) return; 

        inOrderTraversal(node->left); 
        node->task.display(); 
        inOrderTraversal(node->right); 
    }

    
    void BSTfindTasksWithExactDuration(BstNode* node, int duration, bool& found) {
        if (node == nullptr) { 
            return;
        }

        
        if (duration < node->task.getDuration()) {
            BSTfindTasksWithExactDuration(node->left, duration, found);
        }
        
        else if (duration == node->task.getDuration()) {
            found = true;
            
            node->task.display();

            
            
            BSTfindTasksWithExactDuration(node->left, duration, found);

            
            BSTfindTasksWithExactDuration(node->right, duration, found);
        }
        
        else {
            BSTfindTasksWithExactDuration(node->right, duration, found);
        }
    }

    
    void showTasksAtLeastDuration(BstNode* node, int minDuration, bool& found) {
        if (node == nullptr) return; 

        
        showTasksAtLeastDuration(node->left, minDuration, found);

        
        if (node->task.getDuration() >= minDuration) {
            found = true;
            node->task.display();
        }

        
        showTasksAtLeastDuration(node->right, minDuration, found);
    }

    
    void showTasksAtMostDuration(BstNode* node, int maxDuration, bool& found) {
        if (node == nullptr) return; 

        
        showTasksAtMostDuration(node->left, maxDuration, found);

        
        if (node->task.getDuration() <= maxDuration) {
            found = true;
            node->task.display();
        }

        
        if (node->task.getDuration() <= maxDuration) {
            showTasksAtMostDuration(node->right, maxDuration, found);
        }
    }

    
    BstNode* findMin(BstNode* node) {
        if (node == nullptr) {
            return nullptr;
        }

        BstNode* current = node;
        
        while (current->left != nullptr) {
            current = current->left;
        }

        return current;
    }

    
    BstNode* deleteTaskByDuration(BstNode* currentNode, int targetDuration, bool& isDeleted) {
        if (currentNode == nullptr) { 
            return nullptr;
        }

        
        if (targetDuration < currentNode->task.getDuration()) {
            currentNode->left = deleteTaskByDuration(currentNode->left, targetDuration, isDeleted);
        }
        
        else if (targetDuration > currentNode->task.getDuration()) {
            currentNode->right = deleteTaskByDuration(currentNode->right, targetDuration, isDeleted);
        }
        
        else {
            isDeleted = true;

            
            if (currentNode->left != nullptr && currentNode->left->task.getDuration() == targetDuration) {
                
                
                BstNode* temp = currentNode->left;
                currentNode->left = temp->left;
                temp->right = currentNode->right;
                delete currentNode;
                return temp;
            }

            
            if (currentNode->left == nullptr) {
                BstNode* temp = currentNode->right;
                delete currentNode;
                return temp;
            }
            
            else if (currentNode->right == nullptr) {
                BstNode* temp = currentNode->left;
                delete currentNode;
                return temp;
            }

            
            
            BstNode* smallestOnRight = findMin(currentNode->right);

            
            currentNode->task = smallestOnRight->task;

            
            bool tempDeleted = false;
            currentNode->right = deleteTaskByDuration(currentNode->right, smallestOnRight->task.getDuration(), tempDeleted);
        }

        return currentNode; 
    }

    
    BstNode* deleteAllTasksWithDuration(BstNode* currentNode, int targetDuration, bool& isDeleted) {
        if (currentNode == nullptr) {
            return nullptr;
        }

        
        if (targetDuration < currentNode->task.getDuration()) {
            currentNode->left = deleteAllTasksWithDuration(currentNode->left, targetDuration, isDeleted);
            return currentNode;
        }
        
        else if (targetDuration > currentNode->task.getDuration()) {
            currentNode->right = deleteAllTasksWithDuration(currentNode->right, targetDuration, isDeleted);
            return currentNode;
        }
        
        else {
            isDeleted = true;

            
            
            bool moreEqualDurationNodes = false;
            currentNode->left = deleteAllTasksWithDuration(currentNode->left, targetDuration, moreEqualDurationNodes);

            
            if (currentNode->left == nullptr) {
                BstNode* temp = currentNode->right;
                delete currentNode;
                return temp;
            }
            
            else if (currentNode->right == nullptr) {
                BstNode* temp = currentNode->left;
                delete currentNode;
                return temp;
            }

            
            
            BstNode* smallestOnRight = findMin(currentNode->right);

            
            currentNode->task = smallestOnRight->task;

            
            bool tempDeleted = false;
            currentNode->right = deleteTaskByDuration(currentNode->right, smallestOnRight->task.getDuration(), tempDeleted);
        }

        return currentNode;
    }

    
    void destructTree(BstNode* node) {
        if (node != nullptr) {
            destructTree(node->left);
            destructTree(node->right);
            delete node;
        }
    }

    BstNode* deleteByDurationAndDescHelper(BstNode* node, int duration, const string& description, bool& deleted) {
        if (node == nullptr) return nullptr;

        
        if (duration < node->task.getDuration()) {
            node->left = deleteByDurationAndDescHelper(node->left, duration, description, deleted);
        }
        else if (duration > node->task.getDuration()) {
            node->right = deleteByDurationAndDescHelper(node->right, duration, description, deleted);
        }
        else {
            
            if (node->task.getDescription() == description) {
                deleted = true;

                
                if (node->left == nullptr) {
                    BstNode* temp = node->right;
                    delete node;
                    return temp;
                }
                else if (node->right == nullptr) {
                    BstNode* temp = node->left;
                    delete node;
                    return temp;
                }
                else {
                    BstNode* minNode = findMin(node->right);
                    node->task = minNode->task;

                    bool dummy = false;
                    node->right = deleteByDurationAndDescHelper(node->right, minNode->task.getDuration(), minNode->task.getDescription(), dummy);
                }
            } else {
                
                node->left = deleteByDurationAndDescHelper(node->left, duration, description, deleted);
            }
        }

        return node;
    }

public:
    
    TaskManagerBST() : root(nullptr) {}

    
    ~TaskManagerBST() {
        destructTree(root);
    }

    bool deleteTaskByDurationAndDescription(int duration, const string& description) {
        bool deleted = false;
        root = deleteByDurationAndDescHelper(root, duration, description, deleted);
        return deleted;
    }

    Task findTaskByDurationAndDesc(int duration, const string& partialDescription) {
        BstNode* current = root;

        while (current != nullptr) {
            if (duration < current->task.getDuration()) {
                current = current->left;
            } else if (duration > current->task.getDuration()) {
                current = current->right;
            } else {
                
                if (current->task.descriptionContains(partialDescription)) {
                    return current->task;
                }

                
                current = current->left;
            }
        }

        return Task(); 
    }

    
    void addTaskToTree(const string& description, int duration, const string& category) {
        
        cout << "Adding task to tree: [" << description << ", " << duration << ", " << category << "]" << endl;

        
        Task newTask(description, duration, category);

        
        if (root == nullptr) {
            cout << "Tree is empty, creating root node" << endl;
            root = new BstNode(newTask);
            return;
        }

        
        BstNode* current = root;
        BstNode* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            
            if (duration == current->task.getDuration()) {
                cout << "Found node with same duration, inserting to left" << endl;
                BstNode* newNode = new BstNode(newTask);
                newNode->left = current->left;
                current->left = newNode;
                return;
            }

            
            if (duration < current->task.getDuration()) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        
        BstNode* newNode = new BstNode(newTask);
        if (duration < parent->task.getDuration()) {
            cout << "Inserting as left child of node with duration " << parent->task.getDuration() << endl;
            parent->left = newNode;
        } else {
            cout << "Inserting as right child of node with duration " << parent->task.getDuration() << endl;
            parent->right = newNode;
        }
    }

    
    void showAllTasksSorted() {
        if (root == nullptr) {
            cout << "There are no tasks yet to show..." << endl;
            return;
        }

        
        cout << "Root task:";
        root->task.display();
        cout<<endl;

        inOrderTraversal(root); 
    }

    
    void findTasksWithExactDuration(int duration) {
        if (root == nullptr) {
            cout << "No tasks are in the system yet" << endl;
            return;
        }

        bool isFound = false;

        cout << "\nSearching for tasks that are exactly " << duration << " minutes long...\n" << endl;
        BSTfindTasksWithExactDuration(root, duration, isFound);  

        if (!isFound) {
            cout << "No tasks found that are " << duration << " minutes!" << endl;
        }
    }

    
    void deleteTasksWithExactDuration(int duration) {
        if (root == nullptr) {
            cout << "There are no tasks to delete currently in the system!" << endl;
            return;
        }

        bool wasDeleted = false;

        
        root = deleteAllTasksWithDuration(root, duration, wasDeleted);

        if (wasDeleted) {
            cout << "Task/s that are " << duration << " minutes long were successfully deleted!" << endl;
        } else {
            cout << "Couldn't find any task with duration " << duration << " minutes to delete!" << endl;
        }
    }

    
    void showTasksWithDurationAtLeast(int duration) {
        if (root == nullptr) {
            cout << "No matching tasks available to show right now!" << endl;
            return;
        }

        bool foundAny = false;

        cout << "\nShowing all tasks with duration >= " << duration << " minutes:\n" << endl;
        showTasksAtLeastDuration(root, duration, foundAny);

        if (!foundAny) {
            cout << "No tasks found with duration of " << duration << " minutes or more!" << endl;
        }
    }

    
    void showTasksWithDurationAtMost(int duration) {
        if (root == nullptr) {
            cout << "No matching tasks available to show right now!" << endl;
            return;
        }

        bool foundAny = false;

        cout << "\nShowing all tasks with duration <= " << duration << " minutes:\n" << endl;
        showTasksAtMostDuration(root, duration, foundAny);

        if (!foundAny) {
            cout << "No tasks found with duration of " << duration << " minutes or less!" << endl;
        }
    }
};

#endif 
