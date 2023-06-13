#include"RB_Tree.h"
#include<iostream>

template<class K,class V>
bool RB_Tree<K, V>::is_balance() 
{
	if (_root == nullptr) return true;
	if (_root->_color != BLACK) return false;
	Node* tmp = _root;
	int black_count = 0;
	while(tmp)
	{
		if (tmp->_color == BLACK) black_count++;
		tmp = tmp->_left;
	}
	return _is_balance(_root, 0, black_count);
}

template<class K,class V>
bool RB_Tree<K, V>::_is_balance(Node* root, int black_count, const int& ref_black_count)
{
	if (root == nullptr)
	{
		if (black_count != ref_black_count)
		{
			cout << "当前路径的黑色节点与参考值不同" << endl;
			return false;
		}
	}

	//因为根节点必须是黑的，所以如果某节点为红的，它一定有父节点，所以不需判断父节点是否存在
	if (root->_color == RED && root->_parent->_color == "RED")
	{
		cout << root->_data.first << " 此节点及其父节点均为红色" << endl;
		return false;
	}

	if (root->_color == BLACK) ++black_count;

	return _is_balcnce(root->_left, black_count, ref_black_count) && _is_balance(root->_right, black_count, ref_black_count);
}

//模板成员函数显式初始化
template typename RB_Tree<int, int>::Node* RB_Tree<int, int>::find(const int& n);
template<class K,class V>
RB_Tree_Node<K, V>* RB_Tree<K, V>::find(const K& key)
{
	Node* cur = _root; 

	while (cur)
	{
		if (cur->_data.first > key) cur = cur->_left;
		else if (cur->_data.first < key) cur = cur->_right;
		else return cur;
	}
	return nullptr;
}

template<class K,class V>
void RB_Tree<K, V>::left_rotate(Node* parent)
{
	Node* subR = parent->_right;
	Node* subRL = subR->_left;
	Node* parent_parent = parent->_parent;
	
	//建立subRL与parent之间的联系
	parent->_right = subRL;
	if (subRL) subRL->_parent = parent;

	//建立parent与subR之间的联系
	subR->_left = parent;
	parent->_parent = subR;

	//建立subR与parent_parent之间的联系
	//有可能parent是根节点，需要判断
	if (parent_parent == nullptr)
	{
		_root = subR;
		_root->_parent = nullptr;
	}
	else
	{
		if (parent == parent_parent->_left)
		{
			parent_parent->_left = subR;
		}
		else
		{
			parent_parent->_right = subR;
		}
		subR->_parent = parent_parent;
	}
}

template<class K, class V>
void RB_Tree<K, V>::right_rotate(Node* parent)
{
	Node* subL = parent->_left;
	Node* subLR = subL->_right;
	Node* parent_parent = parent->_parent;

	//建立subLR与parent之间的联系
	parent->_left = subLR;
	if (subLR) 
		subLR->_parent = parent;

	//建立parent与subL之间的联系
	subL->_right = parent;
	parent->_parent = subL;

	//建立parent_parent与subL之间的联系
	if (parent_parent == nullptr)
	{
		_root = subL;
		subL->_parent = nullptr;//subL就是_root,不过写subL似乎更直观些??
	}
	else
	{
		if (parent_parent->_left == parent)
		{
			parent_parent->_left = subL;
		}
		else if (parent_parent->_right == parent)
		{
			parent_parent->_right = subL;
		}
		subL->_parent = parent_parent;
	}
}

//右左旋或者左右旋 本质都是首先将grandfather parent child由折线形转换为直线形，再调整为对称的三角形
template<class K, class V>
void RB_Tree<K, V>::left_right_rotate(Node* parent)
{
	//参考资源文件中的左旋右旋png
	//传入的参数为grandparent，则首先对parent进行左旋，左旋后再对grandparent进行右旋
	left_rotate(parent->_left);
	right_rotate(parent);
}

template<class K, class V>
void RB_Tree<K, V>::right_left_rotate(Node* parent)
{
	right_rotate(parent->_right);
	left_rotate(parent);
}

//模板成员函数显式初始化
template pair<typename RB_Tree<int, int>::Node*, bool> RB_Tree<int, int>::insert(const pair<int, int>& data);
template<class K,class V>
pair<typename RB_Tree<K,V>::Node*,bool> RB_Tree<K, V>::insert(const pair<K, V>& data)
{
	//若为空树
	if (_root == nullptr)
	{
		_root = new Node(data);
		_root->_color = BLACK;
		return make_pair(_root, true);
	}
	
	//1.按二叉搜索树的插入方法，找到待插入位置
	Node* cur = _root;
	Node* parent = nullptr;
	while (cur)
	{
		if (data.first < cur->_data.first)
		{
			parent = cur;
			cur = cur->_left;
		}
		else if (data.first > cur->_data.first)
		{
			parent = cur;
			cur = cur->_right;
		}
		else
		{
			//key值相等，插入失败
			return make_pair(cur, false);
		}
	}

	//2.将待插入节点插入到树中
	cur = new Node(data);
	//记录新插入的结点（便于后序返回）
	Node* new_node = cur;
	if (data.first < parent->_data.first)
	{
		parent->_left = cur;
		cur->_parent = parent;
	}
	else
	{
		parent->_right = cur;
		cur->_parent = parent;
	}

	//3.若插入节点的父节点是红色的，则需要对红黑树进行调整
	while (parent && parent->_color == RED)
	{
		Node* grandfather = parent->_parent;
		if (parent = grandfather->_left)
		{
			Node* uncle = grandfather->_right;
			
			//uncle存在且为红色
			if (uncle && uncle->_color == RED)
			{
				//颜色调整
				parent->_color = uncle->_color = BLACK;
				grandfather->_color = RED;

				//继续往上处理
				cur = grandfather;
				parent = cur->_parent;
			}
			//uncle不存在  或者  uncle存在但为红色
			else
			{
				//若祖孙三代的关系是直线，即cur、parent、grandfather这三个结点为一条直线，则我们需要先进行单旋操作，再进行颜色调整
				if (cur == parent->_left)
				{
					right_rotate(grandfather);

					grandfather->_color = RED;
					parent->_color = BLACK;
				}
				//若祖孙三代的关系是折线，即cur、parent、grandfather这三个结点为一条折线，则我们需要先进行双旋操作，再进行颜色调整，
				else if (cur == parent->_right)
				{
					left_right_rotate(grandfather);

					grandfather->_color = RED;
					cur->_color = BLACK;
				}
				//子树旋转后，该子树的根变成了黑色，无需继续往上进行处理
				break;
			}
		}
		//parent是grandfather的右孩子
		else if(parent=grandfather->_right)
		{
			Node* uncle = grandfather->_left;
			if (uncle && uncle->_color == RED)
			{
				uncle->_color = parent->_color = BLACK;
				grandfather->_color = RED;

				//继续往上处理
				cur = grandfather;
				parent = cur->_parent;
			}
			//uncle不存在 + uncle存在且为黑
			else
			{
				if (cur == parent->_left)
				{
					right_left_rotate(grandfather);

					cur->_color = BLACK;
					grandfather->_color = RED;
				}
				else if (cur == parent->_right)
				{
					left_rotate(grandfather);

					grandfather->_color = RED;
					parent->_color = BLACK;
				}
				//子树旋转后，该子树的根变成了黑色，无需继续往上进行处理
				break;
			}
		}
	}
	_root->_color = BLACK;
	return make_pair(new_node, true);
}

template bool RB_Tree<int, int>::erase(const int& key);
template<class K,class V>
bool RB_Tree<K, V>::erase(const K& key)
{
	/*找结点的过程与二叉搜索树寻找待删除结点的方法一样，若找到的待删除结点的左右子树均不为空，则需要使用替换法进行删除。
	因此我们最终需要删除的都是左右子树至少有一个为空的结点。
	找到实际待删除结点后，先不删除该结点，否则调整红黑树时不容易控制，找到实际待删除结点后立即进行红黑树的调整。*/

	//遍历二叉树
	Node* parent = nullptr;
	Node* cur = _root;
	//标记实际的待删除节点及其父节点
	Node* del_node_parent = nullptr;
	Node* del_node = nullptr;

	while (cur)
	{
		if (key < cur->_data.first)
		{
			parent = cur;
			cur = cur->_left;
		}
		else if (key > cur->_data.first)
		{
			parent = cur;
			cur = cur->_right;
		}
		//找到了待删除节点
		else
		{
			//待删除结点的左子树为空
			if (cur->_left == nullptr)
			{
				if (cur == _root)
				{
					//让根结点的右子树作为新的根结点
					_root = _root->_right;
					if (_root)
					{
						_root->_parent = nullptr;
						_root->_color = BLACK;
					}
					delete cur;
					return true;
				}
				else
				{
					del_node_parent = parent;
					del_node = cur;
				}
				//进行红黑树的调整以及结点的实际删除
				break;
			}
			//待删除结点的右子树为空
			else if (cur->_right = nullptr)
			{
				if (cur == _root)
				{
					//让根结点的左子树作为新的根结点
					_root = _root->_left;
					if (_root)
					{
						_root->_parent = nullptr;
						_root->_color = BLACK;
					}
					delete cur;
					return true;
				}
				else
				{
					del_node_parent = parent;
					del_node = cur;
				}
				break;
			}
			//待删除结点的左右子树均不为空
			else
			{
				//替换法删除
				//寻找待删除节点右子树当中key值最小的节点作为实际删除结点
				Node* min_parent = cur;
				Node* min_right = cur->_right;
				while (min_right->_left)
				{
					min_parent = min_right;
					min_right = min_right->_left;
				}
				//将找到的节点的data值复制过来 并标记实际删除的节点后break
				cur->_data.first = min_right->_data.first;
				cur->_data.second = min_right->_data.second;
				del_node_parent = min_parent;
				del_node = min_right;
				break;
			}
		}
	}
	//del_node为空节点，说明没有找到待删除结点
	if (del_node == nullptr) return false;

	/*调整红黑树之前，我们先判断一下本次结点的删除是否会破坏了红黑树的性质，
	若破坏了我们才需要对红黑树进行调整。
	若实际删除的结点是红色结点，那么本次删除操作不会破坏红黑树的性质，因此我们不需要对红黑树进行调整。
	若删除的结点是黑色结点，因为黑色结点的删除将会使得一些路径中黑色结点的数目减少，我们就需要对红黑树进行调整。*/

	//记录待删除结点及其父结点,用于后续实际删除
	Node* del = del_node;
	Node* del_parent = del_node_parent;

	//调整红黑树
	if (del_node->_color == BLACK)
	{
		//此时待删除节点最多只有一个孩子（见上寻找过程），且如果有孩子必为红色
		if (del_node->_left)
			del_node->_left->_color = BLACK;
		else if (del_node->_right)
			del_node->_right->_color = BLACK;
		else
		{
			//可能一直调整到根结点
			while (del_node != _root)
			{
				if (del_node == del_node_parent->_left)
				{
					Node* brother = del_node_parent->_right;
					//情况一：brother为红色
					if (brother->_color == RED)
					{
						del_node_parent->_color = RED;
						brother->_color = BLACK;
						left_rotate(del_node_parent);
						
						//需要继续处理，并更新brother，否则在本循环中执行其他情况的代码会出错
						brother = del_node_parent->_right;
					}
					//情况二：brother为黑色，且其左右孩子都是黑色结点或为空
					if ((brother->_left == nullptr || brother->_left->_color == BLACK)
						&& (brother->_right == nullptr || brother->_right->_color == BLACK))
					{
						brother->_color = RED;
						if (del_node_parent->_color == RED)
						{
							del_node_parent->_color = BLACK;
							break;
						}
						del_node = del_node_parent;
						del_node_parent = del_node->_parent;
					}
					//情况三：brother为黑色，且其左孩子是红色结点，右孩子是黑色结点或为空
					else
					{
						if (brother->_right == nullptr || brother->_right->_color == BLACK)
						{
							brother->_left->_color = BLACK;
							brother->_color = RED;
							right_rotate(brother);
							// 继续处理 更新brother，否则执行下面情况四的代码会出错
							brother = del_node_parent->_right;
						}
					}
					//情况四：brother为黑色，且其右孩子是红色结点
					brother->_color = del_node_parent->_color;
					del_node_parent->_color = BLACK;
					brother->_right->_color = BLACK;
					left_rotate(del_node_parent);
					//情况四执行完毕后调整一定结束
					break;
				}
				else if (del_node = del_node_parent->_right)
				{
					Node* brother = del_node_parent->_right;
					//情况一：brother为红色
					if (brother->_color == RED)
					{
						del_node_parent->_color = RED;
						brother->_color = BLACK;
						right_rotate(del_node_parent);

						//继续处理，更新brother
						brother = del_node_parent->_left;
					}
					//情况二：brother为黑色，且其左右孩子都是黑色结点或为空
					if (((brother->_left == nullptr) || (brother->_left->  _color == BLACK))
						&& ((brother->_right == nullptr) || (brother->_right->_color == BLACK)))
					{
						brother->_color = RED;
						if (del_node_parent->_color == RED)
						{
							del_node_parent->_color = BLACK;
							break;
						}
						//需要继续处理
						del_node = del_node_parent;
						del_node_parent = del_node->_parent;
					}
					else
					{
						//情况三：brother为黑色，且其右孩子是红色结点，左孩子是黑色结点或为空
						if ((brother->_left == nullptr) || (brother->_left->_color == BLACK))
						{
							brother->_right->_color = BLACK;
							brother->_color = RED;
							left_rotate(brother);
							//需要继续处理
							brother = del_node_parent->_left; //更新brother（否则执行下面情况四的代码会出错）
						}
						//情况四：brother为黑色，且其左孩子是红色结点
						brother->_color = del_node_parent->_color;
						del_node_parent->_color = BLACK;
						brother->_left->_color = BLACK;
						right_rotate(del_node_parent);
						break; //情况四执行完毕后调整一定结束
					}
				}
			}
		}
	}

	//开始实际删除
	if (del->_left == nullptr)
	{
		if (del == del_parent->_left)
		{
			del_parent->_right = del->_right;
			if (del->_right)
				del->_right->_parent = del_parent;
		}
		else 
		{
			del_parent->_right = del->_right;
			if (del->_right)
				del->_right->_parent = del_parent;
		}
	}
	else
	{
		if (del == del_parent->_left)
		{
 			del_parent->_left = del->_left;
			if (del->_left)
				del->_left->_parent = del_parent;
		}
		else
		{
			del_parent->_right = del->_left;
			if (del->_left)
				del->_left->_parent = del_parent;
		}
	}
	delete del;
	return true;
}