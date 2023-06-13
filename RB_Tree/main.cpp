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
	//这个不算分离编译，因为在同一文件中
	int i = 1, j = 2;
	Obj<int> obj;
	cout << obj.add(i, j);
}

int main() 
{
	RB_Tree<int, int> rb_tree;
	pair<int, int> p(1, 1);
	//分离式编译在使用模板函数的时候需要在源文件中对模板函数进行显示声明  
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