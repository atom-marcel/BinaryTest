#pragma once

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

template<typename T>
class Node
{
public:
	T value;
	Node<T>* next;

	Node(T notation) :value(notation), next(NULL) {};

	int add(T value) 
	{
		Node<T>* node = this;
		while (node->next != NULL)
		{
			node = node->next;
		}
		node->next = new Node(value);

		return 0;
	};

	Node<T>* remove(int index)
	{
		Node<T> *n = this;
		Node<T>* root = this;
		Node<T>* prev = NULL;
		if (index == 0)
		{
			Node<T>* next = n->next;
			return next;
		}
		for (int i = 0; i < index; i++)
		{
			if (n == NULL) return NULL;
			prev = n;
			n = n->next;
		}
		prev->next = n->next;
		delete(n);
		return root;
	};

	int length()
	{
		Node<T>* node = this;
		int i = 1;
		while (node->next != NULL)
		{
			i++;
			node = node->next;
		}

		return i;
	};

	Node<T>* reverse()
	{
		Node<T> *rev = NULL;
		Node<T> *prev = NULL;
		for (Node<T> *n = this; n != NULL; n = n->next)
		{
			prev = rev;
			rev = new Node<T>(n->value);
			rev->next = prev;
		}
		return rev;
	};

	int find(T value_find)
	{
		Node* node = this;
		int index = 0;
		while (node != NULL)
		{
			if (node->value == value_find) return index;
			index++;
			node = node->next;
		}

		return -1;
	};

	T get(int index)
	{
		Node<T>* current = this;
		for (int i = 0; i < index; i++)
		{
			current = current->next;
		}
		return current->value;
	};

	T operator[](int i)
	{
		return this->get(i);
	}

	void print()
	{
		for (Node<T>* node = this; node != NULL; node = node->next)
		{
			if (node->next == NULL)
			{
				std::cout << "(" << node->value << ")\n";
			}
			else
			{
				std::cout << "(" << node->value << "), ";
			}
		}
	};

	std::string get_string()
	{
		std::string s("");
		for (Node<T>* node = this; node != NULL; node = node->next)
		{
			if (node->next == NULL)
			{
				s += "(" + std::to_string(node->value) + ")\n";
			}
			else
			{
				s += "(" + std::to_string(node->value) + "), ";
			}
		}
		return s;
	};
};

#endif