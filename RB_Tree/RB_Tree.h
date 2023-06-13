#pragma once
#ifndef RB_TREE_H
#define RB_TREE_H

#include<utility>
using namespace std;

//ö�ٶ��������ɫ
enum Color
{
	RED,
	BLACK
};


template<class K, class V>
struct RB_Tree_Node
{
	//���캯��
	RB_Tree_Node(const pair<K, V>& data)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _data(data)
		, _color(RED)
	{}

	//������
	RB_Tree_Node<K, V>* _left;
	RB_Tree_Node<K, V>* _right;
	RB_Tree_Node<K, V>* _parent;

	//�洢�ļ�ֵ��
	pair<K, V> _data;

	//������ɫ
	int _color; //��/��
};

template<class K,class V>
class RB_Tree {
public:
	typedef RB_Tree_Node<K, V> Node;

	//����
	pair<Node*, bool> insert(const pair<K, V>& data);

	//ɾ��
	bool erase(const K& k); 


	//����key���в���
	Node* find(const K& key);

	//�ж��ǲ��Ǻ����
	bool is_balance();
private:
	Node* _root=nullptr;
	bool _is_balance(Node* node, int count, const int& black_cnt);

	//����
	void left_rotate(Node* parent);
	//���� 
	void right_rotate(Node* parent);
	//����˫��
	void left_right_rotate(Node* parent);
	//����˫��
	void right_left_rotate(Node* parent);
};
#endif