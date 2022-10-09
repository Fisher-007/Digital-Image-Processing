#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H


#include <iostream>
#include <string>
#include <iomanip>
#include <cstdarg>
using std::string;
using std::cout;
using std::cin;


// ��ӡһ��������Ϣ
void StatusMessage(const string& message);
// ��ӡ�¼����ǰ��Ϣ������ĩβ��...
void StatusMessageB(const string& message);
// ��ӡ�¼���ɺ���Ϣ����������ҪĬ��ΪSucceed��
void StatusMessageA(const string& message = "");

// ��ӡ������ʾ��Ϣ������ĩβ�ӣ�
void PrintHintMessageB(const string& message);
// ��ӡ������ʾ��Ϣ����������ҪĬ��Ϊ'\n'
void PrintHintMessageA(const string& message = "\n");

// ��ӡ������ʾ����ȡ������Ϣ
void SetArg();

template <typename T, typename... U>
void SetArg(T& first, U&...args) {
	cin >> first;
	SetArg(args...);
}

template <typename T, typename... U>
void InputMessage(const string& message, T& first, U&...args) {
	PrintHintMessageB(message);
	SetArg(first, args...);
	PrintHintMessageA();
}

// ��ӡ������Ϣ
void Warning(const string& message);
// ��ӡError��Ϣ���˳�
void ExitMessage(const string& message);


#endif