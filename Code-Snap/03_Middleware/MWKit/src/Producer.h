//
// Created by root on 19-7-12.
//

#ifndef CV2X_PRODUCER_H
#define CV2X_PRODUCER_H
#include "Thread/ThreadPool.h"


using namespace toolkit;

namespace mwkit
{


class Producer
{
public:
    Producer();
    virtual ~Producer();
    void start();
    virtual void run() = 0;

private:
    ThreadPool::Ptr _thread;

};
}

#endif //CV2X_PRODUCER_H
