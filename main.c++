#include <iostream>
#include <string>
#include <math.h> 
#include <conio.h>

#define H 31
#define W 31

#define clear() printf("\e[1;1H\e[2J");

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
    for (x = -graph.QUATER; x <= graph.QUATER; x += .1) {
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
    for (x = -graph.QUATER; x <= graph.QUATER; x += 1) {
        // y = (v_dilation)variable^((h_dilation)x + x_intercept) + y_intercept
        y = pow(variable, (h_dilation * x + x_intercept)) * v_dilation + y_intercept;
        // floor or ceil used to ensure the continuous...
        // graph never reaches 0 as x approaches infinity (limit)
        // if (v_dilation >= 0) {y = ceil(y);} else {y = floor(y);}
        if (y > graph.QUATER || y < -graph.QUATER || isnan(y)) {continue;} 
        graph.grid[graph.y_origin - (int) y][(int) x + graph.x_origin] = 'x';
    }
}

// struct Attributes {
//     Attributes *node;
//     char *variable_count = new char;
//     int *digits = new int;
//     bool power; 
// };

// void power(Graph &graph, Attributes &attr, std::string equation) {
//     for (int i = 0; i < equation.size(); i++) {
//         switch (equation[i]) {
//             case '+':
//                 attr.add = true;
//                 break;
//             case '-':
//                 attr.minus = true;
//                 break;
//             case '*':
//                 attr.multi = true;
//                 break;
//             case '/':
//                 attr.divid = true;
//                 break;
//             case '^':
//                 attr.power = true;
//                 break;
//             if (isalpha(i)) {attr.variable_count[i] = i;}
//             if (isdigit(i)) {attr.variable_count[i] = i;}
//         }
//     }
//     // have as much variables a,b,x,y
//     // have as much left or right shifts
//     // have as much coefficients / powers / sqrt
//     // 1/2x(x+x)^2+x^x
//     // ^^^^^^^^^ the wild shit

// }

int main() {
    // int w, h;
    // bool invaild = true;
    // while (invaild) {
    //     std::cout << "Enter graph size: (w, h)\n\n>>> ";
    //     std::cin >> w >> h;
    //     if (w == 0 || h == 0) {std::cout << "\ninavild type";}
    //     invaild = false;  
    // }
    int y_origin = (float) ceil(W / 2);
    int x_origin = (float) ceil(H / 2);

    Graph graph(y_origin, x_origin);
    initGraph(graph);
    const double PI = 3.14159265358979323846;
    const double E = exp(1); // euler's constant 
    // xToPower(graph, 1);
    // xToPower(graph, 1, 1, -1, 1);
    // printGraph(graph);
    double i = 1, j = -1;
    while (1) {
        clear();   
        // xToPower(graph, 4, 1, (double)1/i);
        xToPower(graph, 3, (double) 1/i);
        xToPower(graph, 3, (double) 1/i, 1, 1);
        xToPower(graph, 3, (double) 1/i, 1, 2);
        if (i > 10000 || j < -10000) {
            j = 1, i = -1;
        } else if (j > 10000 || i < -10000) {
            j = -1, i = 1;
        }
        // xToPower(graph, 3, 1, (double)1/i, (double)0.1);
        printGraph(graph);
        initGraph(graph);
        i *= 1.5;
    }
    _kbhit();
    getch();
    return EXIT_SUCCESS;
}