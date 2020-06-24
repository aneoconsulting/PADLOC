#include <iostream>

#define EPSILLON 0.01

int main(int argc, char const *argv[])
{

    double  a = 0.1,
            b = 0.2,
            c = 0.7, 
            expected = 1.0;

    if(argc >= 5)
    {
        a = strtod(argv[1],NULL);
        b = strtod(argv[2],NULL);
        c = strtod(argv[3],NULL);
        expected = strtod(argv[4],NULL);
    }
    
    std::cout << std::scientific << "\nA = " << a << "\nB = " << b << "\nC = " << c << "\nExpected : " << expected << std::endl;

    std::cout << "\nA+B+C = " << (a+b)+c << "\t\t Difference to " << expected << " : " << expected - ((a+b)+c); 
    if(a+b+c >= expected-EPSILLON && a+b+c <= expected+EPSILLON) std::cout << "\t\033[1;32m[ OK ]\033[0m"; else std::cout  << "\t\033[1;31m[ KO ]\033[0m";


    std::cout << "\nA+C+B = " << (a+c)+b << "\t\t Difference to " << expected << " : " << expected - ((a+c)+b);
    if(a+c+b >= expected-EPSILLON && a+b+c <= expected+EPSILLON) std::cout << "\t\033[1;32m[ OK ]\033[0m"; else std::cout  << "\t\033[1;31m[ KO ]\033[0m";

    std::cout << "\nB+A+C = " << (b+a)+c << "\t\t Difference to " << expected << " : " << expected - ((b+a)+c);
    if(b+a+c >= expected-EPSILLON && a+b+c <= expected+EPSILLON) std::cout << "\t\033[1;32m[ OK ]\033[0m"; else std::cout  << "\t\033[1;31m[ KO ]\033[0m";

    std::cout << "\nB+C+A = " << (b+c)+a << "\t\t Difference to " << expected << " : " << expected - ((b+c)+a);
    if(b+c+a >= expected-EPSILLON && a+b+c <= expected+EPSILLON) std::cout << "\t\033[1;32m[ OK ]\033[0m"; else std::cout  << "\t\033[1;31m[ KO ]\033[0m";

    std::cout << "\nC+A+B = " << (c+a)+b << "\t\t Difference to " << expected << " : " << expected - ((c+a)+b);
    if(c+a+b >= expected-EPSILLON && a+b+c <= expected+EPSILLON) std::cout << "\t\033[1;32m[ OK ]\033[0m"; else std::cout  << "\t\033[1;31m[ KO ]\033[0m";

    std::cout << "\nC+B+A = " << (c+b)+a << "\t\t Difference to " << expected << " : " << expected - ((c+b)+a);
    if(c+b+a >= expected-EPSILLON && a+b+c <= expected+EPSILLON) std::cout << "\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\033[1;31m[ KO ]\033[0m" << std::endl;

    return 0;
}
