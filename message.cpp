/*****************************************************************************
*  @file     message.cpp                                                       *
*  @brief    信息处理类                                                      *
*  @details  用于打印程序运行过程中需要展示的信息，包括警告和报错            *
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