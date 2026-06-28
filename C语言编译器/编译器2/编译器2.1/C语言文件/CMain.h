#include <stdio.h>
#include <stdlib.h>

struct Questions
{
	unsigned __int8 question1 : 1;
	unsigned __int8 question2 : 1;
	unsigned __int8 question3 : 1;
	unsigned __int8 question4 : 1;
	unsigned __int8 question5 : 1;
};

struct Quests
{
	unsigned __int8 question1;
	unsigned __int8 question2;
	unsigned __int8 question3;
	unsigned __int8 question4;
	unsigned __int8 question5;
};

int run(void)
{
	struct Questions questionset={1,0,1,0,1};
	struct Quests questset = { 1,0,1,0,1 };
	unsigned __int8 a;
	a = 0;
	printf("sizeof(Questions)=%i\n",sizeof(questionset));
	printf("sizeof(Quests)=%i\n", sizeof(questset));
	printf("sizeof(unsigned __int8)=%i\n", sizeof(a));
	printf("The answers to the questions:\n");
	printf("\tQuestion 1 : %s\n", (questionset.question1)?"True":"False");
	printf("\tQuestion 2 : %s\n", (questionset.question2)?"True":"False");
	printf("\tQuestion 3 : %s\n", (questionset.question3)?"True":"False");
	printf("\tQuestion 4 : %s\n", (questionset.question4)?"True":"False");
	printf("\tQuestion 5 : %s\n", (questionset.question5)?"True":"False");
	printf("");
	system("pause");
	return 0;
}