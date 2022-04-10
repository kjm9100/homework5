#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20


typedef enum{ // ���������� �켱 ���� ����
	lparen = 0,  // ( ��ȣ
	rparen = 4,  // ) ��ȣ
	times_divide = 3,  // * ����or / ������
	plus_minus = 2,    // + ���� or - ����
	operand = 1 // �ǿ�����
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

	printf("[----- [������] [2021041068] -----]\n\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		// ���Ű ����
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) { // ���Ű�� ���� �ٸ� ���� ����
		case 'i': case 'I': // i, I ���Ű ���� ��, ����ǥ��� ���� �Է�
			getInfix();
			break;
		case 'p': case 'P': // p, P ���Ű ���� ��, �Է��� ������ ����ǥ��� �������� ��ȯ
			toPostfix();
			break;
		case 'e': case 'E': // e, E ���Ű ���� ��, ��ȯ�� ����ǥ��� ������ ���
			evaluation();
			break;
		case 'd': case 'D': // d, D ���Ű ���� ��, ����� Ȯ�� �� postfixStack �м�
			debug();
			break;
		case 'r': case 'R': // r, R ���Ű ���� ��, �迭 �� Top �ε��� �ʱ�ȭ
			reset();
			break;
		case 'q': case 'Q': // q, Q ���Ű ���� ��, ���α׷� ����
			break;
		default: // �� �̿��� ���Ű ���� ��, ���� �޽��� ���
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q, Q ���Ű ���� �ÿ��� ���α׷� ����

	return 1; // ���� ���� Ȯ�� ��ȯ
}

void postfixPush(char x) // postfixStack�� ���� ����� ���� �����ڸ� �ִ� �Լ� ����
{ // postfixStack�� Top�� ������ ����(Top �ε����� �ʱ⿡ -1�� �ʱ�ȭ�Ʊ⿡ ���� ���� �� ����)
    postfixStack[++postfixStackTop] = x; 
}

char postfixPop() // postfixStack�� Top�� �ִ� �����ڸ� ������ �Լ� ����
{
	char x;
    if(postfixStackTop == -1) // postfixStack�� �ƹ��͵� ���� ���
        return '\0'; // NUL ���ڸ� ��ȯ
    else { 
	// postfixStack�� �����ڰ� �ִٸ�, postfixStack�� Top�� �ִ� ������ �Ѱ��� �� Top �ε������� 1 ����
    	x = postfixStack[postfixStackTop--];
    }
    return x; // postfixStack�� Top�� �ִ� ������ ��ȯ
}

void evalPush(int x) // evalStack�� �ǿ����� �� �߰� ��� ����� �����ϴ� �Լ� ����
{ // evalStack�� Top�� ����(Top �ε����� �ʱ⿡ -1�� �ʱ�ȭ�Ʊ⿡ ���� ���� �� ����)
    evalStack[++evalStackTop] = x;
}

int evalPop() // evalStack�� ������ ���� �Ѱ��ִ� �Լ� ����
{
    if(evalStackTop == -1) // evalStack�� �ƹ��͵� ���� ��� -1 ��ȯ
        return -1; 
    else // evalStack�� ���� �����Ѵٸ�, evalStack�� Top�� �ִ� ���� ���� ��ȯ
        return evalStack[evalStackTop--]; 
}

void getInfix() // ����ǥ��� ������ �Է¹޴� �Լ� ����
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); // ����ǥ��� ������ �Է¹޾� infixExp �迭�� ����

	for(int i =0; i <strlen(infixExp); i++) // �Է¹��� ���ڿ��� �˻��� (��)�������� ������ ����(�ƽ�Ű�ڵ�ǥ �̿�)
	{	// 0~9,'+','-','*','/','(',')' �̿��� ���ڸ� �Է����� ��� �޽��� ��� �� reset
		if(!((infixExp[i]>='(')&&(infixExp[i]<='9')&&(infixExp[i]!=',')&&(infixExp[i]!='.'))){ 
		printf("Check out the operand and operation\n");
		reset();
		printf("Retype the expression.\n\n");
		return;
		}
	}
}

precedence getToken(char symbol) // (��)�����ڸ� �޾� �װ��� �켱������ �����ϴ� �Լ� ����
{
	/* 					<�켱����>
	<- ����										->����
	( ��ȣ, �ǿ�����, <����, ���ϱ�>, <������, ���ϱ�>, ) ��ȣ

	�켱 ���� ���� �ÿ� mnemonic ����� �� ��ȣ�� �켱 ������ ��ȯ 
	*/
	switch(symbol) {
	case '(' : return lparen; 
	case ')' : return rparen;
	case '+' : case '-' : return plus_minus;
	case '/' : case '*' : return times_divide;
	default : return operand;
	}
}

precedence getPriority(char x) // �켱���� �� (��)������ ������ �˾Ƴ��� �Լ� ����
{
	return getToken(x);
}

void charCat(char* c) // ���ڸ� �޾� postfixExp�� ����
{
	if (postfixExp == '\0') // postfixExp�� ����ٸ�,
		strncpy(postfixExp, c, 1); // ���� �ϳ��� ������ postfixExp�� ����
	else // ���ڵ��� �̹� �ִٸ�, �� ���ڵ� �ڿ� �̾� ����
		strncat(postfixExp, c, 1);
}

void toPostfix() 
{	/* infixExp�� �а� postfixStack�� �̿��� ����ǥ��� ������
	����ǥ��� �������� ������ postfixExp�� �����ϴ� �Լ� ���� */
	
	// infixExp�� ���ڸ� �б� ���� ������ ����
	char *exp = infixExp;
	char x; // ���ڸ� �ӽ� ������ ����

	/* exp ������ ������ �̿��� infixExp�� ���ڸ� �ϳ��� �а� postfixStack�� 
	�̿��� ����ǥ��� ������ ����ǥ��� �������� ��ȯ�� postfixExp�� ���� */
	while(*exp != '\0') // ������ ������ ����

	{	
		if(getPriority(*exp) == operand) /* ������ ���ڰ� �ǿ������̸�, �� ���ڸ� postfixExp�� ����*/
		{ 
			x = *exp; 
        	charCat(&x);
		}
        else if(getPriority(*exp) == lparen) // ������ ���ڰ� ( ��ȣ�̸�
		{
        	postfixPush(*exp); // �� ���ڸ� postfixStack�� ����
        }
        else if(getPriority(*exp) == rparen) // ������ ���ڰ� ) ��ȣ�̸� 
        {
        	while((x = postfixPop()) != '(') { // postfixStack���� ( ��ȣ �������� �����ڵ��� postfixExp�� �̾� ����
        		charCat(&x);
        	}
        }
        else // �� �� ������(+,-,*,/)�� ���
        {	// postfixStack�� Top �ε����� �ִ� �����ڰ� exp�� ����Ű�� ���ں��� �켱������ ���ų� ������
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
            	x = postfixPop(); // postfixStack�� Top���� �����ڸ� ���� postfixExp�� �̾� ����
            	charCat(&x);
            }
            postfixPush(*exp); // �׸��� ������ ���ڸ� postfixStack�� Top�� ����
        }
        exp++; // ���� ���ڸ� �б� ���� �ε��� �̵�
	}
	/* ����ǥ��� ������ ��� �а� ����, postfixStack�� ����� �����ִ� �����ڵ��� Top���� postfixExp�� �̾� ���� */
    while(postfixStackTop != -1) 
    {
    	x = postfixPop();
    	charCat(&x);
    }

}

void debug() // infixExp, postfixExp, evalResult�� ����ϴ� �Լ� ����
{
	// ����ǥ���/ ����ǥ��� ���� �� ��� ��� ���
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	// ����ǥ��� ��ȯ�� ��, postfixStack �迭�� �����ִ� �� ���
	printf("postfixStack : "); 
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() // infixExp, postfixExp, postfixStack �� �ʱ�ȭ�ϴ� �Լ� ����
{
	// NUL ���ڷ� ������ ��� �迭 �ʱ�ȭ
	infixExp[0] = '\0';
	postfixExp[0] = '\0';
	
	// NUL ���ڷ� postfixStack �迭�� �ʱ�ȭ
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	// Top �ε����� ��Ÿ���� ������ -1�� �ʱ�ȭ�ϰ� ��� ������� 0���� �ʱ�ȭ 
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation() // ������ �а� ������� ����ϴ� �Լ� ���� 
{
	int opr1, opr2, i; // �ǿ����� ���� opr1, opr2

	int length = strlen(postfixExp); 
	char symbol;
	evalStackTop = -1; // evalStack�� Top �ε����� -1�� �ʱ�ȭ

	for(i = 0; i < length; i++) // postfixExp�� ó������ ������ Ȯ���ϸ鼭 ���
	{
		symbol = postfixExp[i]; 
		if(getToken(symbol) == operand) { // ����ǥ��� ������ ���ڰ� �ǿ������� ���
			evalPush(symbol - '0'); // ���ڸ� ���ڷ� ��ȯ�� evalStack�� ����(�ƽ�Ű�ڵ�ǥ �̿�)
		}
		else { // ����ǥ��� ������ ���ڰ� �������� ���
			opr2 = evalPop(); // ����ǥ��� ���Ŀ��� �������� �ڿ� ���� �ǿ�����
			opr1 = evalPop(); // ����ǥ��� ���Ŀ��� �������� �տ� ���� �ǿ�����
			switch(getToken(symbol)) { // �ش� �����ڿ� ���� ����� ���� evalStack�� ����
			case plus_minus: // +,-�켱 ������ ���� �����߱⿡ if���� ���� �����ڿ� ���� ������ ����
			if(symbol =='+'){
				evalPush(opr1 + opr2);
			}
			else{
				evalPush(opr1 - opr2); 
			}
			break;
			case times_divide: // *,/ �켱 ������ ���� �����߱⿡ if���� ���� �����ڿ� ���� ������ ����
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
	evalResult = evalPop(); // ���� ��갪�� ��� evalStack���� ���� evalResult�� ����� ����
}

