#include <stdio.h>
#include <stdlib.h>
// ��ũ�� ������ �̿��� ť �ִ������ 4�� ����
#define MAX_QUEUE_SIZE 4

typedef char element; // �ڷ��� �̸� ������
typedef struct { // ����ü ����
	element queue[MAX_QUEUE_SIZE]; // ť�� ���� �迭
	int front, rear; // ť ó���� ������ �ε����� ǥ���ϱ� ���� front, rear 
}QueueType;


QueueType *createQueue(); // ť �����ϴ� �Լ� ����
int freeQueue(QueueType *cQ); // ť �����ϴ� �Լ� ����
int isEmpty(QueueType *cQ); // ť�� ������� Ȯ���ϴ� �Լ� ����
int isFull(QueueType *cQ); // ť�� ��� á���� Ȯ���ϴ� �Լ� ����
void enQueue(QueueType *cQ, element item); // ť�� �����Ͱ� �Է��ϴ� �Լ� ����
void deQueue(QueueType *cQ, element* item); // ť�� �����Ͱ��� �����ϴ� �Լ� ����
void printQ(QueueType *cQ); // ť�� �����Ͱ��� ����ϴ� �Լ� ����
void debugQ(QueueType *cQ); // ť �迭�� ��� ��Ҹ� ����ϴ� �Լ� ����
element getElement(); // �����Ͱ����� �Է��� ��Ҹ� �޴� �Լ� ����


int main(void)
{
	QueueType *cQ = createQueue(); // ť ����
	element data;

	char command;

	printf("[----- [������] [2021041068] -----]\n\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");
		// ���Ű ����
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) { // ������ ���Ű�� ���� ���� ����

		case 'i': case 'I': // i, I ���Ű ���� ��, �����Ͱ��� �Է¹޾� ť�� ����
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D': // d, D ���Ű ���� ��, ť�� ����� �� ���� 
			deQueue(cQ,&data);
			break;
		case 'p': case 'P': // p, P ���Ű ���� ��, ť�� �����Ͱ� ���
			printQ(cQ);
			break;
		case 'b': case 'B': // b, B ���Ű ���� ��, ť �迭�� ��� ��Ҹ� ���
			debugQ(cQ);
			break;
		case 'q': case 'Q': // q, Q ���Ű ���� ��, ���α׷� ����
			break;
		default: // �� �̿��� ���Ű ���� ��, ���� �޽��� ���
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q, Q ���Ű ���� �ÿ��� ���α׷� �����ϱ� ����.


	return 1; // ���� ���� Ȯ�� ��ȯ
}

QueueType *createQueue() // ť ���� �Լ� ����
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // �����Ҵ����� QueueType ����ü ����
	cQ->front = 0; // ť�� ó�� �ε����� 0���� �ʱ�ȭ  
	cQ->rear = 0;  // ť�� ������ �ε����� 0���� �ʱ�ȭ
	return cQ; // QueueType ����ü �ּҰ� ��ȯ
}

int freeQueue(QueueType *cQ) // ť ���� �Լ� ����
{
    if(cQ == NULL) // ť�� NULL�̶�� �Լ� ����
	{
		return 1;
	}
    free(cQ); // �׷��� �ʴٸ�, ť�� free
    return 1;
}

element getElement() // ť�� �Է��� �����Ͱ��� �Է¹޴� �Լ� ����
{
	element item;
	printf("Input element = "); 
	scanf(" %c", &item); // �����Ͱ��� �Է�
	return item; // �����Ͱ� ��ȯ
}


int isEmpty(QueueType *cQ) // ť�� ������� Ȯ���ϴ� �Լ� ����
{
	if (cQ->front == cQ->rear){ // front �ε����� rear �ε����� ���� ���
		printf("Circular Queue is empty!"); // ť�� �� ������ �Ǵ��� �޽��� ���
		return 1; // True ������ 1 ��ȯ
	}
	else return 0; //�׷��� �ʴٸ�, False ������ 0 ��ȯ
}

int isFull(QueueType *cQ) // ť�� �� á���� Ȯ���ϴ� �Լ� ����
{
	/* ���� ť�� ������ ���̱� ������, ������ �ε����� �� �ܰ� ���� �ε����� �ε��� 0�� �ǰ�, 
	front �ε������� ���� ����־� �Ѵ�. */
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { // rear �ε����� front �ε����� �� �ܰ� �� �ε������ 
		printf(" Circular Queue is full!"); // ť�� �� �� ������ �Ǵ��� �޽��� ���
		return 1; // True ������ 1 ��ȯ
	} 
	else return 0; // �׷��� �ʴٸ�, False ������ 0 ��ȯ
}

void enQueue(QueueType *cQ, element item) // ť�� �����Ͱ��� �Է��ϴ� �Լ� ����
{
	/* ���� ť�� ������ ���̱� ������, ������ �ε����� �� �ܰ� ���� �ε����� �ε��� 0�� �ǰ�, 
	front �ε������� ���� ����־� �Ѵ�. */
	if(isFull(cQ)) return; // ť ���� �� á�ٸ� �Լ� ����
	else { // �׷��� �ʴٸ�, rear �ε����� �ϳ� �ڷ� �ű�� �� �ε����� �����Ͱ��� �Ҵ�
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		cQ->queue[cQ->rear] = item;
	}
}

void deQueue(QueueType *cQ, element *item) // ť�� �����Ͱ��� �����ϴ� �Լ� ����
{
	/* ���� ť�� ������ ���̱� ������, ������ �ε����� �� �ܰ� ���� �ε����� �ε��� 0�� �ǰ�, 
	front �ε������� ���� ����־� �Ѵ�. */
	if(isEmpty(cQ)) return; // ť�� ����ٸ� �Լ� ����
	else { // ť�� ������� �ʴٸ�, front �ε����� �ϳ� �ڷ� �ű�� �� �ε����� �����Ͱ��� ����
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;
		*item = cQ->queue[cQ->front];
		return;
	}
}


void printQ(QueueType *cQ) // ť�� �ִ� �����Ͱ����� ���
{
	int i, first, last;
	
	// front �ε����� ���� �ε����� ť�� �����Ͱ��� �����ϴ� �ε��� �� ù��°�� ����
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;
	// rear �ε����� ���� �ε����� ���� �ǹ��ϴ� last �ε����� ����
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; 

	printf("Circular Queue : [");

	i = first;
	while(i != last){ // first �ε������� last-1 �ε������� �����Ͱ����� ���
		printf("%3c", cQ->queue[i]); // ť�� ������ ������ ���
		i = (i+1)%MAX_QUEUE_SIZE; // �ε��� �̵�

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) // ť �迭�� ��� ��Ҹ� ����ϴ� �Լ� ����
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // ť �迭�� ��� ��Ҹ� ���
	{
		if(i == cQ->front) { // front �ε����� ��쿡�� front��� ���
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // front �ε����� �ƴ� ��, ť �迭�� ��Ұ��� ���

	}
	printQ(cQ); // ť�� �����Ͱ����� ����� ť �迭�� ��� ��� ��� ����� ��, �м��ϱ� ����.
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front �ε����� rear �ε����� ��� 
}


