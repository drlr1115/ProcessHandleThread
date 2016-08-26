//
// File: ProcessHandleThread.h
//
// Cliff Dong
//


#if !defined(_PROCESS_HANDLE_THREAD_H)
#define _PROCESS_HANDLE_THREAD_H

#include <vector>
#include <string>
#include <Poco/Process.h>
#include <Poco/Thread.h>

typedef int (*onStopFunc)(int id);

class ProcessHandleThread : public Poco::Runnable
{
public:
    ProcessHandleThread(int id);
    ~ProcessHandleThread();
    void start(const std::string& command, const std::vector<std::string>& args, onStopFunc onStopPtr);
    bool isRunning();
    void stop();

private:
    int _id;
    Poco::Thread _thread;
    Poco::Process::PID _pid;
    std::string _command;
    std::vector<std::string> _args;
    onStopFunc onStop;
    void killProc();
    virtual void run();
};

#endif  //_PROCESS_HANDLE_THREAD_H
