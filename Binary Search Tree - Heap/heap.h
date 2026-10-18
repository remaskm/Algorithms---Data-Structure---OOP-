#ifndef HEAP_H
#define HEAP_H

#include "BST.h"
using namespace std;


class MinHeap {
private:
    Task* heapArray;       
    int capacity;          
    int size;              

    
    string* categoryNames; 
    int* categoryCounts;   
    int categorySize;      
    int categoryCapacity;  

    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    
    void heapifyUp(int index) {
        if (index == 0) return;
        int p = parent(index);
        if (heapArray[p].getDuration() > heapArray[index].getDuration()) {
            swap(heapArray[p], heapArray[index]);
            heapifyUp(p);
        }
    }

    
    void heapifyDown(int index) {
        int smallest = index;
        int l = leftChild(index);
        int r = rightChild(index);

        
        if (l < size && heapArray[l].getDuration() < heapArray[smallest].getDuration())
            smallest = l;
        if (r < size && heapArray[r].getDuration() < heapArray[smallest].getDuration())
            smallest = r;

        
        if (smallest != index) {
            swap(heapArray[index], heapArray[smallest]);
            heapifyDown(smallest);
        }
    }

    
    int getCategoryIndex(const string& category) const {
        for (int i = 0; i < categorySize; i++) {
            if (categoryNames[i] == category) return i;
        }
        return -1;  
    }

    
    void resizeCategoryArrays() {
        int newCap = categoryCapacity * 2;
        string* newNames = new string[newCap];
        int* newCounts = new int[newCap];

        
        for (int i = 0; i < categorySize; i++) {
            newNames[i] = categoryNames[i];
            newCounts[i] = categoryCounts[i];
        }

        
        delete[] categoryNames;
        delete[] categoryCounts;

        categoryNames = newNames;
        categoryCounts = newCounts;
        categoryCapacity = newCap;
    }

    
    int countTasksWithDuration(int duration) const {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (heapArray[i].getDuration() == duration) count++;
        }
        return count;
    }

public:
    
    MinHeap(int capacity = 100) {
        this->capacity = capacity;
        heapArray = new Task[capacity];
        size = 0;

        
        categoryCapacity = 10;
        categorySize = 0;
        categoryNames = new string[categoryCapacity];
        categoryCounts = new int[categoryCapacity];
    }

    
    ~MinHeap() {
        delete[] heapArray;
        delete[] categoryNames;
        delete[] categoryCounts;
    }

    
    MinHeap(const MinHeap& other) {
        capacity = other.capacity;
        size = other.size;

        
        heapArray = new Task[capacity];
        for (int i = 0; i < size; i++) {
            heapArray[i] = other.heapArray[i];
        }

        
        categoryCapacity = other.categoryCapacity;
        categorySize = other.categorySize;
        categoryNames = new string[categoryCapacity];
        categoryCounts = new int[categoryCapacity];
        for (int i = 0; i < categorySize; i++) {
            categoryNames[i] = other.categoryNames[i];
            categoryCounts[i] = other.categoryCounts[i];
        }
    }

    
    MinHeap& operator=(const MinHeap& other) {
        if (this != &other) {
            
            delete[] heapArray;
            delete[] categoryNames;
            delete[] categoryCounts;

            
            capacity = other.capacity;
            size = other.size;
            heapArray = new Task[capacity];
            for (int i = 0; i < size; i++) {
                heapArray[i] = other.heapArray[i];
            }

            
            categoryCapacity = other.categoryCapacity;
            categorySize = other.categorySize;
            categoryNames = new string[categoryCapacity];
            categoryCounts = new int[categoryCapacity];
            for (int i = 0; i < categorySize; i++) {
                categoryNames[i] = other.categoryNames[i];
                categoryCounts[i] = other.categoryCounts[i];
            }
        }
        return *this;
    }

    
    void insert(Task task) {
        if (size >= capacity) {
            cout << "Heap is full!" << endl;
            return;
        }

        
        heapArray[size] = task;
        heapifyUp(size);
        size++;

        
        int idx = getCategoryIndex(task.getCategory());
        if (idx != -1) {
            
            categoryCounts[idx]++;
        } else {
            
            if (categorySize >= categoryCapacity) resizeCategoryArrays();
            categoryNames[categorySize] = task.getCategory();
            categoryCounts[categorySize] = 1;
            categorySize++;
        }
    }

    
    Task extractMin() {
        if (size <= 0) {
            cout << "Heap is empty!" << endl;
            return Task();
        }

        
        Task root = heapArray[0];
        heapArray[0] = heapArray[size - 1];
        size--;

        
        if (size > 0) heapifyDown(0);

        
        int idx = getCategoryIndex(root.getCategory());
        if (idx != -1) {
            categoryCounts[idx]--;
            if (categoryCounts[idx] == 0) {
                
                for (int i = idx; i < categorySize - 1; i++) {
                    categoryNames[i] = categoryNames[i + 1];
                    categoryCounts[i] = categoryCounts[i + 1];
                }
                categorySize--;
            }
        }

        return root;
    }

    
    void displayAll() const {
        if (size <= 0) {
            cout << "No completed tasks yet." << endl;
            return;
        }

        
        MinHeap tempHeap = *this;
        while (tempHeap.size > 0) {
            Task t = tempHeap.extractMin();
            t.display();
        }
    }

    
    void displayCategoryReport() const {
        cout << "\n=====Category report======" << endl;
        for (int i = 0; i < categorySize; i++) {
            cout << categoryNames[i] << " = " << categoryCounts[i] << endl;
        }
    }

    
    Task markAsCompleted(TaskManagerBST& bst, int duration, const string& partialDescription) {
        Task found = bst.findTaskByDurationAndDesc(duration, partialDescription);
        if (found.getDuration() > 0) {
            bst.deleteTaskByDurationAndDescription(duration, found.getDescription());
            return found;
        }
        return Task();  
    }

    
    int getTaskCountWithDuration(int duration) {
        return countTasksWithDuration(duration);
    }

    
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }

};

#endif 
