#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H


#include <iostream>
#include <string>
#include <iomanip>
using std::string;
using std::cout;


// 打印一般文字信息
void StatusMessage(const string& message);
// 打印事件完成前信息，文字末尾加...
void StatusMessageB(const string& message);
// 打印事件完成后信息，无特殊需要默认为Succeed！
void StatusMessageA(const string& message = "");
// 打印警告信息
void Warning(const string& message);
// 打印Error信息并退出
void ExitMessage(const string& message);


#endif