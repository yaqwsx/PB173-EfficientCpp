#include <iostream>
#include <vector>
#include <utility>

int main() {
    double xsum = 0;
    double xsum2 = 0;
    double ysum = 0;
    double xysum = 0;

    double x, y;
    int count = 0;
    while ( std::cin >> x >> y ) {
        xsum += x;
        xsum2 += x * x;
        ysum += y;
        xysum += y * x;
        count++;
    }

    double a = ( count * xysum - xsum * ysum ) / ( count * xsum2 - xsum * xsum );
    double b = ( xsum2 * ysum - xsum * xysum ) / ( xsum2 * count - xsum * xsum );
    std::cout << "a: " << a << ", b: " << b << "\n";
}