#pragma once
#include <iostream> 
#include "student.h"
using namespace std;

struct _keys {
	long long int keys; // ������ѧ�ţ�
	int indexs;		// ��ѯ��Ӧ���ݱ����
};

// �洢ȫ����ѧ�ţ�����B��ʱʹ��
long long int allStuID[MAX_STUDENT_NUM];
// ѧ�ż���������B��ʱʹ��
int cnt = 0;

// ����B-����� 
class BTreeNode {
private:
	//long long int* keys;   
	_keys* keys;		// ��Ž��ֵ������
	int t;      // ��С�� (������keyֵ��������Χ) ���ײ�����С��
	// ÿ����㶼�������ٰ���t-1��ֵ����������ٰ���һ��ֵ
	// ���н�㣨��������㣩���ɰ���2t �C 1��ֵ
	// �����������С��Ϊ3����ôһ����������5��ֵ�������ڵ�Ϊ������2��ֵ
	// ������С��Ϊ3��B-����Ӧ�Ľ׾���6
	// ��С��     �׼�
	//   2        4
	//   3        6
	//   4        8
	//   5        10
	// ������Ϊ�˷����������������Ľ��洢��ֵ�������������������û�鵽����
	BTreeNode** C; // ����ӽ��ָ�� 
	int n;     // ��ǰ���keyֵ�ĸ���
	bool leaf; // �ж��ǲ���Ҷ���

public:

	BTreeNode(int _t, bool _leaf);   // ���캯��

	// �Ӹ���㿪ʼ��������B�� 
	void TraverseBTree();

	// ���������аѽ��������ļ�
	void TraverseBTree(FILE* fp);

	// ���������аѽ�����������
	void TraverseSaveBTree();

	// �Ӹ���㿪ʼѰ��ĳ��ֵ���Ҳ����ͷ���NULL
	BTreeNode* SearchBTree(long long int k);

	// ���ص�һ�����ڵ���k������
	int findKey(long long int k);

	// ���Ըý��Ϊ���������в���һ����ֵ
	// �ڵ����������ʱ���������Ƿ�����
	void insertNonFull(long long int k, int index);

	// ���ӽ��������ʱ��ſ��Ե�����������������԰ѽ��һ��Ϊ��
	void splitChild(int i, BTreeNode* y);

	// ɾ��ֵk
	void DeleteBTree(long long int k);

	// ɾ��idxλ���ϵ�һ��ֵ��Ҫ����Ҷ���
	void DeleteBTreeFromLeaf(int idx);

	// ɾ��idxλ���ϵ�һ��ֵ��Ҫ���Ƿ�Ҷ���
	void DeleteBTreeFromNonLeaf(int idx);

	// ��ȡ����ϵ�idxλ�õ�ǰ��
	_keys getPred(int idx);

	// ��ȡ����ϵ�idxλ�õĺ��
	_keys getSucc(int idx);

	// ���ص�ǰ�������stuID��Ӧ��index������
	int getIndex(long long int stuID);

	// ���ý���ֵ����С��t-1ʱ���������C[]�����е�idx��λ�õ��ӽ��
	void fill(int idx);

	// ��C[idx-1]������һ��ֵ���������C[idx]���
	void borrowFromPrev(int idx);

	// ��C[idx+1]������һ��ֵ���������C[idx]���
	void borrowFromNext(int idx);

	// �����ĵ�idx���ӽ�����idx+1���ӽ��ϲ�
	void merge(int idx);

	// ѧ�ű��浽���飬����ʱʹ��
	void save(long long int id);

	// ��BTree��ΪBTreeNode����Ԫ
	// �����Ϳ�����BTree�����з���BTreeNode������˽�г�Ա 
	friend class BTree;
	
};

class BTree {
	BTreeNode* root; // ָ������ 
	int t;  // ��С��
public:
	// ȱʡ���캯�� Ĭ����С��Ϊ3
	BTree() {
		root = NULL;
		t = 3;
	}
	// ���캯�� ������ʼ��Ϊ�Զ�����С��
	BTree(int _t) {
		root = NULL;
		t = _t;
	}

	// �������� ��������B-��
	~BTree() {}

	// �������B������ӡ����Ļ��
	void TraverseBTree() {
		if (root != NULL) 
			root->TraverseBTree();
	}

	// �������B������ӡ���ļ���
	void TraverseBTree(FILE* fp) {
		if (root != NULL)
			root->TraverseBTree(fp);
	}

	// �������B������������ֵ�����������У�����ɾ����
	void TraverseSaveBTree() {
		if (root != NULL)
			root->TraverseSaveBTree();
	}

	// ��B���в���һ��ֵ 
	BTreeNode* SearchBTree(long long int k) {
		return (root == NULL) ? NULL : root->SearchBTree(k);
	}

	

	// ��B���в���һ���µ�ֵ
	void InsertBTree(long long int k, int index);

	// ��B����ɾ��һ��ֵ
	void DeleteBTree(long long int k);

	// ��ʼ��B��
	BTree InitBTree(int minDeg);

	// ����B��
	void DestroyBTree();

};

BTreeNode::BTreeNode(int t1, bool leaf1) {
	// �洢��С�Ⱥ��Ƿ�ΪҶ������Ϣ
	t = t1;
	leaf = leaf1;

	// �����ڴ�
	//keys = new long long int[2 * t - 1];
	keys = new _keys[2 * t - 1];
	C = new BTreeNode * [2 * t];

	// ֵ��������ʼʱΪ0
	n = 0;
}

// ���ص�һ�����ڵ���k�ĵ�һ��ֵ������
int BTreeNode::findKey(long long int k) {
	int idx = 0;
	while (idx < n && keys[idx].keys < k)
		++idx;
	return idx;
}

// ��ĳ�������ɾ��ֵk 
void BTreeNode::DeleteBTree(long long int k) {
	// index���ڵ�һ�����ڵ���kֵ������
	int idx = findKey(k);

	// ���Ҫɾ���ļ������ڴ˽����
	if (idx < n && keys[idx].keys == k) {

		// �����Ҷ��㣬�����ɾ��Ҷ���ķ���
		// �������Ҷ��㣬�����ɾ����Ҷ���ķ���
		if (leaf)
			DeleteBTreeFromLeaf(idx);
		else
			DeleteBTreeFromNonLeaf(idx);
	}
	else {
		// ��������Ҫɾ���ļ�������������
		// �����ʱ�Ѿ��ҵ���Ҷ��㣬��ô���ֵ������������� 
		if (leaf) {
			cout << "�� " << k << " ������������\n";
			return;
		}

		// �˺�������Ҫɾ����ֵ�������Ըý��Ϊ�����ӽ����ʱ

		// flag��ʾֵ�Ƿ�����ڴ˽������һ���ӽ����
		bool flag = ((idx == n) ? true : false);

		// ��ɾ��һ��ֵ֮���������ӽ��Ӧ�ô��ڵ�ֵ������t���ˣ��Ǿ�Ӧ���������ӽ��
		if (C[idx]->n < t)
			fill(idx);

		// ������һ���ӽ���Ѿ����ϲ�����ô��һ���Ѿ���ǰһ���ӽ��ϲ��ˣ������ڵ�idx-1���ӽ����ɾ��k������ɾ��������t��ֵ�ĵ�idx���ӽ��
		if (flag && idx > n)
			C[idx - 1]->DeleteBTree(k);
		else
			C[idx]->DeleteBTree(k);
	}
	return;
}

// �����Ҷ���ɾ����idx��ֵ
void BTreeNode::DeleteBTreeFromLeaf(int idx) {

	//����idx��ֵ�������ֵ����ƶ�һ��λ��
	for (int i = idx + 1; i < n; ++i) {
		keys[i - 1] = keys[i];
	}


	// ֵ������1
	n--;

	return;
}

// ��һ����Ҷ���ɾ����idx��ֵ
void BTreeNode::DeleteBTreeFromNonLeaf(int idx) {

	long long int k = keys[idx].keys;

	// ���C[idx]������������t��ֵ����ô����C[idx]Ϊ�����������ҵ�k��һ��ǰ������ǰ���滻k���ݹ�ɾ��C[idx]�е�ǰ��
	if (C[idx]->n >= t) {
		_keys pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->DeleteBTree(pred.keys);
	}

	// ����ӽ��C[idx]��ֵ��������t�٣���ȥ��C[idx+1]�����C[idx+1]������t��ֵ���ڸ�ΪC[idx+1]����������k�ĺ��
	else if (C[idx + 1]->n >= t) {
		_keys succ = getSucc(idx);
		keys[idx] = succ;
		C[idx + 1]->DeleteBTree(succ.keys);
	}

	// ���C[idx]��C[idx+1]��ֵ��t�٣���k�����е�C[idx+1]�ϲ���C[idx]�У�����C[idx]����2t-1����C[idx+1]�ݹ�ش�C[idx]��ɾ��k
	else {
		merge(idx);
		C[idx]->DeleteBTree(k);
	}
	return;
}

// ��ȡ����ֵ��ǰ��
_keys BTreeNode::getPred(int idx) {
	// һֱ�ƶ������ұߵĽ�㣬ֱ������һ��Ҷ���
	BTreeNode* cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	// ����Ҷ�������һ��ֵ
	return cur->keys[cur->n - 1];
}

// ��ȡ����ֵ�ĺ��
_keys BTreeNode::getSucc(int idx) {

	// ��C[idx+1]��ʼ�ƶ�����ߵĽ�㣬ֱ������һ��Ҷ��� 
	BTreeNode* cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	// ����Ҷ���ĵ�һ��ֵ
	return cur->keys[0];
}

// ���ֵ����С��t-1���ӽ��C[idx]
void BTreeNode::fill(int idx) {

	// ���ǰһ���ӽ��C[idx-1]��ֵ����t-1����������ӽ���һ��ֵ
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	// �����һ���ӽ��C[idx+1]��ֵ����t-1����������ӽ���һ��ֵ
	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	// �ϲ�C[idx]�������ֵ�
	// ���C[idx]�����һ����Ԫ�أ���ô������ǰһ����Ԫ�غϲ�
	// ����ͽ�����������һ���ֵܺϲ�
	else {
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

// ��C[idx-1]�н���һ��ֵ��������뵽C[idx]��
void BTreeNode::borrowFromPrev(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx - 1];

	// C[idx-1]�����һ��ֵ����һ�㵽�丸��㣬�������Ը����ĵ�idx-1��ֵ��ΪC[idx]�еĵ�һ��ֵ���롣
	// ���������ֵܽ�����һ��ֵ���ӽ����һ��ֵ

	// ��C[idx]�е�����ֵ��ǰ�ƶ�һ��
	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	// ���C[idx]����Ҷ��㣬��ô����������ָ���ӽ��ָ����ǰ�ƶ�һ��
	if (!child->leaf) {
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	// ���ӽ��ĵ�һ��ֵ����Ϊ��ǰ���ĵ�idx-��ֵ
	child->keys[0] = keys[idx - 1];

	// ���ֵܽ������һ���ӽ����ΪC[idx]�ĵ�һ���ӽ��
	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	// ��ֵ�ƶ������ĸ���㣬������ͬ����ֵ������
	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

//��C[idx+1]�н���һ��ֵ���������C[idx]��
void BTreeNode::borrowFromNext(int idx) {

	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// ��idx��ֵ��ΪC[idx]�е����һ��ֵ����
	child->keys[(child->n)] = keys[idx];

	// ���ֵܽ��ĵ�һ���ӽ����Ϊ���һ���ӽ����뵽C[idx]��
	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	// ��idx��ֵ��Ϊ�ֵܽ��ĵ�һ��ֵ
	keys[idx] = sibling->keys[0];

	// ���ֵܽ���е����м�����ƶ�һ�� 
	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	// ���ӽ��ָ�������һ��
	if (!sibling->leaf) {
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	// ����C[idx]��ֵ����������C[idx+1]��ֵ����
	child->n += 1;
	sibling->n -= 1;

	return;
}

// �ϲ�C[idx]��C[idx+1]��C[idx+1]�ںϲ����ͷ�
void BTreeNode::merge(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// �ӵ�ǰ���ȡ��һ��ֵ�����뵽C[idx]�ĵ�t-1��λ��
	child->keys[t - 1] = keys[idx];

	// ��C[idx+1]���Ƶ�C[idx]
	for (int i = 0; i < sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	// ��ָ���ӽ���ָ���C[idx+1]���Ƶ�C[idx]
	if (!child->leaf) {
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	// ����ǰ�����idx֮������м���ǰ�ƶ�һ��
	// �������idx��ֵ�ƶ���C[idx]����ɵĿ�ȱ
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	// ��idx+1֮���ָ���ӽ���ָ����ǰ�ƶ�һ�� 
	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	// �����ӽ��͵�ǰ����ֵ����
	child->n += sibling->n + 1;
	n--;

	// �ͷ��ֵܽ��ռ�õ��ڴ�
	delete(sibling);
	return;
}

// ��B���в���һ����ֵ
void BTree::InsertBTree(long long int k, int index) {
	// �����Ϊ��
	if (root == NULL) {
		// �����������ڴ�
		root = new BTreeNode(t, true);
		root->keys[0].keys = k;  // ����ֵ
		root->keys[0].indexs = index;
		root->n = 1;  // ���¸���
	}
	else { // ������ǿ�
		// ������������������������һ�㣬�����ĸ߶�����
		if (root->n == 2 * t - 1) {
			// Ϊ�µġ�����㡱�����ڴ�
			BTreeNode* s = new BTreeNode(t, false);

			// ԭ���ĸ�������µĸ������ӽ��
			s->C[0] = root;

			// �ָ�ԭ���ĸ���㣬�����ƶ�һ��ֵ���µĸ������
			s->splitChild(0, root);

			// ���ڵĸ�������������ӽ�㣬�ڲ���ʱѡ����뵽�ĸ��ӽ��
			int i = 0;
			if (s->keys[0].keys < k)
				i++;
			s->C[i]->insertNonFull(k, index);

			// ���������Ϊs
			root = s;
		}
		else
			// ������������ģ������insertNonFull������ֵ
			root->insertNonFull(k, index);
	}
}

// ����������Ľ���в�����ֵ
void BTreeNode::insertNonFull(long long int k, int index) {
	// ��index��ʼ��Ϊ���ұ�Ԫ�ص�����
	int i = n - 1;

	// �������һ��Ҷ��㣬�ǾͿ���ֱ�Ӳ�
	if (leaf == true) {
		// �Ȳ���Ҫ�������ֵ��λ��
		// �ٽ����и����ֵ��ǰ�ƶ�һ��λ��
		while (i >= 0 && keys[i].keys > k) {
			keys[i + 1] = keys[i];
			i--;
		}

		// ���ҵ���λ�ò�����ֵ
		keys[i + 1].keys = k;
		keys[i + 1].indexs = index;
		n = n + 1;
	}
	else {
		// ��Ҷ�������
		// �ҵ����Բ�����ֵ���ӽ��
		while (i >= 0 && keys[i].keys > k)
			i--;

		// ����֮ǰ�ȿ�������ӽ���ǲ�������
		if (C[i + 1]->n == 2 * t - 1) {
			// ����ӽ��������������п�
			splitChild(i + 1, C[i + 1]);

			// �ֿ���C[i]����ֵ������C[i]һ��Ϊ����Ȼ���ٿ����ĸ��ӽ�����ֵ
			if (keys[i + 1].keys < k)
				i++;
		}
		// Ȼ���ٳ��Բ���
		C[i + 1]->insertNonFull(k, index);
	}
}

// �ָ���y
// ����ʱ������������
void BTreeNode::splitChild(int i, BTreeNode* y) {
	// ����һ���½��z���洢y��t-1��ֵ
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// ����ǰt-1��ֵ��z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// ���y����Ҷ��㣬��y�ĺ�t���ӽ�㸴�Ƶ�z
	if (y->leaf == false) {
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// y��ֵ�ĸ������٣���Ϊt-1��
	y->n = t - 1;

	// ��ΪҪ�ѽ��һ��Ϊ��������Ҫ���½�������ռ�
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// ���µ��ӽ��ŵ�����
	C[i + 1] = z;

	// y�е�һ��ֵ���ƶ��������㣬�ҵ���ֵ��λ�ò������б������ֵ��ǰ�ƶ�һλ
	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// ��y����ֵ���Ƶ�������
	keys[i] = y->keys[t - 1];

	// ���ֵ����+1
	n = n + 1;
}

// �����Ըý��Ϊ���������е����н��
void BTreeNode::TraverseBTree() {
	// �����n��ֵ��n+1���ӽ�㣬����n������ǰn���ӽ��
	int i;
	for (i = 0; i < n; i++) {
		// ����ⲻ��Ҷ��㣬��ô�������i��ֵ֮ǰ���Ƚ������ĵ�i���ӽ�����
		if (leaf == false)
			C[i]->TraverseBTree();
		int id = keys[i].indexs;
		printf("\t\t\t%5lld\t%6s\t\t%4d\t\t%4d\t\t%4d\t\n", keys[i].keys, info[id].name, info[id].chineseScore, info[id].mathScore, info[id].englishScore);
	}

	// ������һ������
	if (leaf == false)
		C[i]->TraverseBTree();
}


// �����Ըý��Ϊ���������е����н�㣬������ļ�
void BTreeNode::TraverseBTree(FILE* fp) {
	// �����n��ֵ��n+1���ӽ�㣬����n������ǰn���ӽ��
	int i;
	for (i = 0; i < n; i++) {
		// ����ⲻ��Ҷ��㣬��ô�������i��ֵ֮ǰ���Ƚ������ĵ�i���ӽ�����
		if (leaf == false)
			C[i]->TraverseBTree(fp);
		int id = keys[i].indexs;
		fprintf(fp, "%lld %s %d %d %d\n", keys[i].keys, info[id].name, info[id].chineseScore, info[id].mathScore, info[id].englishScore);
	}

	// ������һ������
	if (leaf == false)
		C[i]->TraverseBTree(fp);
}

// �����Ըý��Ϊ���������е����н�㣬���浽����
void BTreeNode::TraverseSaveBTree() {
	// �����n��ֵ��n+1���ӽ�㣬����n������ǰn���ӽ��
	int i;
	for (i = 0; i < n; i++) {
		// ����ⲻ��Ҷ��㣬��ô�������i��ֵ֮ǰ���Ƚ������ĵ�i���ӽ�����
		if (leaf == false)
			C[i]->TraverseSaveBTree();
		save(keys[i].keys);
	}

	// ������һ������
	if (leaf == false)
		C[i]->TraverseSaveBTree();
}

// Ѱ�ҵ�ǰ�������stuID��Ӧ��index������
int BTreeNode::getIndex(long long int stuID) {
	for (int i = 0; i < n; i++) {
		if (keys[i].keys == stuID)
			return keys[i].indexs;
	}
	// ��һ�ֳ���
	return -1;
}

// ���Ըý��Ϊ��������������ֵk
BTreeNode* BTreeNode::SearchBTree(long long int k) {
	// �ҵ���һ�����ڵ���k��ֵ
	int i = 0;
	while (i < n && k > keys[i].keys)
		i++;

	// ����ҵ���ֵ����k���򷵻ش˽��
	if (keys[i].keys == k)
		return this;

	// �������һ��Ҷ��㣬˵������ʧ�ܣ��Ҳ���ֵ
	if (leaf == true)
		return NULL;

	// ȥ���ڵ��ӽ���������
	return C[i]->SearchBTree(k);
}

void BTree::DeleteBTree(long long int k) {
	if (!root) {
		// root��ָ���κ�λ��ʱ��Ϊ��
		cout << "��Ϊ�գ��޷�ɾ��\n";
		return;
	}

	// ����Ϊ��������ɾ������
	// �����DeleteBTree�����򵥸�����DeleteBTree
	root->DeleteBTree(k);

	// ɾ��֮����Ҫע�������ǲ����Ѿ�û��ֵ��
	// ��������û��ֵ����������ӽ�㣬�������ĵ�һ���ӽ����Ϊ�µĸ����
	// ������ӽ�㶼û�У��Ǿ�ֻ���ø�ָ����ˣ���Ϊ��
	if (root->n == 0) {
		BTreeNode* tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		// �ͷ�ԭ������ڴ�
		delete tmp;
	}
	return;
}

void BTreeNode::save(long long int id) {
	allStuID[cnt++] = id;
}

// ��һ���Զ������С�ȳ�ʼ��B��
BTree BTree::InitBTree(int minDeg) {
	return BTree(minDeg);
}

// ����B���ͷ��ڴ�
void BTree::DestroyBTree() {
	for (int i = 0; i < cnt; i++) {
		DeleteBTree(allStuID[i]);
	}
	(*this).~BTree();
}