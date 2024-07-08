#pragma once
#include "SensorMock.hpp"
#include <vector>

namespace SensorProject
{
    class RegisterReader
    {
    public:
        RegisterReader(std::shared_ptr<std::vector<LogRecord>> sharedBuffer, std::shared_ptr<std::mutex> sharedMutex, std::string_view sensorID);
        ~RegisterReader() = default;

        void ReadRegisters();
    private:

        void SerializeRegisters();
        
        std::shared_ptr<std::vector<LogRecord>> m_SharedBuffer;
        std::shared_ptr<std::mutex> m_SharedMutex;
        std::string m_SensorID;
        std::vector<LogRecord> m_PrivateBuffer;
    };
}