/*****************************************************************************
*  @file     message.cpp                                                       *
*  @brief    ��Ϣ������                                                      *
*  @details  ���ڴ�ӡ�������й�������Ҫչʾ����Ϣ����������ͱ���            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.0                                                             *
*  @date     2022/07/30                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/09/27 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
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