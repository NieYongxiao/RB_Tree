#pragma once
#ifndef RB_TREE_H
#define RB_TREE_H

#include<utility>
using namespace std;

//枚举定义结点的颜色
enum Color
{
	RED,
	BLACK
};


template<class K, class V>
struct RB_Tree_Node
{
	//构造函数
	RB_Tree_Node(const pair<K, V>& data)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _data(data)
		, _color(RED)
	{}

	//三叉链
	RB_Tree_Node<K, V>* _left;
	RB_Tree_Node<K, V>* _right;
	RB_Tree_Node<K, V>* _parent;

	//存储的键值对
	pair<K, V> _data;

	//结点的颜色
	int _color; //红/黑
};

template<class K,class V>
class RB_Tree {
public:
	typedef RB_Tree_Node<K, V> Node;

	//插入
	pair<Node*, bool> insert(const pair<K, V>& data);

	//删除
	bool erase(const K& k); 


	//根据key进行查找
	Node* find(const K& key);

	//判断是不是红黑树
	bool is_balance();
private:
	Node* _root=nullptr;
	bool _is_balance(Node* node, int count, const int& black_cnt);

	//左旋
	void left_rotate(Node* parent);
	//右旋 
	void right_rotate(Node* parent);
	//左右双旋
	void left_right_rotate(Node* parent);
	//右左双旋
	void right_left_rotate(Node* parent);
};
#endif