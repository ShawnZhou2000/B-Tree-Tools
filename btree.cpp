/* 
   The following functions are exclusive for deletion In class BTreeNode:
	1) DeleteBTree
	2) DeleteBTreeFromLeaf
	3) DeleteBTreeFromNonLeaf
	4) getPred
	5) getSucc
	6) borrowFromPrev
	7) borrowFromNext
	8) merge
	9) findKey

   In class BTree:
	 1) DeleteBTree
 */

#include<iostream> 
using namespace std;

struct _keys {
	long long int keys; // 主键（学号）
	int indexs;		// 查询对应数据表序号
};

// 单个B-树结点 
class BTreeNode {
private:
	//long long int* keys;   
	_keys* keys;		// 存放结点值的数组
	int t;      // 最小度 (定义了key值的数量范围) ，阶不是最小度
	// 每个结点都必须至少包含t-1个值，根结点至少包含一个值
	// 所有结点（包括根结点）最多可包含2t – 1个值
	// 举例：如果最小度为3，那么一个结点最多有5个值，除根节点为最少有2个值
	// 所以最小度为3的B-树对应的阶就是6
	// 最小度     阶级
	//   2        4
	//   3        6
	//   4        8
	//   5        10
	// 可能是为了方便操作，这样定义的结点存储的值的最大数量都是奇数？没查到资料
	BTreeNode** C; // 存放子结点指针 
	int n;     // 当前结点key值的个数
	bool leaf; // 判断是不是叶结点

public:

	BTreeNode(int _t, bool _leaf);   // 构造函数

	// 从根结点开始遍历整个B树 
	void TraverseBTree();

	// 从根结点开始寻找某个值，找不到就返回NULL
	BTreeNode* SearchBTree(long long int k);

	// 返回第一个大于等于k的索引
	int findKey(long long int k);

	// 在以该结点为根的子树中插入一个新值
	// 在调用这个函数时，结点必须是非满的
	void insertNonFull(long long int k);

	// 在子结点已满的时候才可以调用这个方法，它可以把结点一分为二
	void splitChild(int i, BTreeNode* y);

	// 删除值k
	void DeleteBTree(long long int k);

	// 删除idx位置上的一个值，要求是叶结点
	void DeleteBTreeFromLeaf(int idx);

	// 删除idx位置上的一个值，要求是非叶结点
	void DeleteBTreeFromNonLeaf(int idx);

	// 获取结点上第idx位置的前驱
	long long int getPred(int idx);

	// 获取结点上第idx位置的后继
	long long int getSucc(int idx);

	// 当该结点的值数量小于t-1时，用于填充C[]数组中第idx个位置的子结点
	void fill(int idx);

	// 从C[idx-1]结点借用一个值并将其放入C[idx]结点
	void borrowFromPrev(int idx);

	// 从C[idx+1]结点借用一个值并将其放入C[idx]结点
	void borrowFromNext(int idx);

	// 将结点的第idx个子结点与第idx+1个子结点合并
	void merge(int idx);

	// 让BTree成为BTreeNode的友元
	// 这样就可以在BTree函数中访问BTreeNode这个类的私有成员 
	friend class BTree;
};

class BTree {
	BTreeNode* root; // 指向根结点 
	int t;  // 最小度
public:
	// 缺省构造函数 默认最小度为3
	BTree() {
		root = NULL;
		t = 3;
	}
	// 构造函数 将树初始化为自定义最小度
	BTree(int _t) {
		root = NULL;
		t = _t;
	}

	// 析构函数 用于销毁B-树
	~BTree() {}

	void TraverseBTree() {
		if (root != NULL) root->TraverseBTree();
	}

	// 在B树中查找一个值 
	BTreeNode* SearchBTree(long long int k) {
		return (root == NULL) ? NULL : root->SearchBTree(k);
	}

	// 在B树中插入一个新的值
	void InsertBTree(long long int k);

	// 在B树中删除一个值
	void DeleteBTree(long long int k);

	// 初始化B树
	BTree InitBTree(int minDeg);
	
	// 销毁B树
	void DestroyBTree();

};

BTreeNode::BTreeNode(int t1, bool leaf1) {
	// 存储最小度和是否为叶结点的信息
	t = t1;
	leaf = leaf1;

	// 分配内存
	//keys = new long long int[2 * t - 1];
	keys = new _keys[2 * t - 1];
	C = new BTreeNode * [2 * t];

	// 值的数量初始时为0
	n = 0;
}

// 返回第一个大于等于k的第一个值的索引
int BTreeNode::findKey(long long int k) {
	int idx = 0;
	while (idx < n && keys[idx].keys < k)
		++idx;
	return idx;
}

// 在某个结点中删除值k 
void BTreeNode::DeleteBTree(long long int k) {
	// index等于第一个大于等于k值的索引
	int idx = findKey(k);

	// 如果要删除的键存在于此结点中
	if (idx < n && keys[idx].keys == k) {

		// 如果是叶结点，则调用删除叶结点的方法
		// 如果不是叶结点，则调用删除非叶结点的方法
		if (leaf)
			DeleteBTreeFromLeaf(idx);
		else
			DeleteBTreeFromNonLeaf(idx);
	}
	else {
		// 这个情况是要删除的键不在这个结点里
		// 如果此时已经找到了叶结点，那么这个值不会存在于树中 
		if (leaf) {
			cout << "键 " << k << " 不存在于树中\n";
			return;
		}

		// 此后的情况是要删除的值存在于以该结点为根的子结点中时

		// flag表示值是否存在于此结点的最后一个子结点中
		bool flag = ((idx == n) ? true : false);

		// 在删除一个值之后，如果这个子结点应该存在的值数量比t少了，那就应该填充这个子结点
		if (C[idx]->n < t)
			fill(idx);

		// 如果最后一个子结点已经被合并，那么它一定已经与前一个子结点合并了，所以在第idx-1个子结点上删除k。否则，删除至少有t个值的第idx个子结点
		if (flag && idx > n)
			C[idx - 1]->DeleteBTree(k);
		else
			C[idx]->DeleteBTree(k);
	}
	return;
}

// 从这个叶结点删除第idx个值
void BTreeNode::DeleteBTreeFromLeaf(int idx) {

	//将第idx个值后的所有值向后移动一个位置
	for (int i = idx + 1; i < n; ++i) {
		keys[i - 1] = keys[i];
	}
		

	// 值数量减1
	n--;

	return;
}

// 从一个非叶结点删除第idx个值
void BTreeNode::DeleteBTreeFromNonLeaf(int idx) {

	long long int k = keys[idx].keys;

	// 如果C[idx]的子树至少有t个值，那么在以C[idx]为根的子树中找到k的一个前驱。用前驱替换k。递归删除C[idx]中的前驱
	if (C[idx]->n >= t) {
		long long int pred = getPred(idx);
		//keys[idx].indexs = keys[pred].indexs;
		keys[idx].keys = pred;
		C[idx]->DeleteBTree(pred);
	}

	// 如果子结点C[idx]的值的数量比t少，就去看C[idx+1]，如果C[idx+1]有至少t个值，在根为C[idx+1]的子树中找k的后继
	else if (C[idx + 1]->n >= t) {
		long long int succ = getSucc(idx);
		//keys[succ].indexs = keys[succ].indexs;
		keys[idx].keys = succ;
		C[idx + 1]->DeleteBTree(succ);
	}

	// 如果C[idx]和C[idx+1]的值比t少，将k和所有的C[idx+1]合并到C[idx]中，现在C[idx]包含2t-1键，C[idx+1]递归地从C[idx]中删除k
	else {
		merge(idx);
		C[idx]->DeleteBTree(k);
	}
	return;
}

// 获取索引值的前驱
long long int BTreeNode::getPred(int idx) {
	// 一直移动到最右边的结点，直到到达一个叶结点
	BTreeNode* cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	// 返回叶结点的最后一个值
	return cur->keys[cur->n - 1].keys;
}

// 获取索引值的后继
long long int BTreeNode::getSucc(int idx) {

	// 从C[idx+1]开始移动最左边的结点，直到到达一个叶结点 
	BTreeNode* cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	// 返回叶结点的第一个值
	return cur->keys[0].keys;
}

// 填充值数量小于t-1的子结点C[idx]
void BTreeNode::fill(int idx) {

	// 如果前一个子结点C[idx-1]的值超过t-1个，就向该子结点借一个值
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	// 如果下一个子结点C[idx+1]的值超过t-1个，就向该子结点借一个值
	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	// 合并C[idx]和它的兄弟
	// 如果C[idx]是最后一个子元素，那么将它与前一个子元素合并
	// 否则就将它与它的下一个兄弟合并
	else {
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

// 从C[idx-1]中借用一个值并将其插入到C[idx]中
void BTreeNode::borrowFromPrev(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx - 1];

	// C[idx-1]的最后一个值上升一层到其父结点，并且来自父结点的第idx-1个值作为C[idx]中的第一个值插入。
	// 这样做后，兄弟结点减少一个值，子结点获得一个值

	// 将C[idx]中的所有值向前移动一格
	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	// 如果C[idx]不是叶结点，那么将它的所有指向子结点指针向前移动一格
	if (!child->leaf) {
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	// 将子结点的第一个值设置为当前结点的第idx-个值
	child->keys[0] = keys[idx - 1];

	// 把兄弟结点的最后一个子结点作为C[idx]的第一个子结点
	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	// 把值移动到它的父结点，减少了同级的值的数量
	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

//从C[idx+1]中借用一个值并将其放入C[idx]中
void BTreeNode::borrowFromNext(int idx) {

	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// 第idx个值作为C[idx]中的最后一个值插入
	child->keys[(child->n)] = keys[idx];

	// 将兄弟结点的第一个子结点作为最后一个子结点插入到C[idx]中
	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	// 第idx个值变为兄弟结点的第一个值
	keys[idx] = sibling->keys[0];

	// 将兄弟结点中的所有键向后移动一格 
	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	// 将子结点指针向后移一格
	if (!sibling->leaf) {
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	// 增加C[idx]的值数量，减少C[idx+1]的值数量
	child->n += 1;
	sibling->n -= 1;

	return;
}

// 合并C[idx]与C[idx+1]，C[idx+1]在合并后释放
void BTreeNode::merge(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// 从当前结点取出一个值，插入到C[idx]的第t-1个位置
	child->keys[t - 1] = keys[idx];

	// 从C[idx+1]复制到C[idx]
	for (int i = 0; i < sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	// 将指向子结点的指针从C[idx+1]复制到C[idx]
	if (!child->leaf) {
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	// 将当前结点中idx之后的所有键向前移动一格
	// 以填补将第idx个值移动到C[idx]后造成的空缺
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	// 在idx+1之后的指向子结点的指针向前移动一格 
	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	// 更新子结点和当前结点的值数量
	child->n += sibling->n + 1;
	n--;

	// 释放兄弟结点占用的内存
	delete(sibling);
	return;
}

// 在B树中插入一个新值
void BTree::InsertBTree(long long int k) {
	// 如果树为空
	if (root == NULL) {
		// 给根结点分配内存
		root = new BTreeNode(t, true);
		root->keys[0].keys = k;  // 插入值
		root->n = 1;  // 更新个数
	}
	else { // 如果树非空
		// 如果根结点已满，则必须插入下一层，让树的高度增加
		if (root->n == 2 * t - 1) {
			// 为新的“根结点”分配内存
			BTreeNode* s = new BTreeNode(t, false);

			// 原来的根结点变成新的根结点的子结点
			s->C[0] = root;

			// 分割原来的根结点，并且移动一个值到新的根结点上
			s->splitChild(0, root);

			// 现在的根结点有了两个子结点，在插入时选择插入到哪个子结点
			int i = 0;
			if (s->keys[0].keys < k)
				i++;
			s->C[i]->insertNonFull(k);

			// 更换根结点为s
			root = s;
		}
		else  
			// 如果根不是满的，则调用insertNonFull插入新值
			root->insertNonFull(k);
	}
}

// 在这个非满的结点中插入新值
void BTreeNode::insertNonFull(long long int k) {
	// 将index初始化为最右边元素的索引
	int i = n - 1;

	// 如果这是一个叶结点，那就可以直接插
	if (leaf == true) {
		// 先查找要插入的新值的位置
		// 再将所有更大的值向前移动一个位置
		while (i >= 0 && keys[i].keys > k) {
			keys[i + 1] = keys[i];
			i--;
		}

		// 在找到的位置插入新值
		keys[i + 1].keys = k;
		n = n + 1;
	}
	else {
		// 非叶结点的情况
		// 找到可以插入新值的子结点
		while (i >= 0 && keys[i].keys > k)
			i--;

		// 插入之前先看看这个子结点是不是满了
		if (C[i + 1]->n == 2 * t - 1) {
			// 如果子结点已满，则把它切开
			splitChild(i + 1, C[i + 1]);

			// 分开后，C[i]的中值上升，C[i]一分为二，然后再看给哪个子结点插新值
			if (keys[i + 1].keys < k)
				i++;
		}
		// 然后再尝试插入
		C[i + 1]->insertNonFull(k);
	}
}

// 分割结点y
// 调用时结点必须是满的
void BTreeNode::splitChild(int i, BTreeNode* y) {
	// 创建一个新结点z，存储y的t-1个值
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// 复制前t-1个值到z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// 如果y不是叶结点，将y的后t个子结点复制到z
	if (y->leaf == false) {
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// y中值的个数减少，变为t-1个
	y->n = t - 1;

	// 因为要把结点一分为二，所以要给新结点留出空间
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// 把新的子结点放到里面
	C[i + 1] = z;

	// y中的一个值会移动到这个结点，找到新值的位置并将所有比他大的值向前移动一位
	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// 将y的中值复制到这个结点
	keys[i] = y->keys[t - 1];

	// 结点值计数+1
	n = n + 1;
}

// 遍历以该结点为根的子树中的所有结点
void BTreeNode::TraverseBTree() {
	// 结点有n个值和n+1个子结点，遍历n个键和前n个子结点
	int i;
	for (i = 0; i < n; i++) {
		// 如果这不是叶结点，那么在输出第i个值之前，先进到它的第i个子结点遍历
		if (leaf == false)
			C[i]->TraverseBTree();
		cout << " " << keys[i].keys;
	}

	// 输出最后一个子树
	if (leaf == false)
		C[i]->TraverseBTree();
}

// 在以该结点为根的子树中搜索值k
BTreeNode* BTreeNode::SearchBTree(long long int k) {
	// 找到第一个大于等于k的值
	int i = 0;
	while (i < n && k > keys[i].keys)
		i++;

	// 如果找到的值等于k，则返回此结点
	if (keys[i].keys == k)
		return this;

	// 如果这是一个叶结点，说明查找失败，找不到值
	if (leaf == true)
		return NULL;

	// 去相邻的子结点继续查找
	return C[i]->SearchBTree(k);
}

void BTree::DeleteBTree(long long int k) {
	if (!root) {
		// root不指向任何位置时树为空
		cout << "树为空，无法删除\n";
		return;
	}

	// 首先为根结点调用删除函数
	// 这里的DeleteBTree是面向单个结点的DeleteBTree
	root->DeleteBTree(k);

	// 删除之后需要注意根结点是不是已经没有值了
	// 如果根结点没有值，如果它有子结点，就让它的第一个子结点作为新的根结点
	// 如果连子结点都没有，那就只能让根指向空了（树为空
	if (root->n == 0) {
		BTreeNode* tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		// 释放原根结点内存
		delete tmp;
	}
	return;
}

BTree BTree::InitBTree(int minDeg) {
	return BTree(minDeg);
}

void BTree::DestroyBTree() {
	(*this).~BTree();
}

int main() {
	 // 一个六阶的B-树
	BTree t;
	t = t.InitBTree(3);

	int num = 0;
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> num;
		t.InsertBTree(num);
	}

	cout << "Traversal of tree constructed is\n";
	t.TraverseBTree();
	cout << endl;

	t.DeleteBTree(6);
	cout << "Traversal of tree after removing 6\n";
	t.TraverseBTree();
	cout << endl;

	/*cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> num;
		BTreeNode* se = t.SearchBTree(num);
		if (se == NULL) {
			cout << "没找到" << num << endl;
		}
		else {
			
		}
	}*/
	t.DestroyBTree();

	return 0;
}