#include <iostream>
#include "Vector.h"

using namespace std;

void Visit(int& a)
{
    cout << a << endl;
}
template<typename T>
struct Increase { virtual void operator()(T& e) { e++; } };

template<typename T>
void increase(Vector<T>& V) { V.traverse(Increase<T>()); }

int main()
{
    Vector<int> T(10, 5,1);

    Vector<int> T1(T);

    Vector<int> T2(T, 2, 4);

    int test[10] = { 2,4,7,10,14,18,22,25,28,30 };
    Vector<int> T3(test, 10);

    Vector<int> T4(test, 2, 4);

    T = T3;
    
    int test1 = T[2];
    test1 = T.find(98);
    //T.deduplicate();


    //T.traverse(Visit);
    //increase(T);
    //T.unsort();

    T.disordered();

    T.uniquify();
    for (int i = 0; i < T.size(); ++i)
    {
        cout << T[i] << endl;
    }

    cout << T[T.search(2)];
    return 0;
}