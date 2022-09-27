/*****************************************************************************
*  @file     message.h                                                       *
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