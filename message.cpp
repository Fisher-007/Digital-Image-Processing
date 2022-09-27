#include "message.h"


void StatusMessage(const string& message) {
	// 后续在参数与格式方面可以继续拓展
	cout << message << "\n";
}


void StatusMessageB(const string& message) {
	cout << message << "...";
}


void StatusMessageA(const string& message) {
	if (message == "")
		cout << "Succeed!" << "\n";
	else
		cout << message << "\n";
}


void Warning(const string& message) {
	cout << "Warning: " << message << "\n";
}


void ExitMessage(const string& message) {
	cout << "Error: " << message << "\n";
	exit(EXIT_FAILURE);
}