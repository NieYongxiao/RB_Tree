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
			cout << "��ǰ·���ĺ�ɫ�ڵ���ο�ֵ��ͬ" << endl;
			return false;
		}
	}

	//��Ϊ���ڵ�����Ǻڵģ��������ĳ�ڵ�Ϊ��ģ���һ���и��ڵ㣬���Բ����жϸ��ڵ��Ƿ����
	if (root->_color == RED && root->_parent->_color == "RED")
	{
		cout << root->_data.first << " �˽ڵ㼰�丸�ڵ��Ϊ��ɫ" << endl;
		return false;
	}

	if (root->_color == BLACK) ++black_count;

	return _is_balcnce(root->_left, black_count, ref_black_count) && _is_balance(root->_right, black_count, ref_black_count);
}

//ģ���Ա������ʽ��ʼ��
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
	
	//����subRL��parent֮�����ϵ
	parent->_right = subRL;
	if (subRL) subRL->_parent = parent;

	//����parent��subR֮�����ϵ
	subR->_left = parent;
	parent->_parent = subR;

	//����subR��parent_parent֮�����ϵ
	//�п���parent�Ǹ��ڵ㣬��Ҫ�ж�
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

	//����subLR��parent֮�����ϵ
	parent->_left = subLR;
	if (subLR) 
		subLR->_parent = parent;

	//����parent��subL֮�����ϵ
	subL->_right = parent;
	parent->_parent = subL;

	//����parent_parent��subL֮�����ϵ
	if (parent_parent == nullptr)
	{
		_root = subL;
		subL->_parent = nullptr;//subL����_root,����дsubL�ƺ���ֱ��Щ??
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

//���������������� ���ʶ������Ƚ�grandfather parent child��������ת��Ϊֱ���Σ��ٵ���Ϊ�ԳƵ�������
template<class K, class V>
void RB_Tree<K, V>::left_right_rotate(Node* parent)
{
	//�ο���Դ�ļ��е���������png
	//����Ĳ���Ϊgrandparent�������ȶ�parent�����������������ٶ�grandparent��������
	left_rotate(parent->_left);
	right_rotate(parent);
}

template<class K, class V>
void RB_Tree<K, V>::right_left_rotate(Node* parent)
{
	right_rotate(parent->_right);
	left_rotate(parent);
}

//ģ���Ա������ʽ��ʼ��
template pair<typename RB_Tree<int, int>::Node*, bool> RB_Tree<int, int>::insert(const pair<int, int>& data);
template<class K,class V>
pair<typename RB_Tree<K,V>::Node*,bool> RB_Tree<K, V>::insert(const pair<K, V>& data)
{
	//��Ϊ����
	if (_root == nullptr)
	{
		_root = new Node(data);
		_root->_color = BLACK;
		return make_pair(_root, true);
	}
	
	//1.�������������Ĳ��뷽�����ҵ�������λ��
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
			//keyֵ��ȣ�����ʧ��
			return make_pair(cur, false);
		}
	}

	//2.��������ڵ���뵽����
	cur = new Node(data);
	//��¼�²���Ľ�㣨���ں��򷵻أ�
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

	//3.������ڵ�ĸ��ڵ��Ǻ�ɫ�ģ�����Ҫ�Ժ�������е���
	while (parent && parent->_color == RED)
	{
		Node* grandfather = parent->_parent;
		if (parent = grandfather->_left)
		{
			Node* uncle = grandfather->_right;
			
			//uncle������Ϊ��ɫ
			if (uncle && uncle->_color == RED)
			{
				//��ɫ����
				parent->_color = uncle->_color = BLACK;
				grandfather->_color = RED;

				//�������ϴ���
				cur = grandfather;
				parent = cur->_parent;
			}
			//uncle������  ����  uncle���ڵ�Ϊ��ɫ
			else
			{
				//�����������Ĺ�ϵ��ֱ�ߣ���cur��parent��grandfather���������Ϊһ��ֱ�ߣ���������Ҫ�Ƚ��е����������ٽ�����ɫ����
				if (cur == parent->_left)
				{
					right_rotate(grandfather);

					grandfather->_color = RED;
					parent->_color = BLACK;
				}
				//�����������Ĺ�ϵ�����ߣ���cur��parent��grandfather���������Ϊһ�����ߣ���������Ҫ�Ƚ���˫���������ٽ�����ɫ������
				else if (cur == parent->_right)
				{
					left_right_rotate(grandfather);

					grandfather->_color = RED;
					cur->_color = BLACK;
				}
				//������ת�󣬸������ĸ�����˺�ɫ������������Ͻ��д���
				break;
			}
		}
		//parent��grandfather���Һ���
		else if(parent=grandfather->_right)
		{
			Node* uncle = grandfather->_left;
			if (uncle && uncle->_color == RED)
			{
				uncle->_color = parent->_color = BLACK;
				grandfather->_color = RED;

				//�������ϴ���
				cur = grandfather;
				parent = cur->_parent;
			}
			//uncle������ + uncle������Ϊ��
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
				//������ת�󣬸������ĸ�����˺�ɫ������������Ͻ��д���
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
	/*�ҽ��Ĺ��������������Ѱ�Ҵ�ɾ�����ķ���һ�������ҵ��Ĵ�ɾ������������������Ϊ�գ�����Ҫʹ���滻������ɾ����
	�������������Ҫɾ���Ķ�����������������һ��Ϊ�յĽ�㡣
	�ҵ�ʵ�ʴ�ɾ�������Ȳ�ɾ���ý�㣬������������ʱ�����׿��ƣ��ҵ�ʵ�ʴ�ɾ�������������к�����ĵ�����*/

	//����������
	Node* parent = nullptr;
	Node* cur = _root;
	//���ʵ�ʵĴ�ɾ���ڵ㼰�丸�ڵ�
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
		//�ҵ��˴�ɾ���ڵ�
		else
		{
			//��ɾ������������Ϊ��
			if (cur->_left == nullptr)
			{
				if (cur == _root)
				{
					//�ø�������������Ϊ�µĸ����
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
				//���к�����ĵ����Լ�����ʵ��ɾ��
				break;
			}
			//��ɾ������������Ϊ��
			else if (cur->_right = nullptr)
			{
				if (cur == _root)
				{
					//�ø�������������Ϊ�µĸ����
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
			//��ɾ������������������Ϊ��
			else
			{
				//�滻��ɾ��
				//Ѱ�Ҵ�ɾ���ڵ�����������keyֵ��С�Ľڵ���Ϊʵ��ɾ�����
				Node* min_parent = cur;
				Node* min_right = cur->_right;
				while (min_right->_left)
				{
					min_parent = min_right;
					min_right = min_right->_left;
				}
				//���ҵ��Ľڵ��dataֵ���ƹ��� �����ʵ��ɾ���Ľڵ��break
				cur->_data.first = min_right->_data.first;
				cur->_data.second = min_right->_data.second;
				del_node_parent = min_parent;
				del_node = min_right;
				break;
			}
		}
	}
	//del_nodeΪ�սڵ㣬˵��û���ҵ���ɾ�����
	if (del_node == nullptr) return false;

	/*���������֮ǰ���������ж�һ�±��ν���ɾ���Ƿ���ƻ��˺���������ʣ�
	���ƻ������ǲ���Ҫ�Ժ�������е�����
	��ʵ��ɾ���Ľ���Ǻ�ɫ��㣬��ô����ɾ�����������ƻ�����������ʣ�������ǲ���Ҫ�Ժ�������е�����
	��ɾ���Ľ���Ǻ�ɫ��㣬��Ϊ��ɫ����ɾ������ʹ��һЩ·���к�ɫ������Ŀ���٣����Ǿ���Ҫ�Ժ�������е�����*/

	//��¼��ɾ����㼰�丸���,���ں���ʵ��ɾ��
	Node* del = del_node;
	Node* del_parent = del_node_parent;

	//���������
	if (del_node->_color == BLACK)
	{
		//��ʱ��ɾ���ڵ����ֻ��һ�����ӣ�����Ѱ�ҹ��̣���������к��ӱ�Ϊ��ɫ
		if (del_node->_left)
			del_node->_left->_color = BLACK;
		else if (del_node->_right)
			del_node->_right->_color = BLACK;
		else
		{
			//����һֱ�����������
			while (del_node != _root)
			{
				if (del_node == del_node_parent->_left)
				{
					Node* brother = del_node_parent->_right;
					//���һ��brotherΪ��ɫ
					if (brother->_color == RED)
					{
						del_node_parent->_color = RED;
						brother->_color = BLACK;
						left_rotate(del_node_parent);
						
						//��Ҫ��������������brother�������ڱ�ѭ����ִ����������Ĵ�������
						brother = del_node_parent->_right;
					}
					//�������brotherΪ��ɫ���������Һ��Ӷ��Ǻ�ɫ����Ϊ��
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
					//�������brotherΪ��ɫ�����������Ǻ�ɫ��㣬�Һ����Ǻ�ɫ����Ϊ��
					else
					{
						if (brother->_right == nullptr || brother->_right->_color == BLACK)
						{
							brother->_left->_color = BLACK;
							brother->_color = RED;
							right_rotate(brother);
							// �������� ����brother������ִ����������ĵĴ�������
							brother = del_node_parent->_right;
						}
					}
					//����ģ�brotherΪ��ɫ�������Һ����Ǻ�ɫ���
					brother->_color = del_node_parent->_color;
					del_node_parent->_color = BLACK;
					brother->_right->_color = BLACK;
					left_rotate(del_node_parent);
					//�����ִ����Ϻ����һ������
					break;
				}
				else if (del_node = del_node_parent->_right)
				{
					Node* brother = del_node_parent->_right;
					//���һ��brotherΪ��ɫ
					if (brother->_color == RED)
					{
						del_node_parent->_color = RED;
						brother->_color = BLACK;
						right_rotate(del_node_parent);

						//������������brother
						brother = del_node_parent->_left;
					}
					//�������brotherΪ��ɫ���������Һ��Ӷ��Ǻ�ɫ����Ϊ��
					if (((brother->_left == nullptr) || (brother->_left->  _color == BLACK))
						&& ((brother->_right == nullptr) || (brother->_right->_color == BLACK)))
					{
						brother->_color = RED;
						if (del_node_parent->_color == RED)
						{
							del_node_parent->_color = BLACK;
							break;
						}
						//��Ҫ��������
						del_node = del_node_parent;
						del_node_parent = del_node->_parent;
					}
					else
					{
						//�������brotherΪ��ɫ�������Һ����Ǻ�ɫ��㣬�����Ǻ�ɫ����Ϊ��
						if ((brother->_left == nullptr) || (brother->_left->_color == BLACK))
						{
							brother->_right->_color = BLACK;
							brother->_color = RED;
							left_rotate(brother);
							//��Ҫ��������
							brother = del_node_parent->_left; //����brother������ִ����������ĵĴ�������
						}
						//����ģ�brotherΪ��ɫ�����������Ǻ�ɫ���
						brother->_color = del_node_parent->_color;
						del_node_parent->_color = BLACK;
						brother->_left->_color = BLACK;
						right_rotate(del_node_parent);
						break; //�����ִ����Ϻ����һ������
					}
				}
			}
		}
	}

	//��ʼʵ��ɾ��
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