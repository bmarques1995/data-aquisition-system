#include "RegisterReader.hpp"
#include <iostream>

SensorProject::RegisterReader::RegisterReader(std::shared_ptr<std::vector<LogRecord>> sharedBuffer, std::shared_ptr<std::mutex> sharedMutex, std::string_view sensorID) :
    m_SharedBuffer(sharedBuffer), m_SharedMutex(sharedMutex), m_SensorID(sensorID)
{

}

void SensorProject::RegisterReader::ReadRegisters()
{
    SensorProject::LogRecord logRecord(m_SensorID);
    std::string SensorID;
    char sensorIDRead[33];
    sensorIDRead[32] = '\0';
    
    while(true)
    {
        auto recordStart = std::chrono::steady_clock::now();
        m_SharedMutex->lock();
        std::cout << "Elements: " << m_SharedBuffer->size() << "\n";
        while(m_SharedBuffer->size() > 0)
        {
            m_PrivateBuffer.push_back(m_SharedBuffer->back());
            m_SharedBuffer->pop_back();
        }
        m_SharedMutex->unlock();
        auto recordEnd = std::chrono::steady_clock::now();
        while(std::chrono::duration_cast<std::chrono::milliseconds>(recordEnd - recordStart).count() < (double)(5000))
            recordEnd = std::chrono::steady_clock::now();
    }
}