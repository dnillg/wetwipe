#include "CommandQueue.h"

CommandQueue::CommandQueue() {
	// TODO Auto-generated constructor stub

}

CommandQueue::~CommandQueue() {
	// TODO Auto-generated destructor stub
}

//void CommandQueue::executeCommands(unsigned long till) {
//	Command* command = 0;
//	boolean moreAvailable = true;
//	do {
//		if (commands.size() == 0) {
//			moreAvailable = false;
//			break;
//		}
//		Command* cmd = commands.get(0);
//		if (cmd->time <= till && abs(cmd->time - till) < (2<<30)) {
//			cmd->func(cmd->param);
//			if (cmd->deleteParam) {
//				delete cmd->param;
//			}
//			delete commands.pop();
//		} else {
//			moreAvailable = false;
//		}
//	} while (moreAvailable);
//}
//
//void CommandQueue::addCommand(unsigned long time, void (*func)(void*), void* param = 0, bool deleteParam = false) {
//	Command* cmd = new Command();
//	cmd->time = time;
//	cmd->func = func;
//	cmd->param = param;
//	cmd->deleteParam = deleteParam;
//	commands.add(cmd);
//}
//
