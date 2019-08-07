#include <iostream>
#include <signal.h>
#include <thread>
#include <future>
#include "Util/Logger.h"
#include "Util/RecycleResourcePool.h"
#include "V2xMsgQueue.h"
#include "V2xMsg.h"
#include "V2xApp.h"

using namespace std;
using namespace v2x;
using namespace toolkit;

int main()
{
    Logger::Instance().add(make_shared<ConsoleChannel>());
    Logger::Instance().setLevel(LDebug);

    // 加载配置
    SET.load();

    std:shared_ptr<V2xApp> app = std::make_shared<V2xApp>();
    app->run();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
