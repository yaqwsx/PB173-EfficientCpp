#include <iostream>
#include <vector>
#include <utility>

struct Point { double x, y; };

int main() {
    std::vector< Point > data;
    Point p;
    while( std::cin >> p.x >> p.y )
        data.push_back( p );

    double xsum = 0;
    double xsum2 = 0;
    double ysum = 0;
    double xysum = 0;

    for ( auto p : data ) {
        xsum += p.x;
        xsum2 += p.x * p.x;
        ysum += p.y;
        xysum += p.y * p.x;
    }

    double a = (data.size() * xysum - xsum * ysum ) / ( data.size() * xsum2 - xsum * xsum );
    double b = ( xsum2 * ysum - xsum * xysum ) / ( xsum2 * data.size() - xsum * xsum );
    std::cout << "a: " << a << ", b: " << b << "\n";
}