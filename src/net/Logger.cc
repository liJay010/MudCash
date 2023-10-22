#include "Logger.h"
#include "Timestamp.h"
#include <thread>
#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

// 获取日志唯一的实例对象
Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}


Logger::Logger()
{
    std::thread writerThread([&](){
        for(;;)
        {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char file_name[256];
            std::string folderPath = "./log/";
            std::fstream _file;
            _file.open(folderPath, std::ios::in);
            if(!_file) 
            {
                int result = mkdir(folderPath.c_str(), 0777); // 第二个参数是文件夹的权限，这里使用最高权限0777

                if (result == 0) {
                    std::cout << "Folder created successfully." << std::endl;
                } else {
                    std::cout << "Failed to create folder." << std::endl;
                }
            }

            sprintf(file_name, "./log/%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            FILE *pf = fopen(file_name, "a+");
            if (pf == nullptr)
            {
                std::cout << "logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = Logger::instance().m_que.Pop();
            msg.append("\n");
            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });
    writerThread.detach();
}
// 设置日志级别
void Logger::setLogLevel(int level)
{
    logLevel_ = level;
}

// 写日志  [级别信息] time : msg
void Logger::log(std::string msg)
{
    std::string s;
    switch (logLevel_)
    {
    case INFO:
        s +=  "[INFO]";
        break;
    case ERROR:
        s +=  "[ERROR]";
        break;
    case FATAL:
        s +=  "[FATAL]";
        break;
    case DEBUG:
        s +=  "[DEBUG]";
        break;
    default:
        break;
    }

    // 打印时间和msg
    m_que.Push(s + Timestamp::now().toString() + ":" + msg);
    //std::cout << Timestamp::now().toString() << " : " << msg << std::endl;
}