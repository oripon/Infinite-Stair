#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 100

char queue[MAX_QUEUE_SIZE];
int front = 3, rear = 3;
int cons_count;
int prev_inc;
int direction;

int enqueue(int v)
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

char dequeue()
{
	//check if the queue is empty
	if (front == rear)
	{
		return -1;//deque failed
	}
	else
	{
		int _value = queue[front];
		front = (front + 1) % MAX_QUEUE_SIZE;
		return _value;
	}
}

int RandInc(int _wrongNum)//based on probability
{
	int random;
	int IncOrDec;

	random = rand() % 100;// create 0~99 number

	if (random < cons_count * 10 + 10)//probability <0% to 80%>
	{
		IncOrDec = -1;
		direction = direction * -1;
	}
	else
	{
		IncOrDec = 1;
	}

	/*
	if ((cons_count == 0) || (cons_count == 1))
	{
		if (random >= 0 && random < 5)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	else if (cons_count == 2)
	{
		if (random >= 0 && random < 10)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	else if (cons_count == 3)
	{
		if (random >= 0 && random < 15)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	else if (cons_count == 4)
	{
		if (random >= 0 && random < 20)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	else if (cons_count == 5)
	{
		if (random >= 0 && random < 40)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	else if (cons_count == 6)
	{
		if (random >= 0 && random < 65)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	else if (cons_count == 7)
	{
		if (random >= 0 && random < 80)
			IncOrDec = -1;
		else
			IncOrDec = 1;
	}
	*/

	if(_wrongNum != 1)
	{
		if (IncOrDec == 1)
			cons_count++;

		else if (IncOrDec == -1)
			cons_count = 0;
	}
	return IncOrDec;
}

int main(void)
{
	int increase = 0;
	int value = 3;//<0 ~ 7>, init = 3
	int wrongNum = 0;
	direction = -1;

	enqueue(value);
	cons_count = 1;
	printf("->%d", value);

	while (1)
	{
		increase = RandInc(wrongNum);
		value = value + (increase * direction);//from 0 to 7

		if (value >= 0 && value <= 7)
		{
			if (enqueue(value) == -1)
			{
				break;
			}
			printf("->%d", value);
			wrongNum = 0;
		}
		else if (value < 0)
		{
			value = 0;
			wrongNum = 1;
		}
		else if (value > 7)
		{
			value = 7;
			wrongNum = 1;
		}
	}	
	printf("\n\n");

	while ((value = dequeue()) != -1)
	{
		printf("->%d", value);
	}

	return 0;
}

