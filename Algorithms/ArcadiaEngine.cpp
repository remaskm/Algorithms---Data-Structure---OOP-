#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>

using namespace std;
#define TABLE_SIZE 101

//This class implements the player lookup table
class ConcretePlayerTable : public PlayerTable {
private:
    struct Player {
        int playerID;
        string name;
        bool isOccupied;
        bool isDeleted;

        Player() : playerID(-1), name(""), isOccupied(false), isDeleted(false) {}
    };

    vector<Player> hashTable;

    // Primary hash function using Multiplication Method
    int hashFun1(int key) {
        const double A = 0.618033;
        double product = key * A;
        double fractionalPart = product - (int)product;
        return (int)(TABLE_SIZE * fractionalPart);
    }

    // Secondary hash function for double hashing
        int hashFun2(int key) {
        const int PRIME_Num = 97;
        return PRIME_Num - (key % PRIME_Num);
    }

    // Double hashing function
    int doubleHash(int key, int i) {
        return (hashFun1(key) + i * hashFun2(key)) % TABLE_SIZE;
    }

public:
    ConcretePlayerTable() {
        hashTable.resize(TABLE_SIZE);
    }

    void insert(int playerID, string name) override {
        int i = 0;
        int HashIndex;

        // probe until we find an empty or deleted slot
        while (i < TABLE_SIZE) {
            HashIndex = doubleHash(playerID, i);
            // Insert into empty or deleted slot
            if (!hashTable[HashIndex].isOccupied || hashTable[HashIndex].isDeleted) {
                hashTable[HashIndex].playerID = playerID;
                hashTable[HashIndex].name = name;
                hashTable[HashIndex].isOccupied = true;
                hashTable[HashIndex].isDeleted = false;
                return;
            }

            // Update player name if same playerID already exist
            if (hashTable[HashIndex].playerID == playerID) {
                hashTable[HashIndex].name = name;
                hashTable[HashIndex].isDeleted = false;
                return;
            }

            i++;
        }
    }

    string search(int playerID) override {
        int i = 0;
        int HashIndex;
        // Try all possible probe positions
        while (i < TABLE_SIZE) {
            HashIndex = doubleHash(playerID, i);

            if (!hashTable[HashIndex].isOccupied) {
                return "";
            }

            if (hashTable[HashIndex].isOccupied &&
                !hashTable[HashIndex].isDeleted &&
                hashTable[HashIndex].playerID == playerID) {
                return hashTable[HashIndex].name;
            }

            i++;
        }

        return "";  // Not found after full probing
    }
};

//this class implements the game leaderboard using a Skip List where players are sorted in descending order
class ConcreteLeaderboard : public Leaderboard {
private:
    static const int MAX_LEVEL = 16; //setting maximum levels the skip list can have

    //defining a skip list's node representing a player
    struct SkipNode {
        int playerID;
        int score;
        vector<SkipNode*> next;  //vecctor of pointers to next nodes at each of level

        SkipNode(int id, int s, int level) : playerID(id), score(s) {
            next.resize(level + 1, nullptr);
        }
    };

    SkipNode* header;
    int currentMaxLevel;

    // Randomly determines the height of new node to be added with 50% chance to get up to next level
    int getRandomLevel() {
        int level = 0;
        while (rand() % 2 == 0 && level < MAX_LEVEL) { //node level must node be less than max level
            level++;
        }
        return level;
    }


    //Checks if node already in list must come before the node to be added.. newNode<oldNode
    bool shouldComeBefore(SkipNode* node1, int score2, int id2) {
        return (node1->score > score2 ||
                (node1->score == score2 && node1->playerID < id2));
    }

public:
    // initializing the leaderboard
    ConcreteLeaderboard() {
        srand(time(0));  // seed random number generator for getRandomLevel()
        currentMaxLevel = 0;
        header = new SkipNode(-1, INT_MAX, MAX_LEVEL);
    }

    //cleaning up all nodes
    ~ConcreteLeaderboard() {
        SkipNode* node = header;
        while (node != nullptr) {
            SkipNode* next = node->next[0];  //move to next node at level 0
            delete node;
            node = next;
        }
    }

    void addScore(int playerID, int score) override{

        //Check if player already exists
        SkipNode* existingNode = nullptr;
        SkipNode* search = header->next[0];

        while (search != nullptr) {
            if (search->playerID == playerID) {
                existingNode = search;
                break;
            }
            search = search->next[0];
        }

        //If player exists.. remove them
        if (existingNode != nullptr) {
            int oldScore = existingNode->score;

            vector<SkipNode*> oldPreviousNodes(MAX_LEVEL + 1);
            SkipNode* current = header;

            for (int level = currentMaxLevel; level >= 0; level--) {
                while (current->next[level] != nullptr &&
                       shouldComeBefore(current->next[level], oldScore, playerID)) {
                    current = current->next[level];
                }
                oldPreviousNodes[level] = current;
            }

            //remove from all levels
            for (int level = 0; level <= currentMaxLevel; level++) {
                if (oldPreviousNodes[level]->next[level] == existingNode) {
                    oldPreviousNodes[level]->next[level] = existingNode->next[level];
                }
            }

            delete existingNode;

            //reduce currentMaxLevel if top levels are empty
            while (currentMaxLevel > 0 && header->next[currentMaxLevel] == nullptr) {
                currentMaxLevel--;
            }
        }

        //Find position for new score
        vector<SkipNode*> previousNodes(MAX_LEVEL + 1);
        SkipNode* current = header;

        for (int level = currentMaxLevel; level >= 0; level--) {
            while (current->next[level] != nullptr &&
                   shouldComeBefore(current->next[level], score, playerID)) {
                current = current->next[level];
            }
            previousNodes[level] = current;
        }

        //insert the new node
        int newLevel = getRandomLevel();

        if (newLevel > currentMaxLevel) {
            for (int level = currentMaxLevel + 1; level <= newLevel; level++) {
                previousNodes[level] = header;
            }
            currentMaxLevel = newLevel;
        }

        //Creating the new node
        SkipNode* newNode = new SkipNode(playerID, score, newLevel);
        for (int level = 0; level <= newLevel; level++) {
            newNode->next[level] = previousNodes[level]->next[level];
            previousNodes[level]->next[level] = newNode;
        }
    }

    void removePlayer(int playerID) override {
        //Find the node at level 0
        SkipNode* targetNode = nullptr;
        SkipNode* search = header->next[0];

        while (search != nullptr) {
            if (search->playerID == playerID) {
                targetNode = search;
                break;
            }
            search = search->next[0];
        }

        // If player not found..
        if (targetNode == nullptr) {
            return;
        }

        //move on each level to find the nodes before the one we want to remove
        vector<SkipNode*> previousNodes(MAX_LEVEL + 1);
        SkipNode* current = header;
        int targetScore = targetNode->score;

        for (int level = currentMaxLevel; level >= 0; level--) {
            while (current->next[level] != nullptr &&
                   shouldComeBefore(current->next[level], targetScore, playerID)) {
                current = current->next[level];
            }
            previousNodes[level] = current;
        }

        //Remove from all levels
        for (int level = 0; level <= currentMaxLevel; level++) {
            if (previousNodes[level]->next[level] == targetNode) {
                previousNodes[level]->next[level] = targetNode->next[level];
            }
        }

        delete targetNode;

        //Reduce currentMaxLevel when the top levels are empty
        while (currentMaxLevel > 0 && header->next[currentMaxLevel] == nullptr) {
            currentMaxLevel--;
        }
    }

    //return top players
    vector<int> getTopN(int n) override {
        vector<int> topPlayers;

        SkipNode* current = header->next[0];  //begin from first node

        //move along bottom level bec it has all nodes ordered
        int count = 0;
        while (current != nullptr && count < n) {
            topPlayers.push_back(current->playerID);
            current = current->next[0];
            count++;
        }

        return topPlayers;
    }
};

class ConcreteAuctionTree : public AuctionTree {

    enum Color { RED, BLACK };// Colours only defined for our red black tree nodes

    //Node definition
    struct Node {
        int itemID;
        int price;
        Color color;
        Node *left, *right, *parent;

        Node(int id, int p) {
            itemID = id;
            price = p;
            color = RED;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }
    };

    Node* root;
    Node* nil; //null node

    void rotateLeft(Node* x) {
        Node* y = x->right;

        //Move y left child to become x right child
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }

        //connect x parent to y
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        // Put x on y left
        y->left = x;
        x->parent = y;
    }

    //Rotate right around node x
    void rotateRight(Node* x) {
        Node* y = x->left;

        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }

        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node* z) {
        // Keep fixing while parent is red
        while (z->parent->color == RED) {

            // Parent is left child of grandparent
            if (z->parent == z->parent->parent->left) {
                Node* uncle = z->parent->parent->right;

                // if Uncle is red ,Recolor and move up
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    // if Uncle is black and z is right child , Left rotate to make it case 3
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }

                    // if Uncle is black and z is left child , Right rotate and recolor
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            }
            // Parent is right child (mirror of Case A)
            else {
                Node* uncle = z->parent->parent->left;

                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }

        // Root must always be black
        root->color = BLACK;
    }

    // Replace subtree at u with subtree at v
    void transplant(Node* u, Node* v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    // Find node with minimum key in subtree
    Node* minimum(Node* x) {
        while (x->left != nil) {
            x = x->left;
        }
        return x;
    }

    // Fix tree after deletion
    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {

            // if x is left child
            if (x == x->parent->left) {
                Node* sibling = x->parent->right;

                // if brother is red
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    sibling = x->parent->right;
                }

                // if brother is black with two black children
                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    x = x->parent;
                }
                else {
                    // if brother is black, left child is red, right is black
                    if (sibling->right->color == BLACK) {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = x->parent->right;
                    }

                    // if brother is black, right child is red
                    sibling->color = x->parent->color;
                    x->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            // if x is right child
            else {
                Node* sibling = x->parent->left;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    sibling = x->parent->left;
                }

                if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                    sibling->color = RED;
                    x = x->parent;
                }
                else {
                    if (sibling->left->color == BLACK) {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = x->parent->left;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Find node with given itemID
    Node* searchNode(int itemID) {
        Node* current = root;

        while (current != nil && current->itemID != itemID) {
            if (itemID < current->itemID) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return current;
    }

public:
    ConcreteAuctionTree() {
        // Create sentinel nil node
        nil = new Node(0, 0);
        nil->color = BLACK;
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;

        root = nil;
    }

    ~ConcreteAuctionTree() {
        // Memory cleanup would go here
    }

    void insertItem(int itemID, int price) override {
        // Create new node
        Node* z = new Node(itemID, price);
        z->left = nil;
        z->right = nil;

        // Find where to insert (standard BST insertion)
        Node* parent = nil;
        Node* current = root;

        while (current != nil) {
            parent = current;
            if (z->itemID < current->itemID) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Set parent pointer
        z->parent = parent;

        // Insert as left or right child
        if (parent == nil) {
            root = z;  // Tree was empty
        } else if (z->itemID < parent->itemID) {
            parent->left = z;
        } else {
            parent->right = z;
        }

        // New node starts as red
        z->color = RED;

        // Fix any Red-Black violations
        insertFixup(z);
    }

    void deleteItem(int itemID) override {
        Node* z = searchNode(itemID);

        // Item not found
        if (z == nil) {
            return;
        }

        Node* y = z;
        Node* x;
        Color originalColor = y->color;

        // Node has no left child
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        }
        //Node has no right child
        else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        }
        // Node has both children
        else {
            // Find successor
            y = minimum(z->right);
            originalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        // If we removed a black node fix violations
        if (originalColor == BLACK) {
            deleteFixup(x);
        }
    }
};

int InventorySystem::optimizeLootSplit(int n, std::vector<int>& coins) {
    if(n==0)return 0;
    int totalSum=accumulate(coins.begin(),coins.end(),0);
    int target=totalSum/2;

    vector<bool> dp(target+1,false);
    dp[0]=true;
    for(int i=0;i<n;i++)
    {
        for(int j=target;j>=coins[i];j--)
        {
         dp[j]=dp[j] || dp[j-coins[i]];
        }
    }

    int nearest=0;
    for(int i=target;i>=0;i--)
    {
        if(dp[i])
        {
            nearest=i;
            break;
        }
    }

    int diff=abs(nearest-(totalSum-nearest));
    return diff;
}

int InventorySystem::maximizeCarryValue(int capacity, std::vector<pair<int,int>>& items)
{
    int n=items.size();
    vector<vector<int>> dp(n+1,vector<int>(capacity+1,0));

    for(int i=1;i<=n;i++)
    {
        int wt=items[i-1].first;
        int val=items[i-1].second;

        for(int j=0;j<=capacity;j++)
        {
            if(wt<=j)
            {
             dp[i][j]=max(dp[i-1][j],val+dp[i-1][j-wt]);
            }
            else
            {
             dp[i][j]=dp[i-1][j];
            }
        }
    }

    return dp[n][capacity];
}

long long InventorySystem::countStringPossibilities(string s)
{
    int n=s.length();
    if(n==0 || n==1)return 1;
    
    const long long MOD=1000000007;
    //dp[i] stores number of ways to decode first i characters
    vector<long long> dp(n+1,0);
    dp[0]=1; //empty string
    dp[1]=1; //single char
    
    for(int i=2;i<=n;i++)
    {
        //always can take single character
        dp[i]=dp[i-1];
        
        //check if we can form substitution with last 2 chars
        string last2=s.substr(i-2,2);
        if(last2=="uu" || last2=="nn")
        {
         //can decode as w/m or keep as uu/nn
         dp[i]=(dp[i]+dp[i-2])%MOD;
        }
    }
    
    return dp[n];
}

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    if (source == dest) {
        return true; //edge case if source is the same as destination then the pah exists
    }

    //build adjacency list representation of the grapg
    vector<vector<int>> adj(n);
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        //adding the edge in both directions (bidirectional)
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    //if true then vetrex v discovered
    vector<bool> visited(n, false); //d[v]!=∞
    queue<int> q; //queue for bfs traversal

    //start bfs from source
    q.push(source);
    visited[source] = true; //mark source as discovered

    //while Q ≠ ∅
    while (!q.empty()) {
        int u = q.front();  //dequeue the front vertex u ← DEQUEUE(Q))
        q.pop();

        //check if destination was reached
        if (u == dest) {
            return true;
        }

        //destination wasn't reached yet
        //discover all neighbors of current vertex if
        for (int v : adj[u]) {
            //process only unvisited neighbors
            if (!visited[v]) {
                visited[v] = true;  //mark asdiscovered
                q.push(v); //add to queue
            }
        }
    }

    //we couldnt find destination
    return false;  //no path exists
}

long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData) {

//union class for cycle detection
    class DSU {
    private:
        vector<int> parent; //parent[i]
        vector<int> rank; //rank[i] depth of tree rooted at i

    public:
        //create n independent sets
        DSU(int n) {
            parent.resize(n);
            rank.resize(n, 0);

            //each vertex starts as its own parent
            for (int i = 0; i < n; i++) {
                parent[i] = i;
            }
        }

        //finds the representative 'root' of the set containing x
        int find(int x) {
            //compress the path making every node on the path point directly to root
            if (parent[x] != x) {
                parent[x] = find(parent[x]); //recursively find root and compress path
            }
            return parent[x];
        }

        //merges the sets containing x and y
        bool unite(int x, int y) {
            //find representatives of both sets
            int rootX = find(x); //first node
            int rootY = find(y); //second node

            //if already in same set, adding edge would create cycle
            if (rootX == rootY) {
                return false;
            }

            //union by rank to keep the tree balanced
            //attach smaller tree under root of larger tree
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY; //make rootY the parent
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX; //make rootX the parent
            } else {
                parent[rootY] = rootX; //same rank then default to rootX
                rank[rootX]++;
            }

            return true; //sets are merged
        }
    };

    //edge case 1: no roads but multiple cities
    if (m == 0 && n > 1) {
        return -1; //impossible to connect
    }

    //edge case 2: only one city no roads needed
    if (n == 1) {
        return 0;
    }

    //track the total cost
    long long totalCost = 0;
    int edgesUsed = 0; //MST of n vertices has exactly n-1 edges
    DSU dsu(n); //intialize union set

    vector<pair<long long, pair<int, int>>> edges;

    for (const auto& road : roadData) {
        int u = road[0]; //first city
        int v = road[1]; //second city
        long long gold = road[2]; //gold coins required
        long long silver = road[3]; //silver coins required

        //convert to universal currency
        long long weight = gold * goldRate + silver * silverRate;
        //add to queue
        edges.push_back({weight, {u, v}});
    }

    //sort edges E by weight w
    sort(edges.begin(), edges.end());

    //process edges in sorted order
    for (const auto& edge : edges) { //for each edge (u,v) in E
        long long weight = edge.first; //cost of this road
        int u = edge.second.first; //first endpoint
        int v = edge.second.second; //second endpoint

        //check if u and v are in different sets
        if (dsu.find(u) != dsu.find(v)) {

            //if in different sets then add it to MST
            totalCost += weight;
            edgesUsed++;

            //then merge the two sets
            dsu.unite(u, v);

            //we have n-1 edges
            if (edgesUsed == n - 1) {
                break; //all cities are now connected
            }
        }
        //if  u and v are in the same set (already connected) skip this edge
    }

    //check if all  cities are connected
    if (edgesUsed != n - 1) { //MST must have n-1 edges to connect n vertices
        return -1;  //graph is disconnected
    }

    return totalCost; //return total cost of MST
}

string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    const long long INF = 1LL << 60;  //set infinity value to detect missing edges

    // Distance matrix: dist[i][j] will store the shortest known path from i to j
    vector<vector<long long>> dist(n, vector<long long>(n, INF)); //intialized to infinity

    for (int i = 0; i < n; ++i) { //shortest from a node to itself is 0
        dist[i][i] = 0; //set the diagonal of the matrix to 0
    }

    //intialize direct edges (not that this is a directed graph)
    for (const auto& road : roads) {
        int u = road[0];
        int v = road[1];
        int w = road[2];
        dist[u][v] = min(dist[u][v], (long long)w); //handles possible multiple endges between same pair
    }

    //floyd-warshall
    for (int k = 0; k < n; ++k) { //try every node k as an intermediate path
        for (int i = 0; i < n; ++i) { //go over all source nodes i
            for (int j = 0; j < n; ++j) { //go over all destination odes j
                //if going through k gives shorter path, update
                if (dist[i][k] < INF && dist[k][j] < INF) { //if i → k or k → j is INF then the direct edge is sthe shortest path
                    if (dist[i][k] + dist[k][j] < dist[i][j]) { // if i → k → j is shorter than i → j
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    long long total = 0;
    //sum all reachable nodes where i → j. we consider only one direction (i -> j) per pair
    for (int i = 0; i < n; ++i) {
        //add the distance only if a directed path i -> j exists
        for (int j = 0; j < n; ++j) {
            if (i!=j && dist[i][j] < INF) { //if no path exists, dist[i][j] will be INF and is ignored
                total += dist[i][j];
            }
        }
    }
    if (total == 0) return "0"; //binary zero

    string binary;
    while (total > 0) {
        binary.push_back('0' + (total % 2)); //keep on dividing by 2
        total /= 2;
    }
    reverse(binary.begin(), binary.end()); //Reverse so that the result is MSB...LSB
    return binary;
}

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
        //count the frequency of each task a,b,c,..,z
        vector<int> freq(26, 0);
        for (char task : tasks) {
            freq[task - 'A']++;
        }

        //use max-heap to constantly get the task with the highest freq
        priority_queue<int> pq;
        for (int f : freq) {
            if (f > 0) pq.push(f);
        }

        int result = 0; //total cpu intervals including the idle time

        //repeat until all tasks are scheduled
        while (!pq.empty()) {
            vector<int> temp; //store tasks processed in this cycle
            int cycle = n + 1; //length of 1 scheduling cycle is tasks + cooldown

            //current cycle filled with up to n+1 tasks
            for (int i = 0; i < cycle && !pq.empty(); ++i) {
                int curr = pq.top(); //take the task with the highest remaining freq
                pq.pop();
                temp.push_back(curr - 1); //decrement remaining count
                result++; //consider this as 1 cpu interval
            }


            for (int remaining : temp) {
                if (remaining > 0) { //the tasks that still needed to get excuted will be pushed back into the tasks
                    pq.push(remaining);
                }
            }
            //break when all tasks are done
            if (pq.empty()) break;

            //add idle intervals for the slots in the cycle that were not filled
            result += cycle - temp.size();
        }
        return result; //total intervals needed to finish all tasks including idle
    }

// ==========================================
// FACTORY FUNCTIONS
// ==========================================
extern "C" {
    PlayerTable* createPlayerTable() {
        return new ConcretePlayerTable();
    }

    Leaderboard* createLeaderboard() {
        return new ConcreteLeaderboard();
    }

    AuctionTree* createAuctionTree() {
        return new ConcreteAuctionTree();
    }
}
