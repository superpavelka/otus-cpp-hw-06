#pragma once

#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>

enum class State
{
    None = 0,
    Static = 1,
    Dynamic = 2
};

class CommandHandler
{
public:

    CommandHandler(const size_t& batchSize, int bracketsSize = 0) : _batchSize(batchSize), _bracketsCount(bracketsSize){};
    void start();
    void process();

    void openLog();
    void writeLog(const std::string& log);
    void closeLog();

    bool checkSizeAndProcess();

private:
    const std::string startOfBlock = "{";
    const std::string endOfBlock = "}";

    std::ofstream _log;
    std::string _logName;
    State _state = State::None;
    size_t _batchSize;    
    std::queue<std::string> _commands;
    std::stack<std::string> _brackets;
    size_t _bracketsCount;

    bool isStartOfBlock(const std::string& cmd);
    bool isEndOfBlock(const std::string& cmd);
};
