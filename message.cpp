#include "message.h"


void StatusMessage(const string& message) {
	// �����ڲ������ʽ������Լ�����չ
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