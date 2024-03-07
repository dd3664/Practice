/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef struct ListNode{
	int value;
	struct ListNode *next;
} LISTNODE;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
	struct ListNode *head = NULL;
	struct ListNode *tail = NULL;
	int n1, n2, sum;
	int carry = 0;

	while(l1 || l2)
	{
		n1 = l1 ? l1->value : 0;
		n2 = l2 ? l2->value : 0;
		sum = n1 + n2 + carry;

		if (NULL == head)
		{
			head = (struct ListNode *)malloc(sizeof(struct ListNode));
			head->value = sum % 10;
			head->next = NULL;
			tail = head;
		}
		else
		{
			tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
			tail->next->value = sum % 10;
			tail->next->next = NULL;
			tail = tail->next;
		}

		carry = sum / 10;

		if (l1)
		{
			l1 = l1->next;
		}
		if (l2)
		{
			l2 = l2->next;
		}
	}

	if (carry > 0)
	{
		tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
		tail->next->value = carry;
		tail->next->next = NULL;
		tail = tail->next;
	}

	return head;
}

int main(int argc, char *argv[])
{
	struct ListNode *l1, *l2, *tail, *ret, *tmp;
	int l1num[] = {9, 2, 3};
	int l2num[] = {2, 3 ,4};

	tail = l1 = (struct ListNode *)malloc(sizeof(struct ListNode));
	tail->value = l1num[0];
	tail->next = NULL;

	tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
	tail->next->value = l1num[1];
	tail->next->next = NULL;
	tail = tail->next;

	tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
	tail->next->value = l1num[2];
	tail->next->next = NULL;
	tail = tail->next;

	tail = l2 = (struct ListNode *)malloc(sizeof(struct ListNode));
	tail->value = l2num[0];
	tail->next = NULL;

	tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
	tail->next->value = l2num[1];
	tail->next->next = NULL;
	tail = tail->next;

	tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
	tail->next->value = l2num[2];
	tail->next->next = NULL;
	tail = tail->next;

	ret = addTwoNumbers(l1, l2);
	while (ret)
	{
		printf("%d", ret->value);
		tmp = ret;
		ret = ret->next;
		free(tmp);
	}

	return 0;
}
