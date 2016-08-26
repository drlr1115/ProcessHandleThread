//
// File: ProcessHandleThread.h
//
// Cliff Dong
//


#include <sys/types.h>
#include <signal.h>
#include <vector>
#include <string>
#include <iostream>
#include <pthread.h>
#include <Poco/Process.h>
#include <Poco/Thread.h>
#include <Poco/Format.h>

#include <ProcessHandleThread.h>

// Constructor
ProcessHandleThread::ProcessHandleThread(int id):
    _id(id),
    _thread(Poco::format("processHandler_%d", id)),
    _pid(0),
    onStop(NULL)
{}

// Destructor
ProcessHandleThread::~ProcessHandleThread()
{
    if (_thread.isRunning()) {
        killProc();
    }
}

//---------------------------------------------------------------------------
//
//    Function Name: start
//    Description: Start thread and start process
//    Parameters:
//      command, process command
//      args, arguments vector
//      onStopPtr, onStop function pointer
//    Return Values:
//
//---------------------------------------------------------------------------
void ProcessHandleThread::start(const std::string& command,
    const std::vector<std::string>& args,
    onStopFunc onStopPtr)
{
    if (_thread.isRunning()) {
        std::cout << Poco::format("process %s id %d pid %d is already started",
                     _command, _id, _pid)
                  << std::endl;
        return;
    }

    _command = command;
    _args = args;
    onStop = onStopPtr;
    _thread.start(*this);
    // to avoid process exit internally(in this case thread.join() will not be called)
    pthread_detach(_thread.tid());
}

//---------------------------------------------------------------------------
//
//    Function Name: isRunning
//    Description: Test if the thread is running
//    Parameters:
//    Return Values: running - true, not running - false
//
//---------------------------------------------------------------------------
bool ProcessHandleThread::isRunning()
{
    return _thread.isRunning();
}

//---------------------------------------------------------------------------
//
//    Function Name: stop
//    Description: Stop process and thread will in turn be stopped
//    Parameters:
//    Return Values:
//
//---------------------------------------------------------------------------
void ProcessHandleThread::stop()
{
    if (_thread.isRunning()) {
        killProc();
    } else {
        std::cout << Poco::format("process %s id %d pid %d is already stopped",
                     _command, _id, _pid)
                  << std::endl;
        if (onStop) {
            onStop(_id);
        }
    }
}


//---------------------------------------------------------------------------
//
//    Function Name: killProc
//    Description: Kill process
//    Parameters:
//    Return Values:
//
//---------------------------------------------------------------------------
void ProcessHandleThread::killProc()
{
    Poco::Process::kill(_pid);
}

//---------------------------------------------------------------------------
//
//    Function Name: run
//    Description: thread run function
//    Parameters:
//    Return Values:
//
//---------------------------------------------------------------------------
void ProcessHandleThread::run()
{
    Poco::ProcessHandle ph = Poco::Process::launch(_command, _args);
    _pid = ph.id();

    std::vector<std::string>::iterator it;
    std::string args_str;
    for (it = _args.begin(); it != _args.end(); it++) {
        args_str += *it + " ";
    }
    if (_args.size() > 0) {
        args_str = args_str.substr(0, args_str.size() - 1);
    }

    std::cout << Poco::format("Start process \"%s %s\" pid %d",
                 _command, args_str, _pid)
              << std::endl;

    int ret = ph.wait();

    std::cout << Poco::format("process \"%s %s\" pid %d exit with code %d",
                 _command, args_str, _pid, ret)
              << std::endl;

    if (onStop) {
        onStop(_id);
    }
    _pid = 0;
}
