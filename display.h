#pragma once
#include <windows.h>
#include <cstdio>
#define TEXT_MAX 18
#define TIME_BREAK 100
#define TIME_BREAK_LONGER 200
#define TIME_BREAK_SHORTER 50
// 控制程序首次运行时的动画显示

const char* text[TEXT_MAX] = {					// 用于显示菜单的字符串数组
	"\n\n",
	"\t\t\t                         B-树索引查找工具\n",
	"\n",
	"\t\t\t-------------------------------------------------------------------------\n",
	"\t\t\t|\t\t\t系统主菜单选项列表\t\t\t\t|\n",
	"\t\t\t|\t\t\t1.  初始化B-树\t\t\t\t\t|\n",
	"\t\t\t|\t\t\t2.  销毁B-树\t\t\t\t\t|\n",
	"\t\t\t|\t\t\t3.  查看全部信息\t\t\t\t|\n",
	"\t\t\t|\t\t\t4.  按学号查询信息\t\t\t\t|\n",
	"\t\t\t|\t\t\tI.  导入数据表文件进行预处理\t\t\t|\n",
	"\t\t\t|\t\t\tL.  载入索引目录到B-树并挂载数据库\t\t|\n",
	"\t\t\t|\t\t\t0.  退出系统\t\t\t\t\t|\n",
	"\t\t\t-------------------------------------------------------------------------\n",
	"\n",
	"\n",
	"\n",
	"\n",
	"\t\t\t\t\t\t请输入对应的菜单序号: "
};

void initWindow();									// 初始化窗口
void slowDisplay(const char* p);					// 缓慢输出以达到动画效果的核心方法 
void intervalOutput(DWORD time, const char* text);	// 输出一行字符串便暂停某一时间 
void slowDisplayMenu(); 							// 以动画（缓慢输出）形式打印菜单 
void displayMenu();									// 直接打印菜单 

void initWindow() {
	system("title B-树索引查找工具 By Shawn Zhou"); // 设置程序标题
	system("color 9f");								// 设置控制台前景色和背景色
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准句柄，用于控制光标状态
	CONSOLE_CURSOR_INFO CursorInfo;					// 用于存储控制台光标信息
	GetConsoleCursorInfo(handle, &CursorInfo);		// 获取控制台光标信息
	CursorInfo.bVisible = 0;						// 隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);		// 设置控制台光标状态
	slowDisplayMenu();								// 第一次以动画形式显示菜单 
	CursorInfo.bVisible = 1;						// 恢复光标显示
	SetConsoleCursorInfo(handle, &CursorInfo);		// 设置控制台光标状态
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