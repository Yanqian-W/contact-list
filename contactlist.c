// 20411572 scyyw13 Yanqian Wang

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

typedef struct contact{
	char *name;
	char *phone;
	char *email;
	char *department;
	struct contact *next;
}Contact;

void clean_stdin(void);
char* allocateMemory(Contact **head);
int phoneValid(char phone[]);
int emailValid(char email[]);
void printContact(Contact *head);
void insertContact(Contact **head);
void modifyContact(Contact **head, char *input_phone);
void deleteContact(Contact **head, char *input_phone);
void Search(Contact *head, char *str_search);
void freeMemory(Contact **head);


int main(void)
{
	int ifContinue = 1;
	char option[3];
	Contact *head = NULL;  // Initialize the pointer point to the head

	do{
		printf("1) Print contact list\n");
		printf("2) Add a contact\n");
		printf("3) Modify a contact\n");
		printf("4) Delete a contact\n");
		printf("5) Search\n");
		printf("6) Quit\n");
		printf("Option: ");
		
		fgets(option, 3, stdin);
		fflush(stdin);
		
		if(option[0] == '\n')
		{
			continue;
		}
		if(option[1] != '\n')
		{
			printf("Unknown option!\n");
			clean_stdin();   // When the input data do not match the type of the variable
			                 // in the Linux environment, the flush(stdin) is invalid
			continue;
		}
		
		if(option[0]=='1')
		{
			printContact(head);
		}
		else if(option[0]=='2')
		{
			insertContact(&head);
		}
		else if(option[0]=='3')
		{
			char *new_phone;
			printf("Enter phone number to modify: ");
			new_phone = allocateMemory(&head);
			if(new_phone[0] != '\0')
			{
				modifyContact(&head, new_phone);
			}
			free(new_phone);
		}
		else if(option[0]=='4')
		{
			char *delete_phone;
			printf("Enter phone number to delete (return to cancel): ");
			delete_phone = allocateMemory(&head);
			if(delete_phone[0] != '\0')
			{
				deleteContact(&head, delete_phone);
			}
			free(delete_phone);
		}
		else if(option[0]=='5')
		{
			char *str_search;
			printf("Search: ");
			str_search = allocateMemory(&head);
			if(str_search[0] != '\0')
			{
				Search(head, str_search);
			}
			free(str_search);
		}
		else if(option[0]=='6')
		{
			freeMemory(&head);
			exit(-2);
		}
		else{
			printf("Unknown option!\n");
		}
		
	}while(ifContinue == 1);
	
	return 0;
}



void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}



char* allocateMemory(Contact **head)
{
	char* str;
	char* str_copy;
	int i = 1;
	str = (char*)malloc(sizeof(char)*(i+1));
	if(str == NULL)
	{
		printf("Unable to allocate memory.");
		freeMemory(head);
		exit(-1);
	}

	while((str[i-1]=getchar()) != '\n')
	{
		i++;
		str_copy = (char*)malloc(sizeof(char)*(strlen(str)+1));
		if(str_copy == NULL)
		{
			free(str);
			printf("Unable to allocate memory.");
			freeMemory(head);
			exit(-1);
		}
		
		str[i-1] = '\0';
		strcpy(str_copy, str);
		free(str);
		str = (char*)malloc(sizeof(char)*(i+1));
		if(str == NULL)
		{
			free(str_copy);
			printf("Unable to allocate memory.");
			freeMemory(head);
			exit(-1);
		}

		strcpy(str, str_copy);
		free(str_copy);
	}
	str[i-1] = '\0';
	return str;
}



// If the phone number is valid, return 1, else return 0
int phoneValid(char phone[])
{	
	// Check if user entered a zero-length phone number
	if(phone[0] == '\0')
	{
		printf("Phone number cannot be empty, ignoring entry.\n");
		return 0;
	}
	
	// The phone number should contain at least one number
	// Could contain +,-,(,)
	int i;
	int digitNum = 0;
	char str_valid_phone[] = "+-()";
	const int len_phone = strlen(phone);
	for(i=0; i<len_phone; i++)
	{
		if(!(isdigit(phone[i]) || strspn(&phone[i], str_valid_phone)))
		{
			printf("Invalid phone number.\n");
			return 0;
		}
		if(isdigit(phone[i]))
		{
			digitNum++;
		}
	}
	
	if(digitNum == 0)
   	{
   		printf("Invalid phone number.\n");
   		return 0;
	}
	
	return 1;
}



// If the email address is valid, return 1, else return 0
int emailValid(char email[])
{
	if(email[0] != '\0')
   	{
   		// Check the number of '@'
   		int num_aite = 0;
   		const char str_aite[] = "@";
		num_aite = strspn(str_aite, email);
		if(num_aite != 1)
		{
			return 0;
		}
		
		// Prefix cannot be empty
		if(email[0] == '@')
		{
			return 0;
		}
		
		// Separate the prefix and domain of email address
		char *prefix = strtok(email, str_aite);
		char *domain = strtok(NULL, str_aite);
		
		// Domain cannot be empty
		if(domain == NULL)
		{
			return 0;
		}

		// Check if the prefix is valid
		// Allowed characters in prefix: letters, numbers, "_", ".", "-"
		int i;
   		char str_valid_prefix[] = "_.-";
		const int len_prefix = strlen(prefix);
		for(i=0; i<len_prefix; i++)
		{
			if(!(isalnum(prefix[i]) || strspn(&prefix[i], str_valid_prefix)))
			{
				return 0;
			}
			if(strspn(&prefix[i], str_valid_prefix))
			{
				if(!isalnum(prefix[i+1]))
				{
					return 0;
				}
			}
		}

		// Check if the domain is valid
		// There cannot be a '.' after the '@'
		if(domain[0] == '.')
		{
			return 0;
		}
		// Allowed characters in domain: letters, numbers, ".", "-"
		char str_valid_domain[] = ".-";
		const int len_domain = strlen(domain);
		for(i=0; i<len_domain; i++)
		{
			if(!(isalnum(domain[i]) || strspn(&domain[i], str_valid_domain)))
			{
				return 0;
			}
		}
		
		// Allowed characters in the last portion of the domain: letters or numbers
		const char str_point = '.';
		int len_last_portion;
		char *last_portion = strrchr(domain, str_point);
		len_last_portion = strlen(last_portion);
		for(i=1; i<len_last_portion; i++)
		{
			if(!isalnum(last_portion[i]))
			{
				return 0;
			}
		}
		// The last portion must be at least two characters
		if(len_last_portion < 3)
		{
			return 0;
		}
	}
	return 1;
}



void printContact(Contact *head)
{
	Contact *temp = head;
    if(temp == NULL)
    {
        return;
    }
    while (temp != NULL)
    {
        printf("%s\t", temp->phone);
        printf("%s\t", temp->email);
		printf("%s ", temp->name);
		printf("(%s)\n", temp->department);
        temp = temp->next;
    }
}



void insertContact(Contact **head)
{
	// allocate memory
	Contact *new_contact = (Contact*)malloc(sizeof(Contact));
	if(new_contact == NULL)    // failed to allocate memory
   	{
   		freeMemory(head); 
      	printf("Unable to allocate memory.\n");
      	exit(-1);
   	}
	
	// Enter the new data
	fflush(stdin);
	printf("Name: ");
	new_contact->name = allocateMemory(head);
	printf("Number: ");
	new_contact->phone = allocateMemory(head);
	printf("Email: ");
	new_contact->email = allocateMemory(head);
	printf("Department: ");
	new_contact->department = allocateMemory(head);
	new_contact->next = NULL; 

	// Check if the input phone number is valid
	int if_phone_valid;
	char test_phone[strlen(new_contact->phone)+1];
	strcpy(test_phone, new_contact->phone);   // copying new_phone into test_phone
	if_phone_valid = phoneValid(test_phone);
   	
   	// Check if the input email is valid
   	int if_email_valid;
   	char test_email[strlen(new_contact->email)+1];
	strcpy(test_email, new_contact->email);   // copying new_email into test_email
   	if_email_valid = emailValid(test_email);
   	if(if_email_valid == 0)
   	{
   		printf("Invalid email id.\n");
	}

   	// Put new_contact in the end of the linked-list
	// If list is empty, move the head to point to the new node
	if(*head == NULL)
	{
		if(if_phone_valid==0 || if_email_valid==0)
   		{
   			free(new_contact->name);
   			free(new_contact->phone);
   			free(new_contact->email);
   			free(new_contact->department);
   			free(new_contact);
   			return;
		}
    	*head = new_contact;
		return;
	}
   
   	// Iterate until find the last node
   	int if_equal;
   	Contact *pre;
	Contact *cur = *head;
	while(cur != NULL)
	{
		pre = cur;
		// Check if that phone number already exists in the contact list
		if_equal = strcmp(cur->phone, new_contact->phone);
		if(if_equal == 0){
			printf("Entry already exists, ignoring duplicate entry.\n");
			break;
		}
   		cur = cur->next;
	}
	
	if(if_phone_valid==0 || if_email_valid==0 || if_equal==0)
   	{
   		free(new_contact->name);
   		free(new_contact->phone);
   		free(new_contact->email);
   		free(new_contact->department);
   		free(new_contact);
   		return;
	}
	// Put new_contact in the linked-list
	pre->next = new_contact;
}



void modifyContact(Contact **head, char *input_phone)
{
	// Check if this entry already exists
	int if_equal;
	int temp_num = 0;
	Contact *temp = *head;
	while(temp != NULL)
	{
		// Return zero if it is the same string
		if_equal = strcmp(temp->phone, input_phone);
		if(if_equal == 0)
		{
			break;
		}
		temp = temp->next;
		temp_num++;
	}
	if(temp == NULL)
	{
		printf("Entry does not exist.\n");
		return;
	}
	
	// This entry does already exists
	// Enter the new data
	char *new_name;
	char *new_phone;
	char *new_email;
	char *new_department;
	
	printf("Enter new name (return to keep [%s]): ", temp->name);
	new_name = allocateMemory(head);
	printf("Enter new number (return to keep [%s]): ", temp->phone);
	new_phone = allocateMemory(head);
	printf("Enter new email (return to keep [%s]): ", temp->email);
	new_email = allocateMemory(head);
	printf("Enter new department (return to keep [%s]): ", temp->department);
	new_department = allocateMemory(head);
	
	// Check if the input phone number is valid
	int if_phone_valid;
	if(new_phone[0] == '\0')
	{
		if_phone_valid = 1;
	}else
	{
		char test_phone[strlen(new_phone)+1];
		strcpy(test_phone, new_phone);   // copying new_phone into test_phone
		if_phone_valid = phoneValid(test_phone);
	}

	// Check if the new phone does already exists in the contact list
	Contact *pre = *head;
	if(if_phone_valid==1 && pre->next!=NULL)
	{
		temp = temp->next;
		while(temp != NULL)
		{
			if_equal = strcmp(temp->phone, new_phone);
			if(if_equal == 0)
			{
				printf("Entry already exists, ignoring duplicate entry.\n");
				if_phone_valid = 0;
				break;
			}
			temp = temp->next;
		}
		
		temp = *head;
		while(temp!=NULL && temp_num>0)
		{
			if_equal = strcmp(temp->phone, new_phone);
			if(if_equal == 0)
			{
				printf("Entry already exists, ignoring duplicate entry.\n");
				if_phone_valid = 0;
				break;
			}
			temp = temp->next;
			pre = temp;  // pre finally point to the target node
			temp_num--;
		}
	}

	// Check if the input email address is valid
	int if_email_valid;
	char test_email[strlen(new_email)+1];
	strcpy(test_email, new_email);   // copying new_email into test_email
   	if_email_valid = emailValid(test_email);
   	if(if_email_valid == 0)
   	{
   		printf("Invalid email id.\n");
	}
   	
   	if(if_phone_valid==0 || if_email_valid==0)
   	{
   		free(new_name);
   		free(new_phone);
   		free(new_email);
   		free(new_department);
   		return;
	}

	// Modify the contact list
	if(new_name[0] != '\0'){
		free(pre->name);
		pre->name = new_name;
	}else{
		free(new_name);
	}
	if(new_phone[0] != '\0'){
		free(pre->phone);
		pre->phone = new_phone;
	}else{
		free(new_phone);
	}
	if(new_email[0] != '\0'){
		free(pre->email);
		pre->email = new_email;
	}else{
		free(new_email);
	}
	if(new_department[0] != '\0'){
		free(pre->department);
		pre->department = new_department;
	}else{
		free(new_department);
	}
}

	

void deleteContact(Contact **head, char *input_phone)
{
	int if_equal;
	Contact *pre = *head;
	Contact *cur;
		
	// If the first element matches
	if_equal = strcmp(pre->phone, input_phone);
	if(if_equal == 0)
	{
		printf("Successfully deleted the entry for %s\n", input_phone);
		*head = pre->next;
		free(pre->name);
   		free(pre->phone);
   		free(pre->email);
   		free(pre->department);
		free(pre);
		return;
	}
		
	cur = pre->next;
	while(cur != NULL)
	{
		if_equal = strcmp(cur->phone, input_phone);
		if(if_equal == 0)
		{
			printf("Successfully deleted the entry for %s\n", input_phone);
			pre->next = cur->next;
			free(cur->name);
   			free(cur->phone);
   			free(cur->email);
   			free(cur->department);
			free(cur);
			return;
		}
		pre = cur;
		cur = cur->next;
	}
	// No such entry exists, do nothing
}



void Search(Contact *head, char *str_search)
{
	char *part1;
	char *part2;
	char *part3;
	char *part4;
		
	Contact *temp = head;
	while(temp != NULL)
	{
		part1 = strstr(temp->name, str_search);
		part2 = strstr(temp->phone, str_search);
		part3 = strstr(temp->email, str_search);
		part4 = strstr(temp->department, str_search);
			
		if(part1||part2||part3||part4)  // find
		{
			printf("%s\t", temp->phone);
        	printf("%s\t", temp->email);
			printf("%s ", temp->name);
			printf("(%s)\n", temp->department);
		}
		temp = temp->next;
	}
}



void freeMemory(Contact **head)
{
	Contact *temp = *head;
	while(temp != NULL)
	{
		*head = temp->next;
		free(temp->name);
   		free(temp->phone);
   		free(temp->email);
   		free(temp->department);
		free(temp);
		temp = *head;
	}
}











