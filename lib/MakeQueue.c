#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>


//----- Varaiables for QUEUE ------
#define MAX_QUEUE_SIZE 30
#define FIRST_VALUE 4


//int queue[MAX_QUEUE_SIZE];
int front = 0, rear = 0;
int value;
int prev_value;
int cons_count;
int prev_inc;
int direction;

int enqueue(int *queue, int v)
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

char dequeue(int *queue)
{
	//check if the queue is empty
	/*
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
	*/
	if (front == rear)
	{
		return -1;//deque failed
	}
	else
	{
		int _value = queue[front];
		for(int i = 0; i<MAX_QUEUE_SIZE-1; i++)
		{
			queue[i] = queue[i+1];
		}
		rear -= 1;
		//front = (front + 1) % MAX_QUEUE_SIZE;
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
	if(_wrongNum != 1)
	{
		if (IncOrDec == 1)
			cons_count++;

		else if (IncOrDec == -1)
			cons_count = 0;
	}
	return IncOrDec;
}

int insertRightValue(int *queue)
{
	int increase = 0;
	int wrongNum = 0;

	while(1)
	{
		increase = RandInc(wrongNum);
		value = value + (increase * direction);//from 0 to 7

		if (enqueue(queue, value) == -1)
		{
			value = prev_value;
			return 1;//fullflags on
		}
		prev_value = value;
		wrongNum = 0;
		break;

		if (value >= 0 && value <= 7)
		{
			if (enqueue(queue, value) == -1)
			{
				value = prev_value;
				return 1;//fullflags on
			}
			prev_value = value;
			wrongNum = 0;
			break;
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
	return 0;
}
void Init_blocks(int *queue)
{
	int front = 0, rear = 0;
	int fullflags = 0;
	direction = -1;

	for(int i = 0; i<6; i++)
	{
		enqueue(queue,-999);//insert first value
	}
	value = FIRST_VALUE;//first value(3)
	enqueue(queue,value);//insert first value
	cons_count = 1;//count how many stairs to same direction

	while (fullflags != 1)
	{
		fullflags = insertRightValue(queue);
	}
}
/*
int main(void)
{
	char enter;
	int fullflags = 0;
	int dequeue_value = 0;
	direction = -1;

	//Init
	value = FIRST_VALUE;//first value(3)
	enqueue(value);//insert first value
	cons_count = 1;//count how many stairs to same direction
	//

	while (fullflags != 1)
	{
		fullflags = insertRightValue();
	}

	for(int i=0; i<MAX_QUEUE_SIZE-1; i++)
	{
		printf("%d--> ", queue[i]);
	}


	printf("\n-----------------------------------------------------\n");

	while(1)
	{
		scanf("%c", &enter);

		dequeue_value = dequeue();
		//printf("value : %d | next front : %d\n", dequeue_value, front % MAX_QUEUE_SIZE);

		insertRightValue();

		for (int i = 0; i < MAX_QUEUE_SIZE; i++)
		{
			printf("%d ", queue[i]);
		}
		printf("\n");
	}

	return 0;
}
*/
