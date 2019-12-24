/*
	���ݽṹ�γ���� BTree�������
*/
#include <iostream>
#include <cstdio>
#include <ctime>
#include <Windows.h>
#include "BTree.h"
#include "display.h"
#include "student.h"


using namespace std;

// �����ݽṹB-��
BTree t;

bool isActivated = false;
// �Ƿ��Ѵ���һ�ÿ��õ�B��
// ��B��δ����ʼ���������٣�ֵΪfalse
// ��B���ѱ���ʼ����ֵΪtrue


const char* fileIdentifierText = "BTREEDATA";		// ϵͳ���ļ���ʶ��

void initBtree();									// ��ʼ��B-��
void destroyBtree();								// ����B-��
void listRecord();									// �鿴ȫ����¼
void searchRecord();								// ����һ����¼ 
void importDataBase();								// �������ݱ��ļ�����Ԥ����
void loadDataToBtree();								// ��������Ŀ¼�����ݱ�B-��
void exitProgram();									// �˳����� 
void getUserInput();								// ��ȡ�û����� 

int main() {
	initWindow();
	while (1) {
		getUserInput();								// ��ȡ���ж��û������� 
		system("cls");								// ��������������׸��
		displayMenu();								// �˺����ʾΪֱ����ʾ 
	}
	return 0;
}

void initBtree() {
	system("cls");
	cout << "\t\t\t---------------------------------------------------------------" << endl;
	cout << "\n\n\t\t\t\t\t\t1.  ��ʼ��B-��\t\t\t\t" << endl;
	if (isActivated) {			// �ж��Ƿ��Ѿ���ʼ��
		cout << "\n\n\t\t\t\t\tB���Ѿ�����ʼ����" << endl;
		cout << "\n\n\t\t\t---------------------------------------------------------------" << endl;
	}
	else {
		int minDeg;
		cout << "\n\n\t\t\t\t\t������B-������С�ȣ�";
		cin >> minDeg;
		
		if (minDeg < 3) {
			cout << "\n\n\t\t\t\t\t��С�Ȳ����Ե���3��" << endl;
			cout << "\n\n\t\t\t\t\t�ѽ���С����Ϊ3��" << endl;
			minDeg = 3;
		}
		t = t.InitBTree(minDeg);
		isActivated = true;
		cout << "\n\n\t\t\t\t\t��ʼ����ɣ�" << endl;
		cout << "\n\n\t\t\t---------------------------------------------------------------" << endl;
	}
	system("pause");					// �ȴ��û�����������ټ������г����²���׸��
}



void destroyBtree() {
	system("cls");
	cout << "\t\t\t---------------------------------------------------------------" << endl;
	cout << "\n\n\t\t\t\t\t\t2.  ����B-��\t\t\t\t" << endl;
	if (isActivated) {
		cout << "\n\t\t\t\t��ᵼ�¶�ʧB-����ȫ�������ݣ�ȷ��Ҫ��ô����" << endl;
		cout << "\n\t\t\t\t����y������ȷ����ô��������������ȡ���˲���:";
		char choice;
		cin >> choice;
		if (choice == 'y') {
			t.TraverseSaveBTree();
			t.DestroyBTree();
			stuNum = 0;
			isActivated = false;
			cout << "\n\t\t\t\tB-���ѱ�����" << endl;
		}
		else {
			cout << "\n\t\t\t\t�û�ȡ��������B-���Ĳ���" << endl;
		}
	}
	else {
		cout << "\n\t\t\t\tB-��δ��ʼ����" << endl;
	}
	
	
	cout << "\n\n\n\t\t\t---------------------------------------------------------------" << endl;
	system("pause");
}

void listRecord() {
	system("cls");
	cout << "\t\t\t---------------------------------------------------------------" << endl;
	cout << "\n\n\t\t\t\t\t\t3.  �鿴ȫ����Ϣ\t\t\t\t" << endl;
	if (isActivated) {
		time_t nowTime = time(0);

		if (stuNum >= MAX_DISPLAY_LENGTH) {
			FILE* file = NULL;										// �ļ�ָ��
			char rout[MAX_STRING_LENGTH] = "D:\\BTreeTest\\listinfo\\";	// ·����
			char fileName[MAX_STRING_LENGTH];				// �ļ���
			strftime(fileName, sizeof(fileName), "listinfo-%Y-%m-%d %H-%M-%S.txt", localtime(&nowTime));
			cout << "\n\t\t\t��¼���࣬�޷�����Ļ��ӡ����鿴" << fileName << "�ļ��е����ݡ�" << endl;
			file = fopen(strcat(rout, fileName), "w");
			t.TraverseBTree(file);
			fclose(file);
		}
		else {
			cout << "\n\t\t\t\t\t  һ����" << stuNum << "����¼����ϸ��Ϣ����:\n" << endl;
			cout << "\t\t\tѧ��\t  \t  ����  \t���ĳɼ�\t��ѧ�ɼ�\tӢ��ɼ�\t\n" << endl;
			t.TraverseBTree();
		}
	}
	else {
		cout << "\n\t\t\t\tB-��δ��ʼ����" << endl;
	}
	
	
	cout << "\n\n\n\t\t\t---------------------------------------------------------------" << endl;
	system("pause");
}

void searchRecord() {
	system("cls");
	printf("\t\t\t---------------------------------------------------------------\n");
	printf("\n\n\t\t\t\t\t\t4.  ��ѧ�Ų�ѯ��Ϣ\t\t\t\t\n");
	if (isActivated) {
		long long int searchID;
		cout << "\n\t\t\t\t\t������Ҫ��ѯ��ѧ�ţ�";
		cin >> searchID;
		BTreeNode* des = t.SearchBTree(searchID);
		if (des == NULL) {
			cout << "\n\t\t\t\tδ���ҵ�ѧ��Ϊ" << searchID << "��ͬѧ����Ϣ" << endl;
		}
		else {
			int index = des->getIndex(searchID);
			cout << "\t\t\tѧ��\t  \t  ����  \t���ĳɼ�\t��ѧ�ɼ�\tӢ��ɼ�\t\n" << endl;
			printf("\t\t\t%5lld\t%6s\t\t%4d\t\t%4d\t\t%4d\t\n", searchID, info[index].name, info[index].chineseScore, info[index].mathScore, info[index].englishScore);
		}
	}
	else {
		cout << "\n\t\t\t\tB-��δ��ʼ����" << endl;
	}
	
	printf("\n\n\n\t\t\t---------------------------------------------------------------\n");
	system("pause");
}

void importDataBase() {
	system("cls");
	printf("\t\t\t---------------------------------------------------------------\n");
	printf("\n\n\t\t\t\t\t\tI.  �������ݱ��ļ�����Ԥ����\t\t\t\t\n");
	if (isActivated) {
		FILE* file = NULL;										// �ļ�ָ��
		char rout[MAX_STRING_LENGTH] = "D:\\BTreeTest\\";	// ·����
		char fileName[MAX_STRING_LENGTH];				// �ļ���
		printf("\n\t\t\t\t������Ҫ��������ݱ��ļ�����");
		cin >> fileName;
		strcat(rout, fileName);
		file = fopen(rout, "r");
		if (file == NULL) {						// �ж��ļ��Ƿ���Դ򿪣���ֹ����
			printf("\t\t\t\t�Ҳ���%s�ļ���", rout);
		}
		else {
			char isLegal[MAX_STRING_LENGTH];	// У�����ݴ���
			/*
			����У�����ԭ�����£�����ȡ��ϵͳ�ڿ�ʶ����ļ����ڶ���ʱ��ͳ����쳣�����³������
			ʹ�ñ�ϵͳ������ļ�ͷ���Դ�У���룬������һ���̶��ϱ�֤ͨ���ļ�����ĺϷ��ԡ�
			*/
			fscanf(file, "%s", isLegal);
			if (strcmp(isLegal, fileIdentifierText) != 0) {
				printf("\t\t\t��ʧ�ܣ��ļ�У����������Ƿ�Ϊ�Ϸ���ϵͳ�����ļ���\n");
			}
			else {
				// �ٽ���һ���ļ���init�ļ��У���ѧ�ź�����д��ȥ
				// ��������ֵ����Ϣ�浽������
				char tName[MAX_STRING_LENGTH];
				int tChineseScore, tMathScore, tEnglishScore;
				long long int tStuID;
				char indexPath[MAX_STRING_LENGTH] = "D:\\BTreeTest\\init\\nowIndex.txt";	// �����ļ�·����
				char dataPath[MAX_STRING_LENGTH] = "D:\\BTreeTest\\data\\nowData.txt";	// �����ļ�·����
				FILE* indexFile = fopen(indexPath, "w");
				FILE* dataFile = fopen(dataPath, "w");
				fprintf(indexFile, fileIdentifierText);
				fprintf(indexFile, "\n");
				fprintf(dataFile, fileIdentifierText);
				fprintf(dataFile, "\n");
				stuNum = 0;
				while (fscanf(file, "%lld %s %d %d %d", &tStuID, tName, &tChineseScore, &tMathScore, &tEnglishScore) == 5) {
					fprintf(indexFile, "%lld %d\n", tStuID, stuNum);
					fprintf(dataFile, "%d %s %d %d %d\n", stuNum, tName, tChineseScore, tMathScore, tEnglishScore);
					stuNum++;
				}
				fclose(file);
				fclose(indexFile);
				fclose(dataFile);
				printf("\t\t\t\t�Ѵ�%s�ﵼ��ѧ����Ϣ��ϵͳ\n", rout);
			}
		}
	}
	else {
		cout << "\n\t\t\t\tB-��δ��ʼ����" << endl;
	}
	printf("\n\n\n\t\t\t---------------------------------------------------------------\n");
	system("pause");
}

void loadDataToBtree() {
	system("cls");
	printf("\t\t\t---------------------------------------------------------------\n");
	printf("\n\n\t\t\t\t\t\tL.  ��������Ŀ¼��B-�����������ݿ�\t\t\t\t\n");
	
	if (isActivated) {
		FILE* file;

		int checkNum1 = 0, checkNum2 = 0;

		char rout[MAX_STRING_LENGTH] = "D:\\BTreeTest\\init\\";
		char fileName[MAX_STRING_LENGTH];
		printf("\t\t\t\t������Ҫ����������ļ�����");
		cin >> fileName;
		file = fopen(strcat(rout, fileName), "r");
		if (file == NULL) {						// �ж��ļ��Ƿ���Դ򿪣���ֹ����
			printf("\t\t\t\t�Ҳ���%s�ļ���", rout);
		}
		else {
			char isLegal[MAX_STRING_LENGTH];	// У�����ݴ���
			/*
			����У�����ԭ�����£�����ȡ��ϵͳ�ڿ�ʶ����ļ����ڶ���ʱ��ͳ����쳣�����³������
			ʹ�ñ�ϵͳ������ļ�ͷ���Դ�У���룬������һ���̶��ϱ�֤ͨ���ļ�����ĺϷ��ԡ�
			*/
			fscanf(file, "%s", isLegal);
			if (strcmp(isLegal, fileIdentifierText) != 0) {
				printf("\t\t\t��ʧ�ܣ��ļ�У����������Ƿ�Ϊ�Ϸ���ϵͳ�����ļ���\n");
			}
			else {
				long long int tStuID;
				int tIndex;
				while (fscanf(file, "%lld %d", &tStuID, &tIndex) == 2) {
					t.InsertBTree(tStuID, tIndex);
					checkNum1++;
				}
				printf("\t\t\t\t�Ѵ�%s�ﵼ�������ļ���B-��\n", rout);
			}
			fclose(file);
		}

		printf("\t\t\t\t������Ҫ��������ݿ��ļ�����");
		char rdata[MAX_STRING_LENGTH] = "D:\\BTreeTest\\data\\";
		cin >> fileName;
		file = fopen(strcat(rdata, fileName), "r");
		if (file == NULL) {						// �ж��ļ��Ƿ���Դ򿪣���ֹ����
			printf("\t\t\t\t�Ҳ���%s�ļ���", rdata);
		}
		else {
			char isLegal[MAX_STRING_LENGTH];	// У�����ݴ���
			fscanf(file, "%s", isLegal);
			if (strcmp(isLegal, fileIdentifierText) != 0) {
				printf("\t\t\t��ʧ�ܣ��ļ�У����������Ƿ�Ϊ�Ϸ���ϵͳ�����ļ���\n");
			}
			else {
				char tName[MAX_STRING_LENGTH] = "";
				int tChineseScore, tMathScore, tEnglishScore;
				int tIndex;
				while (fscanf(file, "%d %s %d %d %d", &tIndex, tName, &tChineseScore, &tMathScore, &tEnglishScore) == 5) {

					strcpy(info[tIndex].name, tName);
					info[tIndex].chineseScore = tChineseScore;
					info[tIndex].mathScore = tMathScore;
					info[tIndex].englishScore = tEnglishScore;
					checkNum2++;
				}
				printf("\t\t\t\t�Ѵ�%s�ﵼ�����ݿ��ļ���B-��ʹ��\n", rout);
			}
			fclose(file);
		}

		if (checkNum1 != checkNum2) {
			cout << "\t\t\t���ݿ���Ŀ��������Ŀ��һ�£�ִ�в�ѯ�������ܻ����쳣�����飡" << endl;
		}
		else
			stuNum = checkNum1;
	}
	else {
		cout << "\n\t\t\t\tB-��δ��ʼ����" << endl;
	}

	printf("\n\n\n\t\t\t---------------------------------------------------------------\n");
	system("pause");
}

void exitProgram() {
	printf("\t\t\t\t\t��лʹ�ñ����ҹ��ߣ��밴������˳�...\n");
	system("pause");
	exit(0);
}

void getUserInput() {
	char ch;
	while (1) {
		// ������ַ�ǰ���������ܿ������в����'\n'�����Ӱ����룬���Բ��ô��ַ������յ�������ǰȫ����'\n'��
		cin >> ch;
		if (ch == '\n')
			continue;
		else
			break;
	}
	while ((ch < '0' || ch > '4') && ch != 'i' && ch != 'I' && ch != 'l' && ch != 'L') {
		printf("\t\t\t\t������������������: ");
		while (1) {
			cin >> ch;
			if (ch == '\n')
				continue;
			else
				break;
		}
	}
	switch (ch) {
	case '1':
		initBtree();
		break;
	case '2':
		destroyBtree();
		break;
	case '3':
		listRecord();
		break;
	case '4':
		searchRecord();
		break;
	case 'i':
		importDataBase();
		break;
	case 'I':
		importDataBase();
		break;
	case 'l':
		loadDataToBtree();
		break;
	case 'L':
		loadDataToBtree();
		break;
	case '0':
		exitProgram();
		break;
	}
}

