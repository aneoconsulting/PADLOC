#include <iostream>

using namespace std;

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
    
    cout << scientific << "\nA = " << a << "\nB = " << b << "\nC = " << c << "\nExpected : " << expected << endl;

    cout << "\nA+B+C = " << (a+b)+c << "\t\t Difference to " << expected << " : " << expected - ((a+b)+c);
    cout << "\nA+C+B = " << (a+c)+b << "\t\t Difference to " << expected << " : " << expected - ((a+c)+b);
    cout << "\nB+A+C = " << (b+a)+c << "\t\t Difference to " << expected << " : " << expected - ((b+a)+c);
    cout << "\nB+C+A = " << (b+c)+a << "\t\t Difference to " << expected << " : " << expected - ((b+c)+a);
    cout << "\nC+A+B = " << (c+a)+b << "\t\t Difference to " << expected << " : " << expected - ((c+a)+b);
    cout << "\nC+B+A = " << (c+b)+a << "\t\t Difference to " << expected << " : " << expected - ((c+b)+a) << endl;
    return 0;
}
