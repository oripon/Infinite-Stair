#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define SZ 10

char *queue[SZ];
int front = 0, rear = 0;

int enqueue(char *v)
{
	//check if the queue is full
	if ((rear + 1) % SZ == front)
	{
		return 0;//enque failed
	}
	else
	{
		queue[rear] = v;
		rear = (rear + 1) % SZ;
		return 1;
	}
}

char *dequeue()
{
	//check if the queue is empty
	if (front == rear)
	{
		return 0;//deque failed
	}
	else
	{
		char *v = queue[front];
		front = (front + 1) % SZ;
		return v;
	}
}

int main(void)
{
	char temp[10];
	char *v;

	while (1)
	{
		printf("Enter word : ");
		scanf("%s", temp);

		v = (char *)malloc(strlen(temp) + 1);
		strcpy(v, temp);

		if (enqueue(v) == 0)
		{
			printf("Queue is full\n");

			while ((v = dequeue()) != 0)
			{
				printf("-->%s \n", v);
				free(v);
			}
		}
	}
	return 0;
}
