#include "RegisterReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

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
        for (size_t i = 0; i < m_SharedBuffer->size(); i++)
        {
            m_PrivateBuffer.push_back((*m_SharedBuffer)[i]);
        }
        m_SharedBuffer->clear();
        m_SharedMutex->unlock();
        SerializeRegisters();
        auto recordEnd = std::chrono::steady_clock::now();
        while(std::chrono::duration_cast<std::chrono::milliseconds>(recordEnd - recordStart).count() < (double)(5000))
            recordEnd = std::chrono::steady_clock::now();
    }
}

void SensorProject::RegisterReader::SerializeRegisters()
{
    std::stringstream streamBuffer;
    streamBuffer << m_PrivateBuffer.size() << ";";
    for (size_t i = 0; i < m_PrivateBuffer.size(); i++)
    {
        
        std::time_t tt = std::chrono::system_clock::to_time_t (m_PrivateBuffer[i].timestamp);

        struct std::tm * ptm = std::localtime(&tt);
        streamBuffer << std::put_time(ptm, "%Y-%m-%dT%H:%M:%S") << "|" << m_PrivateBuffer[i].value ;
        (i != (m_PrivateBuffer.size() - 1)) ? streamBuffer << ";" : streamBuffer << "\r\n";
    }
    m_PrivateBuffer.clear();
    std::string castedRegister = streamBuffer.str();
    std::cout << castedRegister;
}