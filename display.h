#pragma once
#include <windows.h>
#include <cstdio>
#define TEXT_MAX 18
#define TIME_BREAK 100
#define TIME_BREAK_LONGER 200
#define TIME_BREAK_SHORTER 50
// ���Ƴ����״�����ʱ�Ķ�����ʾ

const char* text[TEXT_MAX] = {					// ������ʾ�˵����ַ�������
	"\n\n",
	"\t\t\t                         B-���������ҹ���\n",
	"\n",
	"\t\t\t-------------------------------------------------------------------------\n",
	"\t\t\t|\t\t\tϵͳ���˵�ѡ���б�\t\t\t\t|\n",
	"\t\t\t|\t\t\t1.  ��ʼ��B-��\t\t\t\t\t|\n",
	"\t\t\t|\t\t\t2.  ����B-��\t\t\t\t\t|\n",
	"\t\t\t|\t\t\t3.  �鿴ȫ����Ϣ\t\t\t\t|\n",
	"\t\t\t|\t\t\t4.  ��ѧ�Ų�ѯ��Ϣ\t\t\t\t|\n",
	"\t\t\t|\t\t\tI.  �������ݱ��ļ�����Ԥ����\t\t\t|\n",
	"\t\t\t|\t\t\tL.  ��������Ŀ¼��B-�����������ݿ�\t\t|\n",
	"\t\t\t|\t\t\t0.  �˳�ϵͳ\t\t\t\t\t|\n",
	"\t\t\t-------------------------------------------------------------------------\n",
	"\n",
	"\n",
	"\n",
	"\n",
	"\t\t\t\t\t\t�������Ӧ�Ĳ˵����: "
};

void initWindow();									// ��ʼ������
void slowDisplay(const char* p);					// ��������Դﵽ����Ч���ĺ��ķ��� 
void intervalOutput(DWORD time, const char* text);	// ���һ���ַ�������ͣĳһʱ�� 
void slowDisplayMenu(); 							// �Զ����������������ʽ��ӡ�˵� 
void displayMenu();									// ֱ�Ӵ�ӡ�˵� 

void initWindow() {
	system("title B-���������ҹ��� By Shawn Zhou"); // ���ó������
	system("color 9f");								// ���ÿ���̨ǰ��ɫ�ͱ���ɫ
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ��׼��������ڿ��ƹ��״̬
	CONSOLE_CURSOR_INFO CursorInfo;					// ���ڴ洢����̨�����Ϣ
	GetConsoleCursorInfo(handle, &CursorInfo);		// ��ȡ����̨�����Ϣ
	CursorInfo.bVisible = 0;						// ���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);		// ���ÿ���̨���״̬
	slowDisplayMenu();								// ��һ���Զ�����ʽ��ʾ�˵� 
	CursorInfo.bVisible = 1;						// �ָ������ʾ
	SetConsoleCursorInfo(handle, &CursorInfo);		// ���ÿ���̨���״̬
}

void slowDisplay(const char* p) {
	while (1) {
		if (*p != 0)
			cout << (char)* p++;
		else
			break;
		Sleep(TIME_BREAK_SHORTER);
	}
}

void intervalOutput(DWORD time, const char* text) {
	cout << text;
	Sleep(time);
}

void slowDisplayMenu() {
	for (int index = 0; index < TEXT_MAX; index++) {
		if (index == 0)
			cout << text[index];
		else if (index == 1)
			slowDisplay(text[index]);
		else
			intervalOutput(TIME_BREAK_LONGER, text[index]);
	}
}

void displayMenu() {
	for (int index = 0; index < TEXT_MAX; index++) {
		cout << text[index];
	}
}