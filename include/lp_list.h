/*
	Loprin Single Header Library
	#define LP_LIST_IMPLEMENTATION before using
	if you dont want to get undefined reference to ...
*/

#ifndef __LP_LIST_H__
#define __LP_LIST_H__

#include "lp_common.h"
#include <string.h>
#include <stdlib.h>

typedef struct llist_node_t {
	struct llist_node_t* next;
	void* data;
	size_t __elem_size;
} llist_node;

typedef struct llist_t {
	llist_node* first;
	llist_node* last;
	size_t length;
} llist;

/*
	Allocates memory for new node and its data
	Copies data from source location to node data
*/
llist_node* llist_node_alloc(void* data, size_t data_size);

/*
	Releases node and node data memory
*/
void llist_node_free(llist_node* node);

/*
	Create new linked list
	Allocates memory for new list and inits that
*/
llist* llist_init();

/*
	Allocates memory for new node and pushes it to the end of list
*/
void llist_push_back(llist* l, void* data, size_t elem_size);

/*
	Allocates memory for new node and pushes it to the begin of list
*/
void llist_push_front(llist* l, void* data, size_t elem_size);

/// @brief Creates new node and pushes that after 'index' position
/// @param index index of element after which new node should be inserted
void llist_push_after(llist* l, size_t index, void* data, size_t elem_size);

/*
	Deletes last element of list
*/
void llist_pop_back(llist* l);
void llist_pop_front(llist* l);

/// @brief Deletes element with 'index'
/// @param index index of element that should to be deleted
void llist_pop_at(llist* l, size_t index);

/*
	Replace data of element with 'index',
	if elem_size != node->__elem_size -> deletes old data and allocates new memory
	for new data entry
*/
void llist_replace_at(llist* l, void* data, size_t elem_size, size_t index);

/*
	Get data of node with 'index'
*/
void llist_at(llist* l, void* data, size_t elem_size, size_t index);

/*
	Deletes all nodes of list
*/
void llist_clear(llist* l);

/*
	Deletes nodes of list and
	releases memory that was occupied by list
*/
void llist_release(llist* l);


#ifdef LP_LIST_IMPLEMENTATION


llist_node* llist_node_alloc(void* data, size_t data_size)
{
	// Allocate memory for node
	llist_node* node = (llist_node*)malloc(sizeof(llist_node));
	if (!node)
	{
		LP_LOG_ERR("Could not allocate memory for node");
		return NULL;
	}

	// Copy data to node data field
	node->data = calloc(1, data_size);
	if (!node->data)
	{
		LP_LOG_ERR("Could not allocate memory for node data");
		free(node);
		return NULL;
	}
	memcpy(node->data, data, data_size);

	node->__elem_size = data_size;

	return node;
};

void llist_node_free(llist_node* node)
{
	if (node)
	{
		if (node->data)
			free(node->data);
		free(node);
	}
};


llist* llist_init()
{
	llist* l = (llist*)malloc(sizeof(llist));

	if (!l)
	{
		LP_LOG_ERR("Could not allocate memory for linked list");
		return NULL;
	}

	l->first = NULL;
	l->last = NULL;
	l->length = 0;

	return l;
};

void llist_push_back(llist* l, void* data, size_t elem_size)
{
	llist_node* node = llist_node_alloc(data, elem_size);
	if (!node)
		return;

	// Update pointers
	// Last element of list points to null
	node->next = NULL;
	if (l->length == 0)
		// If this node is last
		l->first = node;
	else
		// If this node has previous node
		l->last->next = node;
	l->last = node;
	l->length++;
};

void llist_push_front(llist* l, void* data, size_t elem_size)
{
	llist_node* node = llist_node_alloc(data, elem_size);
	if (!node)
		return;

	// Update pointers
	node->next = l->first;
	l->first = node;

	l->length++;
};

void llist_push_after(llist* l, size_t index, void* data, size_t elem_size)
{
	// If index >= length - push back
	if (index >= l->length - 1)
	{
		llist_push_back(l, data, elem_size);
		return;
	}

	llist_node* node = llist_node_alloc(data, elem_size);
	if (!node)
		return;

	// Update poiners
	llist_node* prev = l->first;
	for (size_t i = 0; i < index; ++i)
		prev = prev->next;

	node->next = prev->next;
	prev->next = node;

	l->length++;
};

void llist_pop_back(llist* l)
{
	// Node to delete
	llist_node* node;
	if (l->length == 0)
		return;
	else if (l->length == 1)
	{
		node = l->first;
		llist_node_free(node);
		l->first = NULL;
		l->last = NULL;
		l->length--;
		return;
	}
	// Previous node from one to be deleted
	llist_node* prev = l->first;

	// Find penultimate node
	while (prev->next->next != NULL)
		prev = prev->next;
	
	// Penultimate now is last one -> it points to NULL
	prev->next = NULL;
	
	// Delete last node
	node = l->last;
	llist_node_free(node);

	// Set last element to list 'last' poiner
	l->last = prev;

	l->length--;
};

/*
	Delete element of the beginning of list
*/
void llist_pop_front(llist* l)
{
	llist_node* node;
	if (l->length == 0)
		return;
	else if (l->length == 1)
	{
		node = l->first;
		llist_node_free(node);
		l->first = NULL;
		l->last = NULL;
		l->length--;
		return;
	}
	node = l->first;
	l->first = l->first->next;
	llist_node_free(node);
	l->length--;
};


void llist_pop_at(llist* l, size_t index)
{
	if (l->length == 0)
		return;

	if (index >= l->length)
	{
		llist_pop_back(l);
		return;
	}
	else if (index == 0)
	{
		llist_pop_front(l);
		return;
	}
	else if (index == l->length - 1)
	{
		llist_pop_back(l);
		return;
	}

	// Node to delete
	llist_node* node = l->first;
	// Previous node from what needs to be deleted
	llist_node* prev = NULL;
	for (size_t i = 0; i < index; i++)
	{
		node = node->next;
		prev = node;
	}
	
	// Break connection to deletion node
	prev->next = node->next;
	// Free node and data
	llist_node_free(node);

	l->length--;
};

void llist_replace_at(llist* l, void* data, size_t elem_size, size_t index)
{
	if (l->length == 0)
		return;

	if (index >= l->length)
	{
		LP_LOG_ERR("Out of range");
		return;
	}

	llist_node* node = l->first;
	// If list length > 1 
	// and index not points to end of list or last element of list
	// iterate on nodes to get specified node
	if (l->length > 1 && index != 0 && index != l->length - 1)
		for (size_t i = 0; i < index; ++i)
			node = node->next;
	// if index points to the end of list -> get last element
	else if (index == l->length - 1)
		node = l->last;

	if (node->data)
	{
		if (node->__elem_size == elem_size)
			memcpy(node->data, data, node->__elem_size);
		else
		{
			void* new_data = calloc(1, elem_size);
			if (!new_data)
			{
				LP_LOG_ERR("Cannot reallocate memory for node");
				return;
			};
			memcpy(new_data, data, elem_size);
			free(node->data);
			node->data = new_data;
		}
	}
};


void llist_at(llist* l, void* data, size_t elem_size, size_t index)
{
	if (l->length == 0)
		return;

	if (index >= l->length)
	{
		LP_LOG_ERR("Out of range");
		return;
	}
	llist_node* node = l->first;

	// If list length > 1 
	// and index not points to end of list or last element of list
	// iterate on nodes to get specified node
	if (l->length > 1 && index != 0 && index != l->length - 1)
		for (size_t i = 0; i < index; ++i)
			node = node->next;
	// if index points to the end of list -> get last element
	else if (index == l->length - 1)
		node = l->last;
	
	// If node has data - copy it to variable
	if (node->data)
	{
		memcpy(data, node->data, elem_size);
	}
	else
	{
		LP_LOG_ERR("Node has not any data");
	}
}

void llist_clear(llist *l)
{
	if (l)
	{
		if (l->length > 0)
		{

			// Node to delete
			llist_node* node = l->first;
			
			// Next node from the one to be deleted
			llist_node* nnode;

			// Release all nodes
			while (node)
			{
				nnode = node->next;
				llist_node_free(node);
				node = nnode;
			};

			// Update list variables
			l->length = 0;
			l->first = NULL;
			l->last = NULL;
		}
	}
};

void llist_release(llist* l)
{
	if (l)
	{
		llist_clear(l);

		// Release list
		free(l);
	}
}

#endif

#endif // __LP_LIST_H__