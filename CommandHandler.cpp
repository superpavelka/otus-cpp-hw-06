#include "CommandHandler.h"
#include <sstream>

void CommandHandler::start()
{
    std::string cmd;

    while (std::getline(std::cin, cmd))
    {
        if (isStartOfBlock(cmd))
        {            
            _brackets.push(startOfBlock);
            openLog();

            if (_state == State::None)
            {
                _state = State::Dynamic;
            }

            if (_state == State::Static)
            {
                process();
                _state = State::Dynamic;
            } 

        }
        else if (isEndOfBlock(cmd))
        {
            if (_state == State::None)
            {
                _commands.push(endOfBlock);
            }

            if (_state == State::Static)
            {
                _commands.push(endOfBlock);
                checkSizeAndProcess();
            }

            if (_state == State::Dynamic)
            {
                _brackets.pop();
                if (_brackets.size() == 0)
                {
                    process();
                    _state = State::None;
                }
            }   
        }
        else 
        {
            if (_state == State::None)
            {
                openLog();
                _commands.push(cmd);
                
                if (!checkSizeAndProcess())
                    _state = State::Static;
            }
            else if (_state == State::Static)
            {
                _commands.push(cmd);
                checkSizeAndProcess();
            }
            else
                _commands.push(cmd);

        }
    }

    if (_state == State::Static)
        process();
    else if (_state == State::Dynamic)
        closeLog();
}

void CommandHandler::process()
{
    std::ostringstream oss;
    std::ostream& os = oss;

    if (!_commands.empty()) 
    {
        os << "bulk: ";
        while (!_commands.empty()) 
        {
            os << _commands.front();
            _commands.pop();
            if (!_commands.empty())
                os << ", ";
        }
        os << std::endl;
    }

    writeLog(oss.str());
    std::cout << oss.str();
}

void CommandHandler::openLog()
{
    auto t = time(nullptr);
    std::ostringstream oss;
    std::ostream& os = oss;
    os << "bulk" << t << ".log";
    _logName = oss.str();
}

void CommandHandler::closeLog()
{
    if (_log.is_open())
        _log.close();
}

void CommandHandler::writeLog(const std::string& log)
{
    _log.open(_logName);
    if (_log.is_open()) 
        _log << log;
    else 
        throw "Error! Unable to open file ";    
}

bool CommandHandler::isStartOfBlock(const std::string& cmd)
{
    return (cmd.size() == 1 && cmd == startOfBlock) ? true : false;
}

bool CommandHandler::isEndOfBlock(const std::string& cmd)
{
    return (cmd.size() == 1 && cmd == endOfBlock) ? true : false;
}

bool CommandHandler::checkSizeAndProcess()
{
    if (_commands.size() - _bracketsCount == _batchSize)
    {
        process();
        return true;
    }

    return false;
}