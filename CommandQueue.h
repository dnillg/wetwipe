#ifndef COMMANDQUEUE_H_
#define COMMANDQUEUE_H_

#include <Arduino.h>
#include <LinkedList.h>

class CommandQueue {
private:
	struct Command {
		unsigned long time;
		void (*func)(void*) = 0;
		void* param;
		bool deleteParam;
	};
	LinkedList<Command*> commands;
public:
	CommandQueue();
	virtual ~CommandQueue();
	void addCommand(unsigned long time, void (*func)(void*), void* param, bool deleteParam);
	void executeCommands(unsigned long till);
};

#endif /* COMMANDQUEUE_H_ */
