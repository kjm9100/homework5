#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20


typedef enum{ // 열거형으로 우선 순위 설정
	lparen = 0,  // ( 괄호
	rparen = 4,  // ) 괄호
	times_divide = 3,  // * 곱셈or / 나눗셈
	plus_minus = 2,    // + 덧셈 or - 뺄셈
	operand = 1 // 피연산자
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];
char postfixExp[MAX_EXPRESSION_SIZE];
char postfixStack[MAX_STACK_SIZE];
int evalStack[MAX_STACK_SIZE];

int postfixStackTop = -1;
int evalStackTop = -1;

int evalResult = 0;

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	printf("[----- [김지민] [2021041068] -----]\n\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		// 명령키 선택
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) { // 명령키별 각각 다른 동작 수행
		case 'i': case 'I': // i, I 명령키 선택 시, 중위표기식 수식 입력
			getInfix();
			break;
		case 'p': case 'P': // p, P 명령키 선택 시, 입력한 수식을 후위표기식 수식으로 변환
			toPostfix();
			break;
		case 'e': case 'E': // e, E 명령키 선택 시, 변환된 후위표기식 수식을 계산
			evaluation();
			break;
		case 'd': case 'D': // d, D 명령키 선택 시, 결과값 확인 및 postfixStack 분석
			debug();
			break;
		case 'r': case 'R': // r, R 명령키 선택 시, 배열 및 Top 인덱스 초기화
			reset();
			break;
		case 'q': case 'Q': // q, Q 명령키 선택 시, 프로그램 종료
			break;
		default: // 그 이외의 명령키 선택 시, 오류 메시지 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q, Q 명령키 선택 시에만 프로그램 종료

	return 1; // 정상 종료 확인 반환
}

void postfixPush(char x) // postfixStack에 순서 기억을 위해 연산자를 넣는 함수 정의
{ // postfixStack의 Top에 연산자 저장(Top 인덱스가 초기에 -1로 초기화됐기에 증감 연산 후 저장)
    postfixStack[++postfixStackTop] = x; 
}

char postfixPop() // postfixStack의 Top에 있는 연산자를 꺼내는 함수 정의
{
	char x;
    if(postfixStackTop == -1) // postfixStack에 아무것도 없을 경우
        return '\0'; // NUL 문자를 반환
    else { 
	// postfixStack에 연산자가 있다면, postfixStack의 Top에 있는 연산자 넘겨준 후 Top 인덱스값을 1 감소
    	x = postfixStack[postfixStackTop--];
    }
    return x; // postfixStack의 Top에 있던 연산자 반환
}

void evalPush(int x) // evalStack의 피연산자 및 중간 계산 결과를 저장하는 함수 정의
{ // evalStack의 Top에 저장(Top 인덱스가 초기에 -1로 초기화됐기에 증감 연산 후 저장)
    evalStack[++evalStackTop] = x;
}

int evalPop() // evalStack의 값들을 꺼내 넘겨주는 함수 정의
{
    if(evalStackTop == -1) // evalStack에 아무것도 없을 경우 -1 반환
        return -1; 
    else // evalStack에 값이 존재한다면, evalStack의 Top에 있는 값을 꺼내 반환
        return evalStack[evalStackTop--]; 
}

void getInfix() // 중위표기식 수식을 입력받는 함수 정의
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); // 중위표기식 수식을 입력받아 infixExp 배열에 저장

	for(int i =0; i <strlen(infixExp); i++) // 입력받은 문자열을 검사해 (피)연산자의 범위를 제약(아스키코드표 이용)
	{	// 0~9,'+','-','*','/','(',')' 이외의 문자를 입력했을 경우 메시지 출력 및 reset
		if(!((infixExp[i]>='(')&&(infixExp[i]<='9')&&(infixExp[i]!=',')&&(infixExp[i]!='.'))){ 
		printf("Check out the operand and operation\n");
		reset();
		printf("Retype the expression.\n\n");
		return;
		}
	}
}

precedence getToken(char symbol) // (피)연산자를 받아 그것의 우선순위를 결정하는 함수 정의
{
	/* 					<우선순위>
	<- 낮음										->높음
	( 괄호, 피연산자, <빼기, 더하기>, <나누기, 곱하기>, ) 괄호

	우선 순위 결정 시에 mnemonic 사용해 각 기호별 우선 순위를 반환 
	*/
	switch(symbol) {
	case '(' : return lparen; 
	case ')' : return rparen;
	case '+' : case '-' : return plus_minus;
	case '/' : case '*' : return times_divide;
	default : return operand;
	}
}

precedence getPriority(char x) // 우선순위 및 (피)연산자 정보를 알아내는 함수 정의
{
	return getToken(x);
}

void charCat(char* c) // 문자를 받아 postfixExp에 저장
{
	if (postfixExp == '\0') // postfixExp가 비었다면,
		strncpy(postfixExp, c, 1); // 문자 하나를 복사해 postfixExp에 저장
	else // 문자들이 이미 있다면, 그 문자들 뒤에 이어 저장
		strncat(postfixExp, c, 1);
}

void toPostfix() 
{	/* infixExp를 읽고 postfixStack을 이용해 중위표기식 수식을
	후위표기식 수식으로 변경해 postfixExp에 저장하는 함수 정의 */
	
	// infixExp의 문자를 읽기 위한 포인터 변수
	char *exp = infixExp;
	char x; // 문자를 임시 저장할 변수

	/* exp 포인터 변수를 이용해 infixExp의 문자를 하나씩 읽고 postfixStack을 
	이용해 중위표기식 수식을 후위표기식 수식으로 변환해 postfixExp에 저장 */
	while(*exp != '\0') // 수식의 끝까지 수행

	{	
		if(getPriority(*exp) == operand) /* 수식의 문자가 피연산자이면, 그 문자를 postfixExp에 저장*/
		{ 
			x = *exp; 
        	charCat(&x);
		}
        else if(getPriority(*exp) == lparen) // 수식의 문자가 ( 괄호이면
		{
        	postfixPush(*exp); // 그 문자를 postfixStack에 저장
        }
        else if(getPriority(*exp) == rparen) // 수식의 문자가 ) 괄호이면 
        {
        	while((x = postfixPop()) != '(') { // postfixStack에서 ( 괄호 전까지의 연산자들을 postfixExp에 이어 저장
        		charCat(&x);
        	}
        }
        else // 그 외 연산자(+,-,*,/)의 경우
        {	// postfixStack의 Top 인덱스에 있는 연산자가 exp가 가리키는 문자보다 우선순위가 높거나 같으면
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
            	x = postfixPop(); // postfixStack의 Top에서 연산자를 꺼내 postfixExp에 이어 저장
            	charCat(&x);
            }
            postfixPush(*exp); // 그리고 수식의 문자를 postfixStack의 Top에 저장
        }
        exp++; // 다음 문자를 읽기 위해 인덱스 이동
	}
	/* 중위표기식 수식을 모두 읽고 나면, postfixStack에 저장돼 남아있는 연산자들을 Top부터 postfixExp에 이어 저장 */
    while(postfixStackTop != -1) 
    {
    	x = postfixPop();
    	charCat(&x);
    }

}

void debug() // infixExp, postfixExp, evalResult를 출력하는 함수 정의
{
	// 중위표기식/ 후위표기식 수식 및 계산 결과 출력
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	// 후위표기식 변환할 때, postfixStack 배열에 남아있는 값 출력
	printf("postfixStack : "); 
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() // infixExp, postfixExp, postfixStack 등 초기화하는 함수 정의
{
	// NUL 문자로 수식을 담는 배열 초기화
	infixExp[0] = '\0';
	postfixExp[0] = '\0';
	
	// NUL 문자로 postfixStack 배열을 초기화
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	// Top 인덱스를 나타내는 변수를 -1로 초기화하고 계산 결과값을 0으로 초기화 
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation() // 수식을 읽고 결과값을 계산하는 함수 정의 
{
	int opr1, opr2, i; // 피연산자 변수 opr1, opr2

	int length = strlen(postfixExp); 
	char symbol;
	evalStackTop = -1; // evalStack의 Top 인덱스를 -1로 초기화

	for(i = 0; i < length; i++) // postfixExp의 처음부터 끝까지 확인하면서 계산
	{
		symbol = postfixExp[i]; 
		if(getToken(symbol) == operand) { // 후위표기식 수식의 문자가 피연산자일 경우
			evalPush(symbol - '0'); // 문자를 숫자로 변환해 evalStack에 저장(아스키코드표 이용)
		}
		else { // 후위표기식 수식의 문자가 연산자일 경우
			opr2 = evalPop(); // 중위표기식 수식에서 연산자의 뒤에 오는 피연산자
			opr1 = evalPop(); // 중위표기식 수식에서 연산자의 앞에 오는 피연산자
			switch(getToken(symbol)) { // 해당 연산자에 따라 계산한 값을 evalStack에 저장
			case plus_minus: // +,-우선 순위를 같게 설정했기에 if문을 통해 연산자에 따른 동작을 수행
			if(symbol =='+'){
				evalPush(opr1 + opr2);
			}
			else{
				evalPush(opr1 - opr2); 
			}
			break;
			case times_divide: // *,/ 우선 순위를 같게 설정했기에 if문을 통해 연산자에 따른 동작을 수행
			if(symbol=='*'){
				evalPush(opr1 * opr2);
			}else{
				evalPush(opr1 / opr2);
			}
			break;
			
			default: break;
			}
		}
	}
	evalResult = evalPop(); // 최종 계산값이 담긴 evalStack에서 꺼내 evalResult에 결과값 저장
}

