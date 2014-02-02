#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argCount, char *argValues[]) {
    int count;

    for (count = 0; count < argCount; count++) {
        cout << "Argument " << count << " = " << argValues[count] << endl;
    }

    cout << "Hello World ! " << endl;

    return argCount;
}
