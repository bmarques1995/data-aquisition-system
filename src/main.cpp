#include <cstdlib>
#include <iostream>
#include <utility>
#include <functional>
#include <thread>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include "SensorMock.hpp"
#include "RegisterReader.hpp"

int main(int argc, char* argv[])
{
    // if (argc != 2)
    // {
    //   std::cerr << "Usage: chat_server <port>\n";
    //   return 1;
    // }

    std::shared_ptr<std::mutex> sharedMutex;
    sharedMutex.reset(new std::mutex());
    std::shared_ptr<std::vector<SensorProject::LogRecord>> sharedBuffer;
    sharedBuffer.reset(new std::vector<SensorProject::LogRecord>());

    SensorProject::SensorMock mockedSensor("a5f87451a5f87451a5f87451a5f87451", 1, 2, 2, sharedBuffer, sharedMutex);
    SensorProject::RegisterReader registerReader(sharedBuffer, sharedMutex, "a5f87451a5f87451a5f87451a5f87451");
    std::thread* sensorThread;
    std::thread* bufferThread;
    sensorThread = new std::thread(std::bind(&SensorProject::SensorMock::SimSensor, &mockedSensor));
    bufferThread = new std::thread(std::bind(&SensorProject::RegisterReader::ReadRegisters, &registerReader));
    for(size_t i = 0; i < 50; ++i)
        mockedSensor.SimSensor();
    bufferThread->join();
    sensorThread->join();
    delete bufferThread;
    delete sensorThread;
}
