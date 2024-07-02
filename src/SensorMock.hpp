#pragma once
#include <chrono>
#include <string>
#include <fstream>

namespace SensorProject
{
    struct LogRecord {
        const std::string sensor_id; // supondo um ID de sensor de até 32 caracteres
        std::time_t timestamp; // timestamp UNIX
        double value; // valor da leitura
        LogRecord(std::string_view id):
            sensor_id(id)
        {

        }
    };

    class SensorMock
    {
    public:
        SensorMock(std::string_view sensorID, size_t sensorPause, double sensorRange, double sensorOffset);
        ~SensorMock() = default;

        void SimSensor();
    private:
        LogRecord m_CurrentLog;
        size_t m_SensorPause; 
        double m_SensorRange;
        double m_SensorOffset;
        void GenRandomLog();
        void RecordLog();
    };
}
