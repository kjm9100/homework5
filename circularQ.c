#include <stdio.h>
#include <stdlib.h>
// 매크로 변수를 이용해 큐 최대사이즈 4로 설정
#define MAX_QUEUE_SIZE 4

typedef char element; // 자료형 이름 재정의
typedef struct { // 구조체 정의
	element queue[MAX_QUEUE_SIZE]; // 큐를 위한 배열
	int front, rear; // 큐 처음과 마지막 인덱스를 표현하기 위한 front, rear 
}QueueType;


QueueType *createQueue(); // 큐 생성하는 함수 선언
int freeQueue(QueueType *cQ); // 큐 삭제하는 함수 선언
int isEmpty(QueueType *cQ); // 큐가 비었는지 확인하는 함수 선언
int isFull(QueueType *cQ); // 큐가 모두 찼는지 확인하는 함수 선언
void enQueue(QueueType *cQ, element item); // 큐에 데이터값 입력하는 함수 선언
void deQueue(QueueType *cQ, element* item); // 큐에 데이터값을 삭제하는 함수 선언
void printQ(QueueType *cQ); // 큐의 데이터값을 출력하는 함수 선언
void debugQ(QueueType *cQ); // 큐 배열의 모든 요소를 출력하는 함수 선언
element getElement(); // 데이터값으로 입력할 요소를 받는 함수 선언


int main(void)
{
	QueueType *cQ = createQueue(); // 큐 생성
	element data;

	char command;

	printf("[----- [김지민] [2021041068] -----]\n\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");
		// 명령키 선택
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) { // 선택한 명령키에 따른 동작 수행

		case 'i': case 'I': // i, I 명령키 선택 시, 데이터값을 입력받아 큐에 저장
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D': // d, D 명령키 선택 시, 큐에 저장된 값 삭제 
			deQueue(cQ,&data);
			break;
		case 'p': case 'P': // p, P 명령키 선택 시, 큐의 데이터값 출력
			printQ(cQ);
			break;
		case 'b': case 'B': // b, B 명령키 선택 시, 큐 배열의 모든 요소를 출력
			debugQ(cQ);
			break;
		case 'q': case 'Q': // q, Q 명령키 선택 시, 프로그램 종료
			break;
		default: // 그 이외의 명령키 선택 시, 오류 메시지 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q, Q 명령키 선택 시에만 프로그램 종료하기 위함.


	return 1; // 정상 종료 확인 반환
}

QueueType *createQueue() // 큐 생성 함수 정의
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 동적할당으로 QueueType 구조체 생성
	cQ->front = 0; // 큐의 처음 인덱스를 0으로 초기화  
	cQ->rear = 0;  // 큐의 마지막 인덱스를 0으로 초기화
	return cQ; // QueueType 구조체 주소값 반환
}

int freeQueue(QueueType *cQ) // 큐 삭제 함수 정의
{
    if(cQ == NULL) // 큐가 NULL이라면 함수 종료
	{
		return 1;
	}
    free(cQ); // 그렇지 않다면, 큐를 free
    return 1;
}

element getElement() // 큐에 입력할 데이터값을 입력받는 함수 정의
{
	element item;
	printf("Input element = "); 
	scanf(" %c", &item); // 데이터값을 입력
	return item; // 데이터값 반환
}


int isEmpty(QueueType *cQ) // 큐가 비었는지 확인하는 함수 정의
{
	if (cQ->front == cQ->rear){ // front 인덱스랑 rear 인덱스가 같을 경우
		printf("Circular Queue is empty!"); // 큐가 빈 것으로 판단해 메시지 출력
		return 1; // True 값으로 1 반환
	}
	else return 0; //그렇지 않다면, False 값으로 0 반환
}

int isFull(QueueType *cQ) // 큐가 다 찼는지 확인하는 함수 정의
{
	/* 원형 큐로 구현한 것이기 때문에, 마지막 인덱스의 한 단계 뒤의 인덱스는 인덱스 0이 되고, 
	front 인덱스에는 값을 비워둬야 한다. */
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { // rear 인덱스가 front 인덱스의 한 단계 전 인덱스라면 
		printf(" Circular Queue is full!"); // 큐가 다 찬 것으로 판단해 메시지 출력
		return 1; // True 값으로 1 반환
	} 
	else return 0; // 그렇지 않다면, False 값으로 0 반환
}

void enQueue(QueueType *cQ, element item) // 큐에 데이터값을 입력하는 함수 정의
{
	/* 원형 큐로 구현한 것이기 때문에, 마지막 인덱스의 한 단계 뒤의 인덱스는 인덱스 0이 되고, 
	front 인덱스에는 값을 비워둬야 한다. */
	if(isFull(cQ)) return; // 큐 값이 다 찼다면 함수 종료
	else { // 그렇지 않다면, rear 인덱스를 하나 뒤로 옮기고 그 인덱스에 데이터값을 할당
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		cQ->queue[cQ->rear] = item;
	}
}

void deQueue(QueueType *cQ, element *item) // 큐의 데이터값을 삭제하는 함수 정의
{
	/* 원형 큐로 구현한 것이기 때문에, 마지막 인덱스의 한 단계 뒤의 인덱스는 인덱스 0이 되고, 
	front 인덱스에는 값을 비워둬야 한다. */
	if(isEmpty(cQ)) return; // 큐가 비었다면 함수 종료
	else { // 큐가 비어있지 않다면, front 인덱스를 하나 뒤로 옮기고 그 인덱스의 데이터값을 삭제
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;
		*item = cQ->queue[cQ->front];
		return;
	}
}


void printQ(QueueType *cQ) // 큐에 있는 데이터값들을 출력
{
	int i, first, last;
	
	// front 인덱스의 다음 인덱스를 큐의 데이터값이 존재하는 인덱스 중 첫번째로 지정
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;
	// rear 인덱스의 다음 인덱스를 끝을 의미하는 last 인덱스로 지정
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; 

	printf("Circular Queue : [");

	i = first;
	while(i != last){ // first 인덱스부터 last-1 인덱스까지 데이터값들을 출력
		printf("%3c", cQ->queue[i]); // 큐의 데이터 값들을 출력
		i = (i+1)%MAX_QUEUE_SIZE; // 인덱스 이동

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) // 큐 배열의 모든 요소를 출력하는 함수 정의
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // 큐 배열의 모든 요소를 출력
	{
		if(i == cQ->front) { // front 인덱스의 경우에는 front라고 출력
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // front 인덱스가 아닐 시, 큐 배열의 요소값을 출력

	}
	printQ(cQ); // 큐의 데이터값들을 출력해 큐 배열의 모든 요소 출력 결과를 비교, 분석하기 위함.
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front 인덱스와 rear 인덱스를 출력 
}


