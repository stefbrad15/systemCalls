#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int main (int argCount, char *argValues[]) {

        int processID = 0;
        int status;
        char * command = (char*)("./helloworld");
        char * arguments[] = { (char*)("testing"),(char*)("one"), (char*)("two"), (char*)("three"),(char*)("testing"), NULL };
        char * environment[] = {NULL };

    //  Try to fork off 1 process and have the
    //  parent wait for it's completion. The
    //  child process will start up our revised
    //  helloworld...

    processID = fork();
    if (processID == 0) { // Child process
        execve(command, arguments, environment);
        cerr << "Child process exec failed!! " << endl;
    } else {
        // the parent process -- we have received the
        // child's process id and will wait for it to
        // complete...
        waitpid(processID, &status, 0);
        cout << "Child completed with a status code of : " << (status>>8) << endl;
    }
    return EXIT_SUCCESS;
}
