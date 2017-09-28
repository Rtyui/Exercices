#ifndef _NODE_H_
#define _NODE_H_
#include <pthread.h>

typedef void (*callback_print)(int val);
void print_node(int val);

struct node {
	struct node *next;
	callback_print cllb_print;
	int val;
};

void sanity_check(struct node *first_node);
struct node* create_node(int value);
void add_node(struct node **first_node, struct node *new_node, pthread_mutex_t *mutex);
void delete_node(struct node **first_node, int pos, pthread_mutex_t *mutex);
void print_list(struct node *first_node, pthread_mutex_t *mutex);
void sort_list(struct node **first_node, pthread_mutex_t *mutex);
void flush_list(struct node **first_node, pthread_mutex_t *mutex);


#endif