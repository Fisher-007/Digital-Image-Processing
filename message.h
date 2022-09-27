#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H


#include <iostream>
#include <string>
#include <iomanip>
using std::string;
using std::cout;


// ��ӡһ��������Ϣ
void StatusMessage(const string& message);
// ��ӡ�¼����ǰ��Ϣ������ĩβ��...
void StatusMessageB(const string& message);
// ��ӡ�¼���ɺ���Ϣ����������ҪĬ��ΪSucceed��
void StatusMessageA(const string& message = "");
// ��ӡ������Ϣ
void Warning(const string& message);
// ��ӡError��Ϣ���˳�
void ExitMessage(const string& message);


#endif