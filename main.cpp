#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <algorithm> // For next_permutation
#include <fstream>
#include <cstdlib>
// 0=Riverside, 1=Moreno Valley, 2=Perris, 3=Hemet
using namespace std;

struct Node {
    int val, cost;
    Node* next;
};

struct Edge {
    int src, dest, weight;
};

class Graph {
    Node** head;
    int N;

public:
    Graph(Edge edges[], int n, int N) {
        head = new Node*[N]();
        this->N = N;

        for (int i = 0; i < N; i++)
            head[i] = nullptr;
        for (unsigned i = 0; i < n; i++) {
            int src = edges[i].src;
            int dest = edges[i].dest;
            int weight = edges[i].weight;
            Node* newNode = getAdjListNode(dest, weight, head[src]);
            head[src] = newNode;
        }
    }

    ~Graph() {
        for (int i = 0; i < N; i++) {
            Node* current = head[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] head;
    }

    Node* getAdjListNode(int value, int weight, Node* head) {
        Node* newNode = new Node;
        newNode->val = value;
        newNode->cost = weight;
        newNode->next = head;
        return newNode;
    }

    void printList(int i) {
        Node* ptr = head[i];
        while (ptr != nullptr) {
            cout <<"("<<i<<", "<<ptr->val<<", "<<ptr->cost<<")";
            ptr = ptr->next;
        }
        cout << endl;
    }
    void tsolarp() {
        vector<int> locations(N);
        for (int i = 0; i < N; i++) {
            locations[i] = i;
        }

        int minCost = numeric_limits<int>::max();
        vector<int> minPath;

        do {
            int currentCost = calculatePathCost(locations);
            if (currentCost < minCost) {
                minCost = currentCost;
                minPath = locations;
            }
        } while (next_permutation(locations.begin() + 1, locations.end()));

        cout << "Shortest path: ";
        for (int i : minPath) {
            cout << i << " ";
        }
        cout << "Total cost: " << minCost << endl;
    }

    int calculatePathCost(const vector<int>& path) {
        int cost = 0;
        for (int i = 0; i < N-1; i++) {
            int u = path[i];
            int v = path[i + 1];
            cost += getEdgeWeight(u, v);
        }
        cost += getEdgeWeight(path[N-1], path[0]);
        return cost;
    }

    int getEdgeWeight(int u, int v) {
        Node* current = head[u];
        while (current != nullptr) {
            if (current->val == v) {
                return current->cost;
            }
            current = current->next;
        }
        return numeric_limits<int>::max(); // Edge not found
    }

  void generateDotFile(const char* filename){
    ofstream dotFile(filename);

    if(dotFile.is_open()){
      dotFile << "graph T {\n";

      for(int i = 0;i < N; i++){
        Node* ptr = head[i];
        while(ptr != nullptr){
          dotFile<<i<<" -- "<<ptr->val<<"[label=\""<<ptr->cost<<"\"];\n";
          ptr = ptr->next;
        }
      }
      dotFile << "}\n";
      dotFile.close();
    }else{
      cerr<<"Unable to open Dot File";
    }
  }
};

int main() {
    Edge edges[] = {
      {0, 1, 16}, {0, 2, 24}, {0, 3, 33},
      {1, 0, 16}, {1, 2, 18}, {1, 3, 26},
      {2, 0, 24}, {2, 1, 18}, {2, 3, 30},
      {3, 0, 33}, {3, 1, 26}, {3, 2, 30}
    };

    int N = 4;
    int n = sizeof(edges) / sizeof(edges[0]);
    Graph graph(edges, n, N);
    graph.generateDotFile("graph.dot");

    graph.tsolarp();

    return 0;
}

//For the generateDotFile function you have to use this "dot -Tpng graph.dot -o graph.png" in the terminal to generate the graph.png file. the dot graph will look a bit weird since the edge edges[] has duplicates and it duplicates the paths but it still get the right total cost   
