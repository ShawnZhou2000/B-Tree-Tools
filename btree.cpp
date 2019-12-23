#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

struct Index {
	string id;
	int loc;
};

//B-Tree ���
class BTreeNode {
protected:
	//string *keys;//�洢�ؼ��ֵ�����
	Index* keys;
	int t; //��С�� (������key��������Χ)
	BTreeNode** C; //�洢���ӽ�������
	int n; //����key�ĸ���
	bool leaf; //�����������һ��Ҷ�� ��ΪTRUE , ���� ΪFALSE
public:
	BTreeNode(int _t, bool _leaf);
	void traverse(); //�����Ըý��Ϊ�������������н��
	BTreeNode* search(Index k);//���Ըýڵ�Ϊ���������в��Ҽ�Ϊk�Ľ��

	void insertNonFull(Index k);
	void splitChild(int i, BTreeNode* y);

	friend class BTree;
};

class BTree {
protected:
	BTreeNode* root; //���ڵ��ָ��
	int t; //��С��
public:
	BTree(int _t) {
		root = NULL; t = _t;
	}
	void traverse() {
		if (root != NULL) root->traverse();
	}
	BTreeNode* search(Index k) {
		if (root == NULL) return NULL;
		else return root->search(k);
	}
	void insert(Index _k);
};

BTreeNode::BTreeNode(int _t, bool _leaf) {
	t = _t;
	leaf = _leaf;
	keys = new Index[2 * t - 1]; //һ�����key�����2*t-1��
	C = new BTreeNode * [2 * t]; //һ����㺢�������2*t��
	n = 0; //�½���㵱ǰkey������Ϊ0
}

void BTreeNode::traverse() {
	// ��n��key��n+1����key������n��key
	// ����ǰn����key
	int i;
	for (i = 0; i < n; ++i) {
		// ����ⲻ��Ҷ�ڵ㣬�ڴ�ӡkey[i]֮ǰ�ȱ�����C[i]Ϊ��������
		if (leaf == false)
			C[i]->traverse();
		cout << " " << keys[i].id;
	}
	// ��ӡ���һ�������ĸ�
	if (leaf == false)
		C[i]->traverse();
}

BTreeNode* BTreeNode::search(Index k) {
	// �ҵ���һ�����ڵ���k��key
	int i = 0;
	while (i < n && k.id > keys[i].id)
		i++;
	//����ҵ���keyֵ����k���򷵻ش˽��
	if (keys[i].id == k.id)
		return this;
	// �������û���ҵ�kezy��������һ��Ҷ�ڵ�
	if (leaf == true)
		return NULL;
	// ȥ��һ���ʵ�������������ѯ
	return C[i]->search(k);
}

void BTree::insert(Index k) {
	//�����Ϊ��
	if (root == NULL) {
		root = new BTreeNode(t, true);
		root->keys[0] = k; //���� key
		root->n = 1; //���µ�ǰroot����key�ĸ���Ϊ1
	}
	else {//����Ϊ��
		// ������ĸ���������������ĸ߶Ȼ�����
		if (root->n == 2 * t - 1) {
			BTreeNode* s = new BTreeNode(t, false);
			// �ɵĸ�����Ϊ�µĸ��ڵ�ĺ���
			s->C[0] = root;
			// ��־ɸ���㲢��һ��key�ƶ����¸������
			s->splitChild(0, root);
			// �µĸ��ڵ��������������ӽ�㡣���ھ��������ӽ���е���һ�������µ�key
			int i = 0;
			if (s->keys[0].id < k.id)
				++i;
			s->C[i]->insertNonFull(k);
			//�ı���ڵ�
			root = s;
		}
		else
			root->insertNonFull(k);
	}
}

// �ڴ˽���в�����key
// �����ڵ����������ʱ���������Ƿ�����
void BTreeNode::insertNonFull(Index k) {
	//��index��ʼ��Ϊ���ұ�Ԫ�ص�����
	int i = n - 1;

	// �������һ��Ҷ�ڵ�
	if (leaf == true) {
		// �����ѭ����������
		// a) ����Ҫ������key��δ֪
		// b) �����нϴ��key�ƶ���ǰ��һ��λ��
		while (i >= 0 && keys[i].id > k.id) {
			keys[i + 1] = keys[i];
			i--;
		}
		// ���ҵ���λ�ò����µ�key
		keys[i + 1] = k;
		n = n + 1;
	}
	else { // ��������㲻��Ҷ�ڵ�
		// �ҵ���ӵ����key���ӽ��
		while (i >= 0 && keys[i].id > k.id)
			i--;

		// �鿴�ҵ�����Ԫ���Ƿ�����
		if (C[i + 1]->n == 2 * t - 1) {
			// ���ĳ���ӽ������������Ҫ���ӽ����в��
			// ������������ӳԱ��ˣ��Ͱ����п����� ���ѿ���
			splitChild(i + 1, C[i + 1]);

			// ִ�в�ֲ�����C[i]�м��key������һ��������C[i]��һ��Ϊ��
			// ������һ����õ���key
			if (keys[i + 1].id < k.id)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}

// �ָ�˽����ӽ��y
// ע�⣬�������������ʱ��y����������
void BTreeNode::splitChild(int i, BTreeNode* y) {
	// ����һ����Ҫ�洢y����t-1��key���½��
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// ����y�����t-1��key��z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// ����y�����t���ӽ�㵽z
	if (y->leaf == false) {
		for (int j = 0; j < t; ++j)
			z->C[j] = y->C[j + t];
	}

	// ����y�е�key����
	y->n = t - 1;

	// ��Ϊ�����㽫��һ���µ��ӽ�㣬����Ҫ���µ��ӽ�㿪�ٿռ�
	for (int j = n; j >= i + 1; --j)
		C[j + 1] = C[j];

	// ���µ��ӽ�����ӵ��˽��
	C[i + 1] = z;

	// һ��y��key�����ƶ��������㣬�ҵ���key��λ�ò������нϴ��key�ƶ�һ��λ�á�
	for (int j = n - 1; j >= i; --j)
		keys[j + 1] = keys[j];

	// ��y�м��key���Ƶ�������
	keys[i] = y->keys[t - 1];

	// �˽���еļ���������1
	n++;
}

int main() {
	BTree t(3);
	t.insert({ "10",1 });
	t.insert({ "20",2 });
	t.insert({ "5",3 });
	t.insert({ "6",4 });
	t.insert({ "12",5 });
	t.insert({ "30",6 });
	t.insert({ "7",7 });
	t.insert({ "17",8 });

	t.traverse();
	cout << endl;
	Index k = { "20",2 };
	if (t.search(k) != NULL)
		cout << "exist" << endl;
	else
		cout << "not exist" << endl;
}