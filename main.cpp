/*
	数据结构课程设计 BTree工具设计
*/
#include <iostream>
#include <cstdio>
#include <ctime>
#include <Windows.h>
#include "BTree.h"
#include "display.h"
#include "student.h"


using namespace std;

// 主数据结构B-树
BTree t;

bool isActivated = false;
// 是否已存在一棵可用的B树
// 当B树未被初始化或已销毁，值为false
// 当B树已被初始化，值为true


const char* fileIdentifierText = "BTREEDATA";		// 系统用文件标识符

void initBtree();									// 初始化B-树
void destroyBtree();								// 销毁B-树
void listRecord();									// 查看全部记录
void searchRecord();								// 查找一条记录 
void importDataBase();								// 导入数据表文件进行预处理
void loadDataToBtree();								// 载入索引目录和数据表到B-树
void exitProgram();									// 退出程序 
void getUserInput();								// 获取用户输入 

int main() {
	initWindow();
	while (1) {
		getUserInput();								// 获取并判断用户的输入 
		system("cls");								// 清屏操作，后不再赘述
		displayMenu();								// 此后的显示为直接显示 
	}
	return 0;
}

void initBtree() {
	system("cls");
	cout << "\t\t\t---------------------------------------------------------------" << endl;
	cout << "\n\n\t\t\t\t\t\t1.  初始化B-树\t\t\t\t" << endl;
	if (isActivated) {			// 判断是否已经初始化
		cout << "\n\n\t\t\t\t\tB树已经被初始化！" << endl;
		cout << "\n\n\t\t\t---------------------------------------------------------------" << endl;
	}
	else {
		int minDeg;
		cout << "\n\n\t\t\t\t\t请输入B-树的最小度：";
		cin >> minDeg;
		
		if (minDeg < 3) {
			cout << "\n\n\t\t\t\t\t最小度不可以低于3！" << endl;
			cout << "\n\n\t\t\t\t\t已将最小度设为3！" << endl;
			minDeg = 3;
		}
		t = t.InitBTree(minDeg);
		isActivated = true;
		cout << "\n\n\t\t\t\t\t初始化完成！" << endl;
		cout << "\n\n\t\t\t---------------------------------------------------------------" << endl;
	}
	system("pause");					// 等待用户输入任意键再继续运行程序，下不再赘述
}



void destroyBtree() {
	system("cls");
	cout << "\t\t\t---------------------------------------------------------------" << endl;
	cout << "\n\n\t\t\t\t\t\t2.  销毁B-树\t\t\t\t" << endl;
	if (isActivated) {
		cout << "\n\t\t\t\t这会导致丢失B-树中全部的数据！确定要这么做吗？" << endl;
		cout << "\n\t\t\t\t输入y代表你确定这么做，输入其他则取消此操作:";
		char choice;
		cin >> choice;
		if (choice == 'y') {
			t.TraverseSaveBTree();
			t.DestroyBTree();
			stuNum = 0;
			isActivated = false;
			cout << "\n\t\t\t\tB-树已被销毁" << endl;
		}
		else {
			cout << "\n\t\t\t\t用户取消了销毁B-树的操作" << endl;
		}
	}
	else {
		cout << "\n\t\t\t\tB-树未初始化！" << endl;
	}
	
	
	cout << "\n\n\n\t\t\t---------------------------------------------------------------" << endl;
	system("pause");
}

void listRecord() {
	system("cls");
	cout << "\t\t\t---------------------------------------------------------------" << endl;
	cout << "\n\n\t\t\t\t\t\t3.  查看全部信息\t\t\t\t" << endl;
	if (isActivated) {
		time_t nowTime = time(0);

		if (stuNum >= MAX_DISPLAY_LENGTH) {
			FILE* file = NULL;										// 文件指针
			char rout[MAX_STRING_LENGTH] = "D:\\BTreeTest\\listinfo\\";	// 路径名
			char fileName[MAX_STRING_LENGTH];				// 文件名
			strftime(fileName, sizeof(fileName), "listinfo-%Y-%m-%d %H-%M-%S.txt", localtime(&nowTime));
			cout << "\n\t\t\t记录过多，无法在屏幕打印，请查看" << fileName << "文件中的内容。" << endl;
			file = fopen(strcat(rout, fileName), "w");
			t.TraverseBTree(file);
			fclose(file);
		}
		else {
			cout << "\n\t\t\t\t\t  一共有" << stuNum << "条记录，详细信息如下:\n" << endl;
			cout << "\t\t\t学号\t  \t  姓名  \t语文成绩\t数学成绩\t英语成绩\t\n" << endl;
			t.TraverseBTree();
		}
	}
	else {
		cout << "\n\t\t\t\tB-树未初始化！" << endl;
	}
	
	
	cout << "\n\n\n\t\t\t---------------------------------------------------------------" << endl;
	system("pause");
}

void searchRecord() {
	system("cls");
	printf("\t\t\t---------------------------------------------------------------\n");
	printf("\n\n\t\t\t\t\t\t4.  按学号查询信息\t\t\t\t\n");
	if (isActivated) {
		long long int searchID;
		cout << "\n\t\t\t\t\t请输入要查询的学号：";
		cin >> searchID;
		BTreeNode* des = t.SearchBTree(searchID);
		if (des == NULL) {
			cout << "\n\t\t\t\t未能找到学号为" << searchID << "的同学的信息" << endl;
		}
		else {
			int index = des->getIndex(searchID);
			cout << "\t\t\t学号\t  \t  姓名  \t语文成绩\t数学成绩\t英语成绩\t\n" << endl;
			printf("\t\t\t%5lld\t%6s\t\t%4d\t\t%4d\t\t%4d\t\n", searchID, info[index].name, info[index].chineseScore, info[index].mathScore, info[index].englishScore);
		}
	}
	else {
		cout << "\n\t\t\t\tB-树未初始化！" << endl;
	}
	
	printf("\n\n\n\t\t\t---------------------------------------------------------------\n");
	system("pause");
}

void importDataBase() {
	system("cls");
	printf("\t\t\t---------------------------------------------------------------\n");
	printf("\n\n\t\t\t\t\t\tI.  导入数据表文件进行预处理\t\t\t\t\n");
	if (isActivated) {
		FILE* file = NULL;										// 文件指针
		char rout[MAX_STRING_LENGTH] = "D:\\BTreeTest\\";	// 路径名
		char fileName[MAX_STRING_LENGTH];				// 文件名
		printf("\n\t\t\t\t请输入要导入的数据表文件名：");
		cin >> fileName;
		strcat(rout, fileName);
		file = fopen(rout, "r");
		if (file == NULL) {						// 判断文件是否可以打开，防止崩溃
			printf("\t\t\t\t找不到%s文件！", rout);
		}
		else {
			char isLegal[MAX_STRING_LENGTH];	// 校验码暂存区
			/*
			设置校验码的原因如下：若读取非系统内可识别的文件，在读入时会就出现异常而导致程序崩溃
			使用本系统输出的文件头部自带校验码，可以在一定程度上保证通过文件输入的合法性。
			*/
			fscanf(file, "%s", isLegal);
			if (strcmp(isLegal, fileIdentifierText) != 0) {
				printf("\t\t\t打开失败，文件校验错误，请检查是否为合法的系统输入文件！\n");
			}
			else {
				// 再建立一个文件到init文件夹，把学号和索引写进去
				// 根据索引值把信息存到数组里
				char tName[MAX_STRING_LENGTH];
				int tChineseScore, tMathScore, tEnglishScore;
				long long int tStuID;
				char indexPath[MAX_STRING_LENGTH] = "D:\\BTreeTest\\init\\nowIndex.txt";	// 索引文件路径名
				char dataPath[MAX_STRING_LENGTH] = "D:\\BTreeTest\\data\\nowData.txt";	// 数据文件路径名
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
				printf("\t\t\t\t已从%s里导入学生信息到系统\n", rout);
			}
		}
	}
	else {
		cout << "\n\t\t\t\tB-树未初始化！" << endl;
	}
	printf("\n\n\n\t\t\t---------------------------------------------------------------\n");
	system("pause");
}

void loadDataToBtree() {
	system("cls");
	printf("\t\t\t---------------------------------------------------------------\n");
	printf("\n\n\t\t\t\t\t\tL.  载入索引目录到B-树并挂载数据库\t\t\t\t\n");
	
	if (isActivated) {
		FILE* file;

		int checkNum1 = 0, checkNum2 = 0;

		char rout[MAX_STRING_LENGTH] = "D:\\BTreeTest\\init\\";
		char fileName[MAX_STRING_LENGTH];
		printf("\t\t\t\t请输入要载入的索引文件名：");
		cin >> fileName;
		file = fopen(strcat(rout, fileName), "r");
		if (file == NULL) {						// 判断文件是否可以打开，防止崩溃
			printf("\t\t\t\t找不到%s文件！", rout);
		}
		else {
			char isLegal[MAX_STRING_LENGTH];	// 校验码暂存区
			/*
			设置校验码的原因如下：若读取非系统内可识别的文件，在读入时会就出现异常而导致程序崩溃
			使用本系统输出的文件头部自带校验码，可以在一定程度上保证通过文件输入的合法性。
			*/
			fscanf(file, "%s", isLegal);
			if (strcmp(isLegal, fileIdentifierText) != 0) {
				printf("\t\t\t打开失败，文件校验错误，请检查是否为合法的系统输入文件！\n");
			}
			else {
				long long int tStuID;
				int tIndex;
				while (fscanf(file, "%lld %d", &tStuID, &tIndex) == 2) {
					t.InsertBTree(tStuID, tIndex);
					checkNum1++;
				}
				printf("\t\t\t\t已从%s里导入索引文件到B-树\n", rout);
			}
			fclose(file);
		}

		printf("\t\t\t\t请输入要载入的数据库文件名：");
		char rdata[MAX_STRING_LENGTH] = "D:\\BTreeTest\\data\\";
		cin >> fileName;
		file = fopen(strcat(rdata, fileName), "r");
		if (file == NULL) {						// 判断文件是否可以打开，防止崩溃
			printf("\t\t\t\t找不到%s文件！", rdata);
		}
		else {
			char isLegal[MAX_STRING_LENGTH];	// 校验码暂存区
			fscanf(file, "%s", isLegal);
			if (strcmp(isLegal, fileIdentifierText) != 0) {
				printf("\t\t\t打开失败，文件校验错误，请检查是否为合法的系统输入文件！\n");
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
				printf("\t\t\t\t已从%s里导入数据库文件供B-树使用\n", rout);
			}
			fclose(file);
		}

		if (checkNum1 != checkNum2) {
			cout << "\t\t\t数据库条目与索引条目不一致，执行查询操作可能会有异常！请检查！" << endl;
		}
		else
			stuNum = checkNum1;
	}
	else {
		cout << "\n\t\t\t\tB-树未初始化！" << endl;
	}

	printf("\n\n\n\t\t\t---------------------------------------------------------------\n");
	system("pause");
}

void exitProgram() {
	printf("\t\t\t\t\t感谢使用本查找工具，请按任意键退出...\n");
	system("pause");
	exit(0);
}

void getUserInput() {
	char ch;
	while (1) {
		// 输入此字符前的输入流很可能留有残余的'\n'，这会影响读入，所以采用此种方法吸收掉输入流前全部的'\n'。
		cin >> ch;
		if (ch == '\n')
			continue;
		else
			break;
	}
	while ((ch < '0' || ch > '4') && ch != 'i' && ch != 'I' && ch != 'l' && ch != 'L') {
		printf("\t\t\t\t输入有误，请重新输入: ");
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

