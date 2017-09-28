#include <stdio.h>
#include "node.h"

pthread_barrier_t barrier;

struct params {
	struct node *first_node;
	pthread_mutex_t *mutex;
};

void* job_t1(void *params)
{
	struct params *pars = (struct params *)params;
	struct node **first_node = &(pars->first_node);
	pthread_barrier_wait(&barrier);


	add_node(first_node, create_node(4), pars->mutex);
	add_node(first_node, create_node(10), pars->mutex);
	delete_node(first_node, 2, pars->mutex);
	sort_list(first_node, pars->mutex);
	delete_node(first_node, 10, pars->mutex);
	delete_node(first_node, 5, pars->mutex);

}

void* job_t2(void *params)
{
	struct params *pars = (struct params *)params;
	struct node **first_node = &(pars->first_node);

	pthread_barrier_wait(&barrier);
	
	add_node(first_node, create_node(11), pars->mutex);
	add_node(first_node, create_node(1), pars->mutex);
	delete_node(first_node, 11, pars->mutex);
	add_node(first_node, create_node(8), pars->mutex);
	print_list(*first_node, pars->mutex);
}

void* job_t3(void *params)
{
	struct params *pars = (struct params *)params;
	struct node **first_node = &(pars->first_node);

	pthread_barrier_wait(&barrier);
	
	add_node(first_node, create_node(30), pars->mutex);
	add_node(first_node, create_node(25), pars->mutex);
	add_node(first_node, create_node(100), pars->mutex);
	sort_list(first_node, pars->mutex);
	print_list(*first_node, pars->mutex);
	delete_node(first_node, 100, pars->mutex);
}

int main()
{
	struct node *first_node = NULL;
	pthread_mutex_t *mutex;

	pthread_t t1, t2, t3;
	struct params par;

	par.first_node = first_node;
	par.mutex = mutex;


	pthread_barrier_init(&barrier, NULL, 3);
	pthread_mutex_init(mutex, NULL);

	pthread_create(&t1, NULL, &job_t1, &par);
	pthread_create(&t2, NULL, &job_t2, &par);
	pthread_create(&t3, NULL, &job_t3, &par);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);


	pthread_barrier_destroy(&barrier);
	return 0;
}