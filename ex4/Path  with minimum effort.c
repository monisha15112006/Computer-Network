#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Direction vectors for moving Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// Helper structure for Queue nodes
typedef struct {
    int r, c;
} Point;

// BFS function to check if a path exists with a maximum effort of 'maxEffort'
bool canReachDestination(int** heights, int rows, int cols, int maxEffort) {
    // Visited array to prevent cycles
    bool visited[rows][cols];
    memset(visited, 0, sizeof(visited));
    
    // Queue implementation using static/dynamic array allocation
    Point* queue = (Point*)malloc(rows * cols * sizeof(Point));
    int head = 0, tail = 0;
    
    // Starting point
    queue[tail++] = (Point){0, 0};
    visited[0][0] = true;
    
    while (head < tail) {
        Point curr = queue[head++];
        
        // If we reached the bottom-right corner, a valid path exists
        if (curr.r == rows - 1 && curr.c == cols - 1) {
            free(queue);
            return true;
        }
        
        // Explore 4 neighbors
        for (int i = 0; i < 4; i++) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];
            
            // Check boundaries and if already visited
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) {
                // Check if the current step effort is within our target maxEffort limit
                int current_step_effort = abs(heights[curr.r][curr.c] - heights[nr][nc]);
                
                if (current_step_effort <= maxEffort) {
                    visited[nr][nc] = true;
                    queue[tail++] = (Point){nr, nc};
                }
            }
        }
    }
    
    free(queue);
    return false;
}

int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];
    
    // Binary Search range for efforts
    int low = 0;
    int high = 1000000; // Maximum possible height value constraint
    int ans = high;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // If it's possible to reach using 'mid' effort, try to find a smaller effort
        if (canReachDestination(heights, rows, cols, mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            // Otherwise, increase the allowed effort threshold
            low = mid + 1;
        }
    }
    
    return ans;
}
