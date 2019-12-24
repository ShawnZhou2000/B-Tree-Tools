#pragma once
#define MAX_STUDENT_NUM 1000000   // 最大记录数量
#define MAX_STRING_LENGTH 256  // 字符串最长长度 
#define MAX_DISPLAY_LENGTH 20 // 全部打印时最多显示的条目
struct _info {
	char name[MAX_STRING_LENGTH];
	int chineseScore;
	int mathScore;
	int englishScore;
};

_info info[MAX_STUDENT_NUM];

int stuNum = 0;
