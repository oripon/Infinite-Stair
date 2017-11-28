#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 30

char *queue[MAX_QUEUE_SIZE];
int front = 3, rear = 3;

int enqueue(char *v)
{
	//check if the queue is full
	if ((rear + 1) % MAX_QUEUE_SIZE == front)
	{
		return -1;//enque failed
	}
	else
	{
		queue[rear] = v;
		rear = (rear + 1) % MAX_QUEUE_SIZE;
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
		front = (front + 1) % MAX_QUEUE_SIZE;
		return v;
	}
}

int RandInc()
{
	int randvalue;
		
	while (1)
	{
		randvalue = rand() % 3 - 1;// from -1 to +1
		
		if (randvalue != 0)
			break;
	}

	return randvalue;
}

int main(void)
{
	char input;
	int increase = 0;
	int value = 3;//<0 ~ 7>, init = 3
	int cnt = 0;
	//char x;

	while (1)
	{
		scanf("%c", &input);
		//scanf("%c", &x);

		if (input != '\0')
		{
			if (++cnt > 30)
			{
				return 0;
			}
			increase = RandInc();

			value = value + increase;// from 0 to 7

			printf("%d <%d>\n", value, cnt);
			/*
			if (enqueue(value) == -1)
			{
				printf("Queue is full\n");
			}
				
			while ((value = dequeue()) != 0)
			{
				printf("-->%s \n", value);
			}
			*/
		}
	}
	return 0;
}

