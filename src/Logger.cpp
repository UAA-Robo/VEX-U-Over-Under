#include <Logger.h>

Logger::Logger(Hardware* hardware, std::string fileName, std::vector<std::string> cols) {
    hw = hardware;
    file = fileName;
    
    if (hw->brain.SDcard.isInserted()){
        dataLog.open(file, std::ofstream::out | std::ofstream::trunc);
        
        //Label column names
        dataLog << "secElapsed, ";
        for (int i = 0; i < cols.size() - 1; i++) {
            dataLog << cols.at(i) << ", ";
        }
        dataLog << cols.at(cols.size() - 1) << "\n";
        dataLog.close();
    }
    
}
Logger::~Logger() {
    dataLog.close();
}

void Logger::addData(std::vector<double> row) {
    if (hw->brain.SDcard.isInserted()){
        dataLog.open(file, std::ofstream::app);

        dataLog << hw->brain.timer(vex::timeUnits::sec) << ", ";

        for (int i = 0; i < row.size() - 1; i++) {
            dataLog << row.at(i) << ", ";
        }
        dataLog << row.at(row.size() - 1) << "\n";
        dataLog.close();
    }
}