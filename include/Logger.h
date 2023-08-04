#pragma once
#include <Hardware.h>
#include <vex.h>

class Logger {
public:
    Logger(Hardware* hardware, std::string csvFileName, std::vector<std::string> cols);
    ~Logger();
    void addData(std::vector<double> row);


private:
    Hardware* hw;
    std::ofstream dataLog;
    std::string file;
};

