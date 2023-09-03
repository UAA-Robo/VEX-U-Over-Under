#include <Logger.h>

Logger::Logger(Hardware* hardware, std::string file_name, std::vector<std::string> column_names) {
    hw = hardware;
    file = file_name;
    
    if (hw->brain.SDcard.isInserted()){
        data_log.open(file, std::ofstream::out | std::ofstream::trunc);
        
        //Label column names
        data_log << "secElapsed, ";
        for (int i = 0; i < column_names.size() - 1; i++) {
            data_log << column_names.at(i) << ", ";
        }
        data_log << column_names.at(column_names.size() - 1) << "\n";
        data_log.close();
    }
    
}


Logger::~Logger() {
    data_log.close();
}


void Logger::add_data(std::vector<double> row) {
    if (hw->brain.SDcard.isInserted()){
        data_log.open(file, std::ofstream::app);

        data_log << hw->brain.timer(vex::timeUnits::sec) << ", ";

        for (int i = 0; i < row.size() - 1; i++) {
            data_log << row.at(i) << ", ";
        }
        data_log << row.at(row.size() - 1) << "\n";
        data_log.close();
    }
}