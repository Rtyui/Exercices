#include "node.h"
#include <stdio.h>
#include <stdlib.h>
void print_node(int val)
{
	printf("%d ", val);
}

void sanity_check(struct node *first_node)
{
	if(first_node == NULL)
		fprintf(stderr, "First node is null!\n");
}

struct node* create_node(int value)
{
	struct node *new_node = (struct node*)malloc(sizeof(struct node));
	if(new_node == NULL)
		fprintf(stderr, "Alloc failed!\n");

	new_node->next = NULL;
	new_node->cllb_print = print_node;
	new_node->val = value;
	return new_node;
}

void add_node(struct node **first_node, struct node *new_node, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	if(*first_node == NULL){
		*first_node = new_node;
		return;
	}

	struct node *iterator = *first_node;
	while(iterator->next) {
		iterator = iterator->next;
	}

	iterator->next = new_node;
	pthread_mutex_unlock(mutex);

}

void delete_node(struct node **first_node, int value, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	struct node *iterator = *first_node;
	while(iterator && iterator->val != value)
		iterator = iterator->next;

	if(iterator == (*first_node)) {
		*first_node = (*first_node)->next;
		free(iterator);
	} else if(iterator != NULL) {
		struct node *deleted = iterator;
		iterator = (*first_node);
		while(iterator->next != deleted)
			iterator = iterator->next;
		iterator->next = iterator->next->next;
		free(deleted);
	}
	pthread_mutex_unlock(mutex);
}

void print_list(struct node *first_node, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	if(first_node == NULL) {
		printf("The list is empty!\n");
		return;
	}

	struct node *iterator = first_node;
	while(iterator)
	{
		iterator->cllb_print(iterator->val);
		iterator = iterator->next;
	}
	printf("\n");
	pthread_mutex_unlock(mutex);
}

void sort_list(struct node **first_node, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	int *v = NULL;
	int size = 0;
	struct node *iterator = *first_node;
	while(iterator) {
		++size;
		iterator = iterator->next;
	}

	v = (int *)malloc(size * sizeof(int));
	iterator = *first_node;
	int i = 0, j = 0;
	while(iterator) {
		v[i++] = iterator->val;
		iterator = iterator->next;
	}

	int aux_int = 0;
	for(i = 0; i < size - 1; ++i) {
		for(j = i + 1; j < size; ++j) {
			if(v[i] > v[j]) {
				aux_int = v[i];
				v[i] = v[j];
				v[j] = aux_int;
			}
		}
	}

	struct node *aux = NULL;
	struct node *last_added = NULL;
	for(i = size - 1; i >= 0; --i)
	{
		iterator = *first_node;

		while(iterator->next && iterator->next->val != v[i])
			iterator = iterator->next;

		if(iterator->next) {
			aux = iterator->next;
			iterator->next = iterator->next->next;	
		} else {
			aux = *first_node;
			*first_node = (*first_node)->next;
		}
		aux->next = last_added;
		last_added = aux;
		if(i == 0)
			*first_node = last_added;
	}
	pthread_mutex_unlock(mutex);
}

void flush_list(struct node **first_node, pthread_mutex_t *mutex)
{
	print_list(*first_node, mutex);
	while(*first_node)
		delete_node(first_node, 0, mutex);
}