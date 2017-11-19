#include <iostream>

int main() {
    std::cout << "#include <stdio.h>\nint main(){ char array[1024*1024] = {0}; char *ptr = array;";

    char c;
    while( std::cin >> c ) {
        switch ( c ) {
        case '>':   std::cout << "++ptr;"; break;
        case '<':   std::cout << "--ptr;"; break;
        case '+':   std::cout << "++*ptr;"; break;
        case '-':   std::cout << "--*ptr;"; break;
        case '.':   std::cout << "putchar( *ptr );"; break;
        case ',':   std::cout << "*ptr=getchar();"; break;
        case '[':   std::cout << "while( *ptr ) { "; break;
        case ']':   std::cout << "}"; break;
        }
    }

    std::cout << "}";
}