#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int N = 6;

typedef struct node
{
	int**arr;
	struct node*child, *rsib;
	int level;
} node;

typedef struct position
{	int a; int b;
} position;

typedef struct queue
{
	node*element;
	struct queue*next;
} queue;

queue*top = NULL;

node*createnode()
{	int i, j;
	node*new = (node*)malloc(sizeof(node));
	int**array = (int**)malloc(N * sizeof(int*));
	for (i = 0; i < N; i++)
	{ array[i] = (int*)malloc(N * sizeof(int));}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			array[i][j] = 0;
		}
	}
	new->arr = array;
	new->level = -1;
	return new;
}

node*createsib(node*brother)
{	int i, j;   int** parr = brother->arr;
	node*new = (node*)malloc(sizeof(node));
	int**array = (int**)malloc(N * sizeof(int*));
	for (i = 0; i < N; i++)
	{ array[i] = (int*)malloc(N * sizeof(int));}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{	if (i == brother->level) {array[i][j] = 0;} else {
				array[i][j] = parr[i][j];
			}
		}
	}
	new->level = brother->level;
	new->arr = array;
	return new;
}

node*createchild(node*parent)
{	int i, j;
	int** parr = parent->arr;
	node*new = (node*)malloc(sizeof(node));
	int**array = (int**)malloc(N * sizeof(int*));
	for (i = 0; i < N; i++)
	{ array[i] = (int*)malloc(N * sizeof(int));}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			array[i][j] = parr[i][j];
		}
	}
	new->arr = array;
	new->level = ((parent->level) + 1);
	return new;
}

position* isvalidchild(node* ref)
{	int i, j, k, l, switc;
	position*new = (position*)malloc(sizeof(position));
	int**rarr = ref->arr;
	i = ((ref->level) + 1);
	j = 0;
	for (k = 0; k < i; k++)
	{	for (l = 0; l < N; l++)
			if (rarr[k][l] == 1)
			{	if (l == j)
				{j++; k = 0; l = 0;}
				else if ((k + l) == (i + j))
				{j++; k = 0; l = 0;}
				else if ((k - l) == (i - j))
				{j++; k = 0; l = 0;}
			}
	}
	if (j < N) {new->a = i; new->b = j;}
	else {new->a = -1; new->b = -1;}
	return new;
}

position* isvalidsib(node*ref)
{	int i, j, k, l, first = -1;
	position*new = (position*)malloc(sizeof(position));
	int**rarr = ref->arr;
	i = (ref->level);
	for (j = 0; j < N; j++)
	{if (rarr[i][j] == 1) {first = j; break;}}
	j = first + 1;
	for (k = 0; k < i; k++)
	{	for (l = 0; l < N; l++)
			if (rarr[k][l] == 1)
			{	if (l == j)
				{j++; k = 0; l = 0;}
				else if ((k + l) == (i + j))
				{j++; k = 0; l = 0;}
				else if ((k - l) == (i - j))
				{j++; k = 0; l = 0;}
			}
	}
	if (j < N) {new->a = i; new->b = j;}
	else {new->a = -1; new->b = -1;}
	return new;
}

void enqueue(node*root)
{	queue*temp = top;
	queue*new =	(queue*)malloc(sizeof(queue));
	new->element = root;
	new->next = NULL;
	if (top == NULL)
	{	top = new;
	}
	else
	{	while (temp->next != NULL)
		{	temp = temp->next;
		}
		temp->next = new;
	}
	if (root->rsib != NULL)
	{	enqueue(root->rsib);
	}
}
void solver(queue*head);
void dequeue()
{	queue*temp = top; int i, j;
	if (top == NULL) {return;}
	if (top->element->level == N - 1)
	{
		for (i = 0; i < N; i++)
		{	for (j = 0; j < N; j++)
			{printf("%d ", top->element->arr[i][j]);}
			printf("\n");
		} printf("\n");
		top = top->next;        free(temp);
		solver(top);
	}
	else
	{	top = top->next; free (temp);
		solver(top);
	}
}
void solver(queue*head)
{	position*test, *stest;   int i, j;
	node*new; node*temp;
	if (head == NULL)
	{	return;
	}
	else {
		if (head->element->child == NULL)
		{
			test = isvalidchild(head->element);
			if (test->a == -1)
			{	dequeue();
			}
			else
			{	new = createchild(head->element);
				i = test->a; j = test->b;
				new->arr[i][j] = 1;
				head->element->child = new;
				stest = isvalidsib(new);
				temp = new;
				while (stest->a != -1)
				{	temp->rsib = createsib(temp);
					temp = temp->rsib;
					temp->arr[stest->a][stest->b] = 1;
					stest = isvalidsib(temp);
				}
				enqueue(head->element->child);
				dequeue();
			}
		}
	}
}
int main()
{	int n = 0; node*root = createnode();
	if (root == NULL) {printf("error"); return 1;}
	enqueue(root);
	solver(top);
	return 0;
}
