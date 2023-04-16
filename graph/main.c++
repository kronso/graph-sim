#include <iostream>
#include <string>
#include <math.h> 
#include <conio.h>

#define H 31
#define W 31    

#define clear() std::cout << "\x1b[H";

class Graph {
    // PUBLIC attributes
    public:
        char (*grid)[W] = new char[H][W]; // 10 pointers to 10 arrays; 10x10 = -5 x 5
        const int QUATER = H/2; // quater of a grid.
        int y_origin, x_origin; // the middle of a grid
        // constructor
        Graph(int y, int x) { 
            y_origin = y; x_origin = x;
        }
};

void initGraph(Graph &graph) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            graph.grid[y][x] = ' ';
        } 
    }
}

void printGraph(Graph &graph) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (graph.grid[y][x] != 'x') {
                if (x % graph.x_origin == 0 && x != 0 && x != W - 1) {
                    std::cout << "| ";
                } else if (y % graph.y_origin == 0 && y != 0 && y != H - 1) {
                    std::cout << "- ";
                } else {
                    std::cout << graph.grid[y][x] << " ";
                }
            } else {
                std::cout << graph.grid[y][x] << " ";
            }
        }
        std::cout << std::endl;
    }
}

// can't do fractions
void xToPower(Graph &graph, double exp, double v_dilation = 1, double h_dilation = 1, double x_intercept = 0, double y_intercept = 0) {
    // y = result;
    // decrease x to make the graph more visible
    double y, x;
    for (x = -graph.QUATER; x <= graph.QUATER; x += .001) {
        // y = (v_dilation)((h_dilation)x + x_intercept)^exp + y_intercept;
        y = pow(h_dilation * x + x_intercept, exp) * v_dilation + y_intercept;
        // if (v_dilation > 0 || h_dilation > 0) {y = ceil(y);} else {y = floor(y);}
        if (y > graph.QUATER || y < -graph.QUATER || isnan(y)) {continue;} 
        graph.grid[graph.y_origin - (int) y][(int) x + graph.x_origin] = 'x';
    }
}

// can't do fractions
void constantToX(Graph &graph, double variable, double v_dilation = 1, double h_dilation = 1, double x_intercept = 0, double y_intercept = 0) {
    // variable = whole number to an exponent;
    double y, x;
    for (x = -graph.QUATER; x <= graph.QUATER; x += .001) {
        // y = (v_dilation)variable^((h_dilation)x + x_intercept) + y_intercept
        y = pow(variable, (h_dilation * x + x_intercept)) * v_dilation + y_intercept;
        // floor or ceil used to ensure the continuous...
        // graph never reaches 0 as x approaches infinity (limit)
        // if (v_dilation >= 0) {y = ceil(y);} else {y = floor(y);}
        if (y > graph.QUATER || y < -graph.QUATER || isnan(y)) {continue;} 
        graph.grid[graph.y_origin - (int) y][(int) x + graph.x_origin] = 'x';
    }
}

void waveFunc(Graph &graph, double amplitude = 1, double wave_length = 1, double x_intercept = 0, double y_intercept = 0) {
    const double PI = 3.14159265358979323846;
    double w, x;
    for (x = -graph.QUATER; x <= graph.QUATER; x += .001) {
        // y = (v_dilation)variable^((h_dilation)x + x_intercept) + y_intercept
        w = amplitude * sin(x * ((2 * PI)/wave_length) + x_intercept) + y_intercept;
        // floor or ceil used to ensure the continuous...
        // graph never reaches 0 as x approaches infinity (limit)
        // if (v_dilation >= 0) {y = ceil(y);} else {y = floor(y);}
        if (w > graph.QUATER || w < -graph.QUATER || isnan(w)) {continue;} 
        graph.grid[graph.y_origin - (int) w][(int) x + graph.x_origin] = 'x';
    }
}

int main() {
    int y_origin = (float) ceil(W / 2);
    int x_origin = (float) ceil(H / 2);

    Graph graph(y_origin, x_origin);
    initGraph(graph);
    const double PI = 3.14159265358979323846;
    const double E = exp(1); // euler's constant 

    waveFunc(graph, 3, 12, 1);
    printGraph(graph);

    double i = 1;
    std::cout << "\033c";
    while (1) {
        clear();   
        // constantToX(graph, E, i, (double)1/10, 0, -i);
        // constantToX(graph, E, i, (double)-1/10, 0, -i);
        // constantToX(graph, E, -i, (double)1/10, 0, i);
        // constantToX(graph, E, -i, (double)-1/10, 0, i);
        // xToPower(graph, 3, (double) 1/i);
        // xToPower(graph, 3, (double) 1/i, 1, 1);
        // xToPower(graph, 3, (double) 1/i, 1, 2);
        waveFunc(graph, i, 12, -i, -i);
        waveFunc(graph, i, 12, -i, i);
        waveFunc(graph, i, 12, i, i);
        waveFunc(graph, i, 12, i, -i);
        if (i > 30) {
            i = 1;
        }
        printGraph(graph);
        initGraph(graph);
        // _kbhit();
        // getch();
        i += .5;
    }
    return EXIT_SUCCESS;
}