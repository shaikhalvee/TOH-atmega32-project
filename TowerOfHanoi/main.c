/*
 * GccApplication1.c
 *
 * Created: 4/28/2016 12:35:12 PM
 * Author : Shaikh
 */
#define F_CPU 1000000
#define INT_MIN -1
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

struct Stack
{
	unsigned capacity;
	int top;
	int *array;
};

// function to create a stack of given capacity.
struct Stack *createStack(unsigned capacity)
{
	struct Stack *stack =
		(struct Stack *)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array =
		(int *)malloc(stack->capacity * sizeof(int));
	return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack)
{
	return (stack->top == stack->capacity - 1);
}

// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack)
{
	return (stack->top == -1);
}

// Function to add an item to stack.  It increases
// top by 1
void push(struct Stack *stack, int item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
}

// Function to remove an item from stack.  It
// decreases top by 1
int pop(struct Stack *stack)
{
	if (isEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top--];
}

unsigned char over[14] = {8, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1, 8};
unsigned char p1[14] = {8, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1, 8};
unsigned char p2[14] = {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8};
unsigned char p3[14] = {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8};
unsigned char g[14] = {0, 0, 0, 0, 30, 33, 33, 38, 36, 23, 0, 0, 0, 0};
unsigned char s[14] = {0, 0, 0, 0, 0, 18, 41, 37, 18, 0, 0, 0, 0, 0};
int top1 = 6, top2 = 7, cur, ret, dsk, fro, to, strt, sim;

void init()
{
	for (int i = 1; i < 13; i++)
	{
		p1[i] = over[i];
		p2[i] = 0;
		p3[i] = 0;
	}
}

void showPeg1()
{
	unsigned char cnt = 0, k = 0;
	for (int i = 0; i < 14; i++)
	{
		if (cur == 1 && (i == top1 || i == top2))
			PORTA = (1 << 7) | ((1 << p1[i]) - 1);
		else
			PORTA = (1 << p1[i]) - 1;
		PORTC = cnt + k;
		k++;
	}
}

void showPeg2()
{
	unsigned char cnt = 0, k = 13;
	int j = 0;
	for (int i = 13; i < 16; i++)
	{
		PORTA = (1 << p2[j++]) - 1;
		PORTC = cnt + k;
		k++;
	}
	cnt = 1 << 4, k = 0;
	for (int i = 0; i < 11; i++)
	{
		if (cur == 2 && (i == 3 || i == 4))
			PORTA = (1 << 7) | ((1 << p2[j++]) - 1);
		else
			PORTA = (1 << p2[j++]) - 1;
		PORTC = cnt + k;
		k++;
	}
}

void showPeg3()
{
	unsigned char cnt = 1 << 4, k = 10;
	int j = 0;
	for (int i = 10; i < 16; i++)
	{
		PORTA = (1 << p3[j++]) - 1;
		PORTC = cnt + k;
		k++;
	}
	cnt = 1 << 5, k = 0;
	for (int i = 0; i < 8; i++)
	{
		if (cur == 3 && (i == 0 || i == 1))
			PORTA = (1 << 7) | ((1 << p3[j++]) - 1);
		else
			PORTA = (1 << p3[j++]) - 1;
		PORTC = cnt + k;
		k++;
	}
}

void showG()
{
	unsigned char cnt = 0, k = 0;
	for (int i = 0; i < 14; i++)
	{
		if (cur == 1 && (i == 6 || i == 7))
		{
			PORTA = (1 << 7) | g[i];
		}
		else
			PORTA = g[i];
		PORTC = cnt + k;
		k++;
	}
}

void showS()
{
	unsigned char cnt = 1 << 4, k = 10;
	int j = 0;
	for (int i = 10; i < 16; i++)
	{
		PORTA = s[j++];
		PORTC = cnt + k;
		k++;
	}
	cnt = 1 << 5, k = 0;
	for (int i = 0; i < 8; i++)
	{
		if (cur == 3 && (i == 0 || i == 1))
			PORTA = (1 << 7) | s[j++];
		else
			PORTA = s[j++];
		PORTC = cnt + k;
		k++;
	}
}

void myDelay()
{
	for (long long j = 0; j < F_CPU / 1000; j++)
	{
		showPeg1();
		showPeg2();
		showPeg3();
	}
}

void myDelay1()
{
	for (long long j = 0; j < F_CPU / 5000; j++)
	{
		showPeg1();
		showPeg2();
		showPeg3();
	}
}

void myDelay2()
{
	for (long long j = 0; j < F_CPU / 5000; j++)
	{
		showG();
		showS();
	}
}

// Function to show the movement of disks
void moveDisk(int fromPeg, int toPeg, int disk)
{
	if (fromPeg == 1)
	{
		for (int c = 0; c < disk; c++)
		{
			p1[top1 - c]--;
			p1[top2 + c]--;
		}
	}
	else if (fromPeg == 2)
	{
		for (int c = 0; c < disk; c++)
		{
			p2[top1 - c]--;
			p2[top2 + c]--;
		}
	}
	else if (fromPeg == 3)
	{
		for (int c = 0; c < disk; c++)
		{
			p3[top1 - c]--;
			p3[top2 + c]--;
		}
	}
	if (toPeg == 1)
	{
		for (int c = 0; c < disk; c++)
		{
			p1[top1 - c]++;
			p1[top2 + c]++;
		}
	}
	else if (toPeg == 2)
	{
		for (int c = 0; c < disk; c++)
		{
			p2[top1 - c]++;
			p2[top2 + c]++;
		}
	}
	else if (toPeg == 3)
	{
		for (int c = 0; c < disk; c++)
		{
			p3[top1 - c]++;
			p3[top2 + c]++;
		}
	}
}

int dk(unsigned char *p)
{
	unsigned char m = p[1];
	int cx = 0;
	for (int i = 2; i < 13; i++)
	{
		if (p[i] > m)
			m = p[i];
	}
	for (int i = 1; i < 13; i++)
	{
		if (p[i] && (p[i] == m))
			cx++;
	}
	return cx / 2;
}

ISR(INT0_vect)
{
	if (strt)
	{
		myDelay2();
		if (cur == 1)
		{
			sim = -1;
		}
		else if (cur == 3)
		{
			sim = 1;
		}
		strt = 0;
	}
	else
	{
		myDelay1();
		ret++;
		if (ret == 2)
		{
			ret = 0;
			to = cur;
			int flag = 0, cnt = 0;
			if (to == 1)
			{
				cnt = dk(p1);
				if (!cnt || dsk < cnt)
					flag = 1;
			}
			else if (to == 2)
			{
				cnt = dk(p2);
				if (!cnt || dsk < cnt)
					flag = 1;
			}
			else if (to == 3)
			{
				cnt = dk(p3);
				if (!cnt || dsk < cnt)
					flag = 1;
			}
			if (flag)
			{
				moveDisk(fro, to, dsk);
				myDelay();
			}
		}
		else if (ret == 1)
		{
			fro = cur;
			if (cur == 1)
			{
				dsk = dk(p1);
			}
			else if (cur == 2)
			{
				dsk = dk(p2);
			}
			else if (cur == 3)
			{
				dsk = dk(p3);
			}
		}
		else
			ret = 0;
	}
}

ISR(INT1_vect)
{
	if (cur > 1)
	{
		if (strt)
		{
			myDelay2();
			cur -= 2;
		}
		else
		{
			myDelay1();
			cur--;
		}
	}
}

ISR(INT2_vect)
{
	if (cur < 3)
	{
		if (strt)
		{
			myDelay2();
			cur += 2;
		}
		else
		{
			myDelay1();
			cur++;
		}
	}
}

// Function to implement legal movement between
// two poles
void moveDisksBetweenTwoPoles(struct Stack *src,
							  struct Stack *dest, int s, int d)
{
	int pole1TopDisk = pop(src);
	int pole2TopDisk = pop(dest);

	// When pole 1 is empty
	if (pole1TopDisk == INT_MIN)
	{
		push(src, pole2TopDisk);
		moveDisk(d, s, pole2TopDisk);
	}

	// When pole2 pole is empty
	else if (pole2TopDisk == INT_MIN)
	{
		push(dest, pole1TopDisk);
		moveDisk(s, d, pole1TopDisk);
	}

	// When top disk of pole1 > top disk of pole2
	else if (pole1TopDisk > pole2TopDisk)
	{
		push(src, pole1TopDisk);
		push(src, pole2TopDisk);
		moveDisk(d, s, pole2TopDisk);
	}

	// When top disk of pole1 < top disk of pole2
	else
	{
		push(dest, pole2TopDisk);
		push(dest, pole1TopDisk);
		moveDisk(s, d, pole1TopDisk);
	}
}

// Function to implement TOH puzzle
void tohIterative(int num_of_disks, struct Stack *src, struct Stack *aux,
				  struct Stack *dest)
{
	int i, total_num_of_moves;
	int s = 1, d = 3, a = 2;

	// If number of disks is even, then interchange
	// destination pole and auxiliary pole
	if (num_of_disks % 2 == 0)
	{
		int temp = d;
		d = a;
		a = temp;
	}
	total_num_of_moves = pow(2, num_of_disks) - 1;

	// Larger disks will be pushed first
	for (i = num_of_disks; i >= 1; i--)
		push(src, i);

	for (i = 1; i <= total_num_of_moves; i++)
	{
		myDelay();
		if (i % 3 == 1)
			moveDisksBetweenTwoPoles(src, dest, s, d);

		else if (i % 3 == 2)
			moveDisksBetweenTwoPoles(src, aux, s, a);

		else if (i % 3 == 0)
			moveDisksBetweenTwoPoles(aux, dest, a, d);
	}
	myDelay();
	p2[top1]--, p2[top2]--;
	p3[top1]++, p3[top2]++;
	myDelay();
}

int done()
{
	for (int i = 0; i < 14; i++)
	{
		if (over[i] != p3[i])
			return 0;
	}
	return 1;
}

int main(void)
{
	/* Replace with your application code */

	DDRA = 0xFF;
	DDRB = ~4;
	DDRD = ~12;
	MCUCSR = (1 << JTD);
	MCUCSR = (1 << JTD);
	DDRC = 0xFF;

	unsigned int num_of_disks = 6;

	struct Stack *src, *dest, *aux;

	src = createStack(num_of_disks);
	aux = createStack(num_of_disks);
	dest = createStack(num_of_disks);
	GICR |= (1 << INT0);
	GICR |= (1 << INT1);
	GICR |= (1 << INT2);
	MCUCSR |= (1 << ISC2);
	MCUCR = 15;
	cur = 1;
	sei();
	while (1)
	{
		strt = 1;
		showG();
		showS();
		if (sim == 1)
		{
			cur = 0;
			src = createStack(num_of_disks);
			aux = createStack(num_of_disks);
			dest = createStack(num_of_disks);
			init();
			tohIterative(num_of_disks, src, aux, dest);
			myDelay();
			cur = 1;
		}
		else if (sim == -1)
		{
			init();
			cur = 1;
			while (1)
			{
				showPeg1();
				showPeg2();
				showPeg3();
				if (done())
				{
					myDelay();
					break;
				}
			}
			cur = 1;
		}
		sim = 0;
	}
	return 0;
}
