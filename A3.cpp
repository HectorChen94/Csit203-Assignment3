#include <cstdio>
#include <iostream>
#include <istream>
#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

struct Point {
    int x;
    int y;
};

const int maxN=100;
const int infinite = 10000000;

int a[maxN][maxN]; // store the graph
Point v[maxN]; // store the coordinates of vertexes
bool checkFile;
int n, m, s, e;

//shortest path variables
int d[maxN], p[maxN], shortestPath[maxN]; // variables to store the paths from start to all vertexes, retrieve of paths, shortest path
bool shortest[maxN]; // check if the path is visited
bool isConected; // check if there is an end of path
int shortestValue;

//longest path variables
int path[maxN], longestPath[maxN]; // store paths and the longest paths
bool longest[maxN]; // check if the vertexes are visited
int sum, edges, longestValue;

void readFile(){
    char in_name[80];
    FILE *in_file;
    int x,y,k;

    cout << "Please Enter the Filename: ";
    cin >> in_name;
    in_file = fopen(in_name, "r");
    if (in_file == NULL){
        cerr << "\nCan't open " << in_name << " for reading.\n";
        checkFile = false;
    } else {
        // read in vertexes and edges
        fscanf(in_file,"%d %d", &n, &m);

        // read in coordinates of v
        for (int i = 0; i < n; i++) {
            fscanf(in_file,"%d %d %d", &k, &x, &y);
            v[k].x = x;
            v[k].y = y;
        }

        // read in the graph
        for (int i = 0; i < m; i++){
            fscanf(in_file,"%d %d %d", &x, &y, &k);
            a[x-1][y-1] = k;
        }

        // read in the start and end vertexes of each edge
        fscanf(in_file,"%d %d",&s, &e);

        s--;
        e--;
        fclose(in_file);
        checkFile = true;
    }
}

// find current vertex with shortest distance
int findI(){
    int mn = infinite;
    int index = -1;
    for (int i = 0; i < n; i++){
        if (mn > d[i] && shortest[i]== false){
            mn = d[i];
            index = i;
        }
    }
    return index;
}

// Dijkstra's Algorithm
void dijkstra(){
    int i;
    while(true){
        // find an unvisited vertex
        i = findI();

        // if there is no vertex to continue
        if (i == -1){
            isConected=false;
            return;

        // if there is a goal vertex
        } else if (i == e){
            isConected=true;
            return;


        } else {
            shortest[i] = true; // the path is satisfied and marked the vertex

            // find the shortest length of connected vertexes link to i
            for (int j = 0; j < n; j++) {
                if (shortest[j] == false && a[i][j]>0 && d[j] >  d[i] + a[i][j]) {
                    d[j] = d[i] + a[i][j]; // assign to new graph
                    p[j]= i; // save trace for later retrieve
                }
            }
        }
    }
}

// retrieval of shortest path
void sPath(){
    // if there is no goal vertex
    if (isConected == false) {
        cout << "Couldn't find the end vertex!" << endl;

    } else {
        shortestPath[0] = e; // assign the goal vertex to the start of the array
        int index = e; // point to the goal vertex
        int size = 1; // the size of array is 1
        while (true){
            shortestPath[size] = p[index]; // the next element in the array is equal to element of the retrieval array that has index at the previous vertex
            index = p[index]; // assign the index to current vertex
            size++;
            if (index == s)
                break;
        }

        for (int i = size-1; i >= 1; i--){
            cout << shortestPath[i] + 1 << " -> ";
        }
        cout << shortestPath[0] + 1 << endl;
    }
}

void findShortestPath(){
    // initialize
    for (int i = 0; i < n; i++){
        p[i] = i; // start is unknown for retrieval
        d[i]= infinite; // initialize start equal to the longest
    }
    d[s] = 0;
    isConected = false; // the goal vertex in unknown at the start
    dijkstra();
    shortestValue = d[e];
}

// function to find the path that is longer
void checkPath(int edge){
    // if the new path is longer than the previous one
    if(sum > longestValue){
        longestValue = sum;
        edges = edge;
        for(int i = 0; i < edge; i++){
            longestPath[i] = path[i];
        }
    }
}

// recursion function to find paths that lead to the goal vertex
void searchRecursion(int numOfEdge){
    // if a new path to goal vertex is found
    if(path[numOfEdge-1] == e) {
        checkPath(numOfEdge); // check the length of this path

    // find the path
    } else {
        for(int i = 0; i < n; i++){
            if ( a[path[numOfEdge-1]][i] > 0 && longest[i] == false){
                path[numOfEdge] = i;
                longest[i] = true;
                sum += a[path[numOfEdge-1]][i];
                searchRecursion(numOfEdge+1);
                path[numOfEdge] = 0;
                sum -= a[path[numOfEdge -1]][i];
                longest[i] = false;
            }
        }
    }
}

void findLongestPath(){
    for(int i = 0; i<n; i++){
        longest[i] = false; // initialization of vertexes are unmarked
        path[i] = 0;
    }
    longest[s] = true; // initialize of start vertex that was visited
    path[0] = s; // initialize the initial path always start from s(from read in)
    sum = 0; // variable store temporary length
    longestValue = 0; // variable store the length of the final longest path
    searchRecursion(1); // initialize recursion with 1
}

// function to print vertexes on the longest path
void lPath() {
    cout << s+1;
    for(int i=1; i < edges; i++){
        cout << " -> " << longestPath[i] + 1;
    }
    cout << endl;
}

double getEuclidDistance(){
    double result = (v[s+1].x - v[e+1].x)*(v[s+1].x - v[e+1].x) + (v[s+1].y - v[e+1].y)*(v[s+1].y - v[e+1].y);
    result = sqrt(result);
    return result;
}

void output() {
    cout << "\n=================================================================================================================================\n";
    cout << "\nThe number of vertexes in the graph: " << n << endl;
    cout << "\nThe number of edges in the graph: " << m << endl;
    cout << "\nThe start vertexes: " << s+1 << endl;
    cout << "\nThe end vertexes: " << e+1 << endl;
    cout << "\n=================================================================================================================================\n";
    cout << "\nThe Euclidean distance between the start and the goal vertexes: " << getEuclidDistance() << endl;
    cout << "\nShortest path: ";
    sPath();
    cout << "\nThe length of the shortest path: " << shortestValue << endl;
    cout << "\nLongest path: ";
    lPath();
    cout << "\nThe length of the longest path: " << longestValue << endl;
    cout << "\n=================================================================================================================================\n";
}

int main(){

    readFile();
    if (checkFile){
        findShortestPath();
        findLongestPath();
        output();
    }
    return 0;
}