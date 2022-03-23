/*
* ChainOfResponsibility Pattern 责任链模式
*  行为型模式
* 使用场景: 日志系统
* 优点: 
* 缺点: 
*/

// #include <thread>
// #include <memory>
#include <iostream>
#include <vector>

using namespace std;

class AbstractLogger{
public:
	enum log_level{
		ERROR     = 3,
		WARNING   = 4,
		INFO      = 5,
	};

    void setNextLogger(AbstractLogger* next) {
    	this->next_logger = next;
    }

    void logMessage(int level, string message) {
    	if(this->level >= level) {
    		write(message);
    	}
    	else{
    		if(next_logger != NULL) {
        		next_logger->logMessage(level, message);
        	}
    	}
    	
    }

protected:
	virtual void write(string message) {}

protected:
	int level;
    AbstractLogger* next_logger;
};


class ErrorLogger:public AbstractLogger{
public:
	ErrorLogger(int level) {
		this->level = level;
	}
protected:
	void write(string message) {
		std::cout<<" ERROR: "<<message<<"\n";
	}
};

class WarningLogger:public AbstractLogger{
public:
	WarningLogger(int level) {
		this->level = level;
	}
protected:
	void write(string message) {
		std::cout<<" WARNING: "<<message<<"\n";
	}
};

class InfoLogger:public AbstractLogger{
public:
	InfoLogger(int level) {
		this->level = level;
	}
protected:
	void write(string message) {
		std::cout<<" INFO: "<<message<<"\n";
	}
};

static AbstractLogger* getChainOfLoggers() {
	AbstractLogger* error_logger = new ErrorLogger(AbstractLogger::ERROR);
	AbstractLogger* warning_logger = new WarningLogger(AbstractLogger::WARNING);
	AbstractLogger* info_logger = new InfoLogger(AbstractLogger::INFO);

    error_logger->setNextLogger(warning_logger);
    warning_logger->setNextLogger(info_logger);

    //info_logger->setNextLogger(warning_logger);
    // warning_logger->setNextLogger(error_logger);
	return error_logger;
}

int main() {
	AbstractLogger* logger = getChainOfLoggers();
	logger->logMessage(5, "level 5");
	logger->logMessage(4, "level 4");
	logger->logMessage(3, "level 3");
}


/* 输出
* INFO: level 5
* WARNING: level 4
* ERROR: level 3
*/
