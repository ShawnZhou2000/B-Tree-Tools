#pragma once
#define MAX_STUDENT_NUM 1000000   // ����¼����
#define MAX_STRING_LENGTH 256  // �ַ�������� 
#define MAX_DISPLAY_LENGTH 20 // ȫ����ӡʱ�����ʾ����Ŀ
struct _info {
	char name[MAX_STRING_LENGTH];
	int chineseScore;
	int mathScore;
	int englishScore;
};

_info info[MAX_STUDENT_NUM];

int stuNum = 0;
