#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int v) : V(v), adj(v) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << "\nParallel BFS: ";
        while (!q.empty()) {
            int node = q.front(); q.pop();
            cout << node << " ";
            #pragma omp parallel for
            for (int i = 0; i < (int)adj[node].size(); i++) {
                int neighbor = adj[node][i];
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
        cout << endl;
    }

    void dfsHelper(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";
        #pragma omp parallel for
        for (int i = 0; i < (int)adj[node].size(); i++) {
            int neighbor = adj[node][i];
            bool doVisit = false;
            #pragma omp critical
            {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    doVisit = true;
                }
            }
            if (doVisit) dfsHelper(neighbor, visited);
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        cout << "\nParallel DFS: ";
        dfsHelper(start, visited);
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: "; cin >> V;
    cout << "Enter number of edges: "; cin >> E;
    Graph g(V);
    cout << "Enter " << E << " edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    int start;
    cout << "Enter starting vertex: "; cin >> start;
    g.parallelBFS(start);
    g.parallelDFS(start);
    return 0;
}