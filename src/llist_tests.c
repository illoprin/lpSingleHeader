#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LP_MATH_IMPLEMENTATION
#define LP_LIST_IMPLEMENTATION

#include <lp_common.h>
#include <lp_math.h>
#include <lp_list.h>

void simple_test()
{
	llist* list = llist_init();

	for (int i = 0; i < 20; ++i)
		llist_push_back(list, (void *)&i, sizeof(int));

	printf("Linked list length: %lu\n", list->length);

	puts("Linked list data:\n");
	llist_node* n = list->first;
	while (n)
	{
		int val = *((int *)n->data);
		printf("Value: %d\n", val);
		n = n->next;
	}
	putchar('\n');

	// Pop front test
	for (int i = 0; i < 3; ++i)
		llist_pop_front(list);

	// Pop back test
	for (int i = 0; i < 6; ++i)
		llist_pop_back(list);

	int pval = 423;
	llist_push_after(list, 2, (void *)&pval, sizeof(int));
	
	pval = 534;
	llist_push_back(list, (void *)&pval, sizeof(int));

	n = list->first;
	while (n)
	{
		int val = *((int *)n->data);
		printf("Value: %d\n", val);
		n = n->next;
	}
	putchar('\n');

	llist_release(list);
}

void stress_test()
{
	llist* list = llist_init();

	int pval = 7834;
	llist_push_back(list, &pval, sizeof(int));

	pval = 234;
	llist_push_back(list, &pval, sizeof(int));
	
	pval = 24;
	llist_push_back(list, &pval, sizeof(int));

	llist_pop_back(list);
	llist_pop_front(list);

	pval = 900;
	llist_replace_at(list, &pval, sizeof(int), 0);

	for (int i = 0; i < 30; ++i)
	{
		if (i % 2)
			llist_push_back(list, &i, sizeof(int));
		else
			llist_push_front(list, &i, sizeof(int));
	}


	llist_node* n = list->first;
	while (n)
	{
		int val = *((int *)n->data);
		printf("Value: %d\n", val);
		n = n->next;
	}
	putchar('\n');

	llist_release(list);
}


int main()
{
	srand(time(NULL));
	puts("Linked list tests\n");

	simple_test();
	stress_test();

	return 0;
};