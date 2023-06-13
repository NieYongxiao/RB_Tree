#include<iostream>
#include<vector>
#include"RB_Tree.h"
using namespace std;

template<class T>
class Obj {
public:
	typedef T value_type;
	value_type add(value_type n1, value_type n2);
};

template<class T>
typename Obj<T>::value_type Obj<T>::add(value_type n1, value_type n2)
{
	return n1 + n2;
}

void test()
{
	//������������룬��Ϊ��ͬһ�ļ���
	int i = 1, j = 2;
	Obj<int> obj;
	cout << obj.add(i, j);
}

int main() 
{
	RB_Tree<int, int> rb_tree;
	pair<int, int> p(1, 1);
	//����ʽ������ʹ��ģ�庯����ʱ����Ҫ��Դ�ļ��ж�ģ�庯��������ʾ����  
	rb_tree.insert(p);
	if (auto it = rb_tree.find(1))
		cout << it->_data.first << endl;
	else cout << "nullptr" << endl;
	rb_tree.erase(1);
	if (auto it = rb_tree.find(1))
		cout << it->_data.first << endl;
	else cout << "nullptr" << endl;
	return 0;
} 