#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LP_MATH_IMPLEMENTATION
#define LP_LIST_IMPLEMENTATION

#include <lp_common.h>
#include <lp_math.h>
#include <lp_list.h>


// Returns difference between start and end in ms
double get_elapsed_time(clock_t start, clock_t end)
{
	return ((double)(end - start) / (double)CLOCKS_PER_SEC) * 1000.0;
}

void llist_int_print(llist* list)
{
	putchar('\n');

	llist_node* iter = list->first;
	int val = 0;
	size_t index = 0;
	while (iter)
	{
		val = *(int*)iter->data;
		printf("Index: %lu Value: %d\n", index, val);
		++index;
		iter = iter->next;
	}
	putchar('\n');
};

void llist_vec3_print(llist* list)
{
	putchar('\n');

	llist_node* iter = list->first;
	vec3_t val;
	size_t index = 0;
	while (iter)
	{
		val = *(vec3_t*)iter->data;
		printf("Index: %lu \n\tX: %.2f Y: %.2f Z: %.2f\n", index, val.x, val.y, val.z);
		++index;
		iter = iter->next;
	}
	putchar('\n');
}

void simple_test()
{
	llist* list = llist_init();

	for (int i = 0; i < 20; ++i)
		llist_push_back(list, (void *)&i, sizeof(int));

	printf("Linked list length: %lu\n", list->length);

	llist_int_print(list);

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

	llist_int_print(list);

	llist_release(list);
};

typedef struct {
	char name[20];
	unsigned index;
	float val1;
	float val2;
	float val3;
} some_struct_t;

void stress_test()
{
	clock_t start, end;
	llist* list = llist_init();

	start = clock();
	for (int i = 0; i < 100000; ++i)
	{
		some_struct_t st = {
			.name = "Hello world",
			.index = 33,
			.val1 = 4.23333f,
			.val2 = 5.27773f,
			.val3 = 1.23443f
		};
		llist_push_back(list, &st, sizeof(some_struct_t));
	}
	end = clock();
	printf(GREEN "Creation of 10_000 nodes: " BOLDRED "%.4f" RESET "\n", get_elapsed_time(start, end));

	start = clock();
	for (int i = 0; i < 1000; ++i)
		llist_pop_back(list);

	for (int i = 0; i < 1000; ++i)
		llist_pop_front(list);
	end = clock();
	printf(GREEN "Deletion of 200 nodes: " BOLDRED "%.4f" RESET "\n", get_elapsed_time(start, end));

	start = clock();
	for (size_t i = 0; i < 3000; ++i)
	{
		some_struct_t st = {
			.name = "Hello world",
			.index = 33,
			.val1 = 4.23333f,
			.val2 = 5.27773f,
			.val3 = 1.23443f
		};
		llist_push_after(list, 400LU, &st, sizeof(some_struct_t));
	}
	end = clock();
	printf(GREEN "Pushing at 400 index of 3000 nodes: " BOLDRED "%.4f" RESET "\n", get_elapsed_time(start, end));

	start = clock();
	llist_clear(list);
	end = clock();
	printf(GREEN "Array clearing: " BOLDRED "%.4f" RESET "\n", get_elapsed_time(start, end));

	llist_release(list);
}

void each_other_test(size_t len)
{
	llist* list = llist_init();

	int val = -(len / 2);
	for (int i = 0; i < len; ++i)
	{
		llist_push_back(list, &val, sizeof(int));
		val++;
	};

	int l_val;
	llist_at(list, &l_val, sizeof(int), list->length - 1);

	if (l_val != (len / 2) - 1)
		puts(BOLDRED "Test not passed\n" RESET);
	else
		puts(BOLDGREEN "Test passed\n" RESET);

	llist_release(list);
};

void test_with_vec3(size_t len)
{
	llist* list = llist_init();

	vec3_t pval;
	for (size_t i = 0; i < len; ++i)
	{
		pval = (vec3_t){(float)i * 2.f,
						(float)i * 2.f,
						(float)i * 2.f};
		
		llist_push_back(list, &pval, sizeof(vec3_t));
	};

	llist_vec3_print(list);

	llist_pop_back(list);
	llist_pop_back(list);
	llist_pop_back(list);

	llist_pop_front(list);
	llist_pop_front(list);
	llist_pop_front(list);
	llist_pop_front(list);

	for (size_t i = 20; i < 23; ++i)
	{
		pval = (vec3_t){(float)i * 2.f,
						(float)i * 2.f,
						(float)i * 2.f};
		
		llist_push_back(list, &pval, sizeof(vec3_t));
	};


	for (size_t i = 10; i < 13; ++i)
	{
		pval = (vec3_t){(float)i * 2.f,
						(float)i * 2.f,
						(float)i * 2.f};
		
		llist_push_front(list, &pval, sizeof(vec3_t));
	};

	for (size_t i = 10; i < 13; ++i)
	{
		pval = (vec3_t){(float)i * 2.f,
						(float)i * 2.f,
						(float)i * 2.f};
		
		llist_push_after(list, 9LU, &pval, sizeof(vec3_t));
	};

	llist_vec3_print(list);

	llist_clear(list);

	int pval2;
	for (size_t i = 0; i < len; ++i)
	{
		pval2 = i * 2;
		llist_push_front(list, &pval2, sizeof(int));
	}

	llist_int_print(list);

	llist_release(list);
};



int main()
{	
	srand(time(NULL));
	puts("Linked list tests\n");

	clock_t start, end;

	start = clock();
	each_other_test(40000);
	end = clock();

	printf(BOLDBLUE "Elapsed total: " BOLDRED "%4.2f ms\n" RESET, get_elapsed_time(start, end));

	return 0;
};