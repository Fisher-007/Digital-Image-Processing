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


// 打印一般文字信息
void StatusMessage(const string& message);
// 打印事件完成前信息，文字末尾加...
void StatusMessageB(const string& message);
// 打印事件完成后信息，无特殊需要默认为Succeed！
void StatusMessageA(const string& message = "");

// 打印交互提示信息，文字末尾加：
void PrintHintMessageB(const string& message);
// 打印交互提示信息，无特殊需要默认为'\n'
void PrintHintMessageA(const string& message = "\n");

// 打印交互提示并提取输入信息
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

// 打印警告信息
void Warning(const string& message);
// 打印Error信息并退出
void ExitMessage(const string& message);


#endif