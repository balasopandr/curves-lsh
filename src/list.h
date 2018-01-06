#ifndef __LIST__
#define __LIST__
#include "global_defs.h"

typedef struct nd *Node;
typedef struct ls *List;

List List_init();
bool List_isEmpty(List list);
void List_insert(List list, void *data);
void List_insert_front(List list, void *data);
void List_print(List list, FILE *stream, void (*print_func)(void *, FILE *));
bool List_delete(List list, void *data, bool (* compare_func)(void *, void *));
void List_destroy(List list, void (*delete_func)(void *));
Node List_get_first_node(List list);
Node List_get_last_node(List list);
unsigned int List_get_length(List list);
Node Node_get_data(Node node);
Node Node_get_next(Node node);
List List_copy(List list);
#endif
