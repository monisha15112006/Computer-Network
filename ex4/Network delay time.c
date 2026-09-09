#include <stdio.h>
#include <stdbool.h>

#define INF 1e9

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    // Step 1: Initialize distance array and visited array
    // We use size n + 1 because the nodes are 1-indexed
    int dist[n + 1];
    bool visited[n + 1];
    
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    
    // Distance to the source node is 0
    dist[k] = 0;
    
    // Step 2: Dijkstra's main loop
    for (int i = 0; i < n; i++) {
        // Find the unvisited node with the minimum distance
        int u = -1;
        int minDist = INF;
        
        for (int j = 1; j <= n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        
        // If we can't find a reachable node, we stop early
        if (u == -1) break;
        
        // Mark the selected node as visited
        visited[u] = true;
        
        // Relax the edges outgoing from node 'u'
        for (int j = 0; j < timesSize; j++) {
            int source = times[j][0];
            int target = times[j][1];
            int weight = times[j][2];
            
            if (source == u) {
                if (dist[u] + weight < dist[target]) {
                    dist[target] = dist[u] + weight;
                }
            }
        }
    }
    
    // Step 3: Find the maximum time taken to reach any node
    int maxTime = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) {
            return -1; // Node is unreachable
        }
        if (dist[i] > maxTime) {
            maxTime = dist[i];
        }
    }
    
    return maxTime;
}
