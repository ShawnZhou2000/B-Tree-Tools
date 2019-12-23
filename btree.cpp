#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

struct Index {
	string id;
	int loc;
};

//B-Tree 结点
class BTreeNode {
protected:
	//string *keys;//存储关键字的数据
	Index* keys;
	int t; //最小度 (决定了key的数量范围)
	BTreeNode** C; //存储孩子结点的数组
	int n; //现在key的个数
	bool leaf; //如果这个结点是一个叶子 则为TRUE , 否则 为FALSE
public:
	BTreeNode(int _t, bool _leaf);
	void traverse(); //遍历以该结点为根的子树中所有结点
	BTreeNode* search(Index k);//在以该节点为根的子树中查找键为k的结点

	void insertNonFull(Index k);
	void splitChild(int i, BTreeNode* y);

	friend class BTree;
};

class BTree {
protected:
	BTreeNode* root; //根节点的指针
	int t; //最小度
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
	keys = new Index[2 * t - 1]; //一个结点key最多有2*t-1个
	C = new BTreeNode * [2 * t]; //一个结点孩子最多有2*t个
	n = 0; //新建结点当前key的数量为0
}

void BTreeNode::traverse() {
	// 有n个key和n+1个子key，遍历n个key
	// 还有前n个子key
	int i;
	for (i = 0; i < n; ++i) {
		// 如果这不是叶节点，在打印key[i]之前先遍历以C[i]为根的子树
		if (leaf == false)
			C[i]->traverse();
		cout << " " << keys[i].id;
	}
	// 打印最后一个子树的根
	if (leaf == false)
		C[i]->traverse();
}

BTreeNode* BTreeNode::search(Index k) {
	// 找到第一个大于等于k的key
	int i = 0;
	while (i < n && k.id > keys[i].id)
		i++;
	//如果找到的key值等于k，则返回此结点
	if (keys[i].id == k.id)
		return this;
	// 如果这里没有找到kezy而这里是一个叶节点
	if (leaf == true)
		return NULL;
	// 去找一个适当的子树继续查询
	return C[i]->search(k);
}

void BTree::insert(Index k) {
	//如果树为空
	if (root == NULL) {
		root = new BTreeNode(t, true);
		root->keys[0] = k; //插入 key
		root->n = 1; //更新当前root结点的key的个数为1
	}
	else {//树不为空
		// 如果树的根结点已满，则树的高度会增加
		if (root->n == 2 * t - 1) {
			BTreeNode* s = new BTreeNode(t, false);
			// 旧的根结点成为新的根节点的孩子
			s->C[0] = root;
			// 拆分旧根结点并将一个key移动到新根结点上
			s->splitChild(0, root);
			// 新的根节点现在有了两个子结点。现在决定两个子结点中的哪一个会有新的key
			int i = 0;
			if (s->keys[0].id < k.id)
				++i;
			s->C[i]->insertNonFull(k);
			//改变根节点
			root = s;
		}
		else
			root->insertNonFull(k);
	}
}

// 在此结点中插入新key
// 假设在调用这个函数时，结点必须是非满的
void BTreeNode::insertNonFull(Index k) {
	//将index初始化为最右边元素的索引
	int i = n - 1;

	// 如果这是一个叶节点
	if (leaf == true) {
		// 下面的循环做两件事
		// a) 查找要插入新key的未知
		// b) 将所有较大的key移动到前方一个位置
		while (i >= 0 && keys[i].id > k.id) {
			keys[i + 1] = keys[i];
			i--;
		}
		// 在找到的位置插入新的key
		keys[i + 1] = k;
		n = n + 1;
	}
	else { // 如果这个结点不是叶节点
		// 找到将拥有新key的子结点
		while (i >= 0 && keys[i].id > k.id)
			i--;

		// 查看找到的子元素是否已满
		if (C[i + 1]->n == 2 * t - 1) {
			// 如果某个子结点已满，则需要对子结点进行拆分
			// 机翻：如果孩子吃饱了，就把它切开（草 我裂开了
			splitChild(i + 1, C[i + 1]);

			// 执行拆分操作后，C[i]中间的key会上升一级，并且C[i]会一分为二
			// 看看哪一个会得到新key
			if (keys[i + 1].id < k.id)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}

// 分割此结点的子结点y
// 注意，当调用这个函数时，y必须是满的
void BTreeNode::splitChild(int i, BTreeNode* y) {
	// 创建一个将要存储y结点的t-1个key的新结点
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// 复制y中最后t-1个key到z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// 复制y中最后t的子结点到z
	if (y->leaf == false) {
		for (int j = 0; j < t; ++j)
			z->C[j] = y->C[j + t];
	}

	// 减少y中的key数量
	y->n = t - 1;

	// 因为这个结点将有一个新的子结点，所以要给新的子结点开辟空间
	for (int j = n; j >= i + 1; --j)
		C[j + 1] = C[j];

	// 将新的子结点链接到此结点
	C[i + 1] = z;

	// 一个y的key将会移动到这个结点，找到新key的位置并将所有较大的key移动一个位置。
	for (int j = n - 1; j >= i; --j)
		keys[j + 1] = keys[j];

	// 将y中间的key复制到这个结点
	keys[i] = y->keys[t - 1];

	// 此结点中的键数量增加1
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