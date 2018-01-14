#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct nd
{
	void *data; /* pointer to the data */
	struct nd *next; /* pointer to the next struct nd in the list */
};
struct ls
{
	unsigned int length; /* length of the list (total items in the list) */
	Node first; /* pointer to the first struct nd of the list for fast insertion */	
	Node last; /* pointer to the final struct nd of the list for fast insertion */	
};

List List_init()
{
	List list = malloc(sizeof(struct ls));
	list->first = NULL;
	list->last = NULL;
	list->length = 0;

	return list;
}

bool List_isEmpty(List list)
{
	return list->length == 0;
}

void List_insert(List list, void *data)
{
	if(List_isEmpty(list))
	{
		list->first = malloc(sizeof(struct nd));
		list->first->data = data;
		list->first->next = NULL;
		list->last = list->first;
	}
	else
	{
		list->last->next = malloc(sizeof(struct nd));
		list->last->next->data = data;
		list->last->next->next = NULL;
		list->last = list->last->next;
	}
	list->length++;
}

void List_insert_front(List list, void *data)
{
	if(List_isEmpty(list))
	{
		list->first = malloc(sizeof(struct nd));
		list->first->data = data;
		list->first->next = NULL;
		list->last = list->first;
	}
	else
	{
		Node new_node = malloc(sizeof(struct nd));
		new_node->data = data;
		new_node->next = list->first;
		list->first = new_node;
	}
	list->length++;
}

void List_print(List list, FILE *stream, void (*print_func)(void *, FILE *))
{
	Node curNode = list->first;
	fprintf(stream, "[");
	if(print_func!=NULL)
	{
		while(curNode!=NULL)
		{
			if(curNode->data != NULL)
			{
				print_func((void *)(curNode->data), stream);
				if(curNode != List_get_last_node(list))
				{
					fprintf(stream, ", ");
				}
			}
			curNode = curNode->next;
		}
	}
	fprintf(stream, "]");
}

/* deletes a specific element from the list */
bool List_delete(List list, void *data, bool (* compare_func)(void *, void *))
{
	if(!List_isEmpty(list))
	{
		Node curNode = list->first;
		Node prev = NULL;
		while(curNode != NULL)
		{
			if(compare_func(curNode->data, data))
			{
				if(prev != NULL)
				{
					prev->next = curNode->next;
				}
				free(curNode);
				curNode = NULL;
				list->length--;
				return true;
			}
			prev = curNode;
			curNode = curNode->next;
		}
		return false;
	}
	return false;
}

/* destroys the whole list */
void List_destroy(List list, void (*delete_func)(void *))
{
	if(List_isEmpty(list))
	{
		free(list);
		list = NULL;
	}
	else
	{
		Node curNode = list->first;
		Node nextNode = curNode->next;
		while(nextNode!=NULL)
		{
			/* optionally destroy the element the node contains */
			if(delete_func != NULL)
			{
				delete_func(curNode->data);
			}
			free(curNode);
			curNode = NULL;
			curNode = nextNode;
			nextNode = nextNode->next;
			list->length--;
		}
		/* optionally destroy the element the node contains */
		if(delete_func != NULL)
		{
			delete_func(curNode->data);
		}
		free(curNode);
		curNode = NULL;
		list->length--;

		list->first = NULL;
		list->last = NULL;

		free(list);
		list = NULL;
	}
}

List List_copy(List list)
{
	List new_list = List_init();
	if(List_isEmpty(list))
	{
		return new_list;
	}

	Node cur_node = list->first;
	while(cur_node != NULL)
	{
		List_insert(new_list, cur_node->data);
		cur_node = cur_node->next;
	}
	return new_list;
}

Node List_get_first_node(List list)
{
	return list->first;
}
Node List_get_last_node(List list)
{
	return list->last;
}
unsigned int List_get_length(List list)
{
	return list->length;
}
Node Node_get_data(Node node)
{
	return node->data;
}
Node Node_get_next(Node node)
{
	return node->next;
}

void Node_set_data(Node node, void *data)
{
	node->data = data;
}
