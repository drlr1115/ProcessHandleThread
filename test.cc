//
// test.cc
//
// Cliff Dong
//


#include <iostream>
#include <string>
#include <vector>

#include <ProcessHandleThread.h>

using namespace std;

static int log_info(int id)
{
    cout << "Process " << id << " exited." << endl;
    return 0;
}

int main()
{
    ProcessHandleThread *app = new ProcessHandleThread(1);
    vector<string> args;
    args.push_back("5");
    app->start("sleep", args, log_info);
    sleep(6);
    return 0;
}
