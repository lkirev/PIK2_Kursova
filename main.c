#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

typedef struct Question
{
    char question[50];
    float importance;
    char a[20];
    char b[20];
    char c[20];
    char d[20];
    float points1;
    float points2;
    float points3;
    float points4;
    float maxPoints;
} question_t;

typedef struct Person
{
    char ID[16];
    char names[70];
    int gender;
	char date[11];
	char interests[250];
	int status;
	int numberOfQuestions;
	question_t questionData[10];
} person_t;

typedef struct Node
{
	person_t personData;
	struct Node *next;
} node;

void printNode (node *head);
void addNode(node** head, person_t personData, int mode);
void deleteNode(node** head);
char* getFileName();
void loadFile(node** head);
void saveFile(node* head);
person_t initPerson(node *head);
int getID(node* head, char* ID, int mode);
int isUnique(node* head, char* ID);
void getNames(char *names);
void getGender(int *gender);
int getDate(char* date);
void getInterests(char *interests);
void getStatus(int *status);
question_t getQuestions(question_t data);
void askRandomPeople(node **head);
int countList(node *head);
void deletePerson(node **head);
char menu();

int main()
{
    node *head;
    while(1)  {
		system("cls");
		switch(menu())
		{
			case '1': loadFile(&head);
				break;
			case '2': saveFile(head);
				break;
			case '3': addNode(&head, initPerson(head), 1);
				break;
			case '4': printNode(head);
				break;
			case '5': askRandomPeople(&head);
				break;
			case '6': deletePerson(&head);
				break;
			default:
                deleteNode(&head);
				return 0;
		}
		system("pause");
	}
}

char menu()
{
	char i;

	printf("1. Load file\n");
	printf("2. Save file\n");
	printf("3. Add person\n");
	printf("4. Print all persons\n");
	printf("5. Pick random person\n");
	printf("6. Delete person\n");
	printf("Ctrl + C to EXIT\n");

	do
	{   printf("Select: ");
		fflush(stdin);
		scanf("%c", &i);
	}   while(i < '0' || i > '6');

	return i;
}

void printPerson(person_t personData)
{
	printf("ID: %s\n", personData.ID);
	printf("Names: %s\n", personData.names);
	printf("Gender: %d\n", personData.gender);
	printf("Date of birth: %s\n", personData.date);
	printf("Interests: %s\n", personData.interests);
	printf("Status: %d\n", personData.status);
}

void addNode(node** head, person_t personData, int mode)
{
	node* temp = (node*)malloc(sizeof(node));
	node* temp2 = *head;
	temp->personData = personData;
	temp->next = NULL;

	if(!(*head))  {
		*head = temp;
		if(mode == 1)  {
			printf("First Person added successfully!\n");
        }
		return;
	}

	while(temp2->next)  {
		temp2 = temp2->next;
    }

	temp2->next = temp;
	if(mode == 1)
		printf("New Person added successfully!\n");
}

void deleteNode(node** head)
{
	node* temp;

	while(*head)  {
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
}

char* getFileName()
{
	char *fileName = (char*)malloc(50*sizeof(char));

	printf("\nEnter file name: ");
	fflush(stdin);
	gets(fileName);

	if(!strchr(fileName, '.'))  {
		strcat(fileName, ".bin");
	}

	return fileName;
}

void loadFile(node** head)
{
	FILE* fp;
	person_t data;
	char *fileName;

	fileName = getFileName();

	if(!(fp = fopen(fileName, "rb")))  {
		fprintf(stderr, "Error loading file!\a\n");
		free(fileName);
		return;
	}

	if(*head)
		deleteNode(head);

	while(1)  {
		if(!fread(&data, sizeof(data), 1, fp))
			break;

		addNode(head, data, 2);
	}

	fclose(fp);

	if(!(*head))
		printf("No data loaded from \"%s\"\n", fileName);
	else
		printf("Info loaded successfully from \"%s\"\n", fileName);

	free(fileName);
}

void saveFile(node* head)
{
	FILE* fp;
	char *fileName;

	if(!head)  {
		printf("List is empty!\n");
		printf("Add some new people first!\n");
		return;
	}

	fileName = getFileName();

	if(!(fp = fopen(fileName, "wb")))  {
		exit(1);
	}

	while(head)  {
		if(fwrite(&head->personData, sizeof(person_t), 1, fp) != 1){
			printf("Error occured while writing on file\n");
			exit(1);
		}
		head = head->next;
	}

	fclose(fp);

	printf("Info saved successfully to \"%s\"\n", fileName);
	free(fileName);
}

person_t initPerson(node *head)
{
    person_t data;
    int i;
    char c;

    while(!(getID(head, data.ID, 1)));
    getNames(data.names);
    getGender(&data.gender);
    while(!getDate(data.date));
    getInterests(data.interests);
    getStatus(&data.status);

    for(i = 0; i<10; i++)
    {
        printf("Do you want to add question <Y/N>: ");

        while(1)  {
            fflush(stdin);
            c = toupper(getchar());
            if(c == 'N')
                return data;
            if(c == 'Y')
                break;
            printf("Select <Y/N>: ");
        }
        data.questionData[i] = getQuestions(data.questionData[i]);
        data.numberOfQuestions = i + 1;
    }

    return data;
}

int getID(node* head, char* ID, int mode)
{
	int i, len;

	printf("Enter unicode(letter and digits - 4 - 15 chars): ");
	fflush(stdin);
	gets(ID);

	if(strlen(ID) < 4 || strlen(ID) > 15)  {
		printf("Unicode must be 4 - 15 characters!\n\n");
		return 0;
	}
	if(!isalpha(ID[0]))  {
		printf("Unicode must start with a letter!\n\n");
		return 0;
	}

	len = strlen(ID);
	for(i = 1; i < len; i++)
	{
		if(!isdigit(ID[i]))  {
			printf("Unicode must contain a letter and DIGITS!\n\n");
			return 0;
		}
	}
    if(mode == 1)  {
        if( !isUnique(head, ID) )  {
            printf("Unicode already exist!\n\n");
            return 0;
        }
	}
	if(mode == 2)  {
        if( !isUnique(head, ID) )  {
            return 1;
        }
	}
	return 1;
}

int isUnique(node* head, char* ID)
{
	while(head)  {
		if(!strcmp(head->personData.ID, ID))
			return 0;
		head = head->next;
	}
	return 1;
}

void getNames(char *names)
{
    char name[20], fatherName[20], surname[20];

    do
    {   printf("Enter name: ");
        fflush(stdin);
        gets(name);
    } while(!(strlen(name)));

    do
    {   printf("Enter father's name: ");
        fflush(stdin);
        gets(fatherName);
    } while(!(strlen(fatherName)));

    do
    {   printf("Enter surname: ");
        fflush(stdin);
        gets(surname);
    } while(!(strlen(surname)));

    sprintf(names, "%s %s %s", name, fatherName, surname);
}

void getGender(int *gender)
{
    do
    {printf("Enter gender. \"0\" for male, \"1\" for female: ");
        scanf("%d", gender);
    } while(*gender < 0 || *gender > 1);
}

int getDate(char* date)
{
	int year, month, day;

	printf("\nEnter year of birth: ");
	scanf("%u", &year);
	printf("Enter month: ");
	scanf("%u", &month);
	printf("Enter day: ");
	scanf("%u", &day);

	sprintf(date, "%.2u/%.2u/%u", day, month, year);

	return 1;
}

void getInterests(char *interests)
{
    do
    {   printf("Enter your interests: ");
        fflush(stdin);
        gets(interests);
    } while(!strlen(interests));
}

void getStatus(int *status)
{
    do
    {   printf("Enter your status: \"0\" - single \"1\" - in a relationship \"2\" - married: ");
        scanf("%d", status);
    } while(*status < 0 || *status > 2);
}

question_t getQuestions(question_t data)
{
    do
    {   printf("Enter question: ");
        gets(data.question);
    } while(!strlen(data.question));

    do
    {   printf("Enter answer A: ");
        gets(data.a);
    } while(!strlen(data.a));

    do
    {   printf("Enter answer B: ");
        gets(data.b);
    } while(!strlen(data.b));

    do
    {   printf("Enter answer C: ");
        gets(data.c);
    } while(!strlen(data.c));

    do
    {   printf("Enter answer D: ");
        gets(data.d);
    } while(!strlen(data.d));

    do
    {   printf("Enter importance of question: ");
        scanf("%f", &data.importance);
    } while (data.importance < 0 || data.importance > 1);

    do
    {   printf("Enter weight of answer A: ");
        scanf("%f", &data.points1);
    } while (data.points1 < 0);

    do
    {   printf("Enter weight of answer B: ");
        scanf("%f", &data.points2);
    } while (data.points2 < 0);

    do
    {   printf("Enter weight of answer C: ");
        scanf("%f", &data.points3);
    } while (data.points3 < 0);

    do
    {   printf("Enter weight of answer D: ");
        scanf("%f", &data.points4);
    } while (data.points4 < 0);

    data.points1 *= data.importance;
    data.points2 *= data.importance;
    data.points3 *= data.importance;
    data.points4 *= data.importance;

    if(data.points1 > data.points2 && data.points1 > data.points3 && data.points1 > data.points4)
        data.maxPoints = data.points1;
    if(data.points2 > data.points1 && data.points2 > data.points3 && data.points2 > data.points4)
        data.maxPoints = data.points2;
    if(data.points3 > data.points2 && data.points3 > data.points1 && data.points3 > data.points4)
        data.maxPoints = data.points3;
    if(data.points4 > data.points2 && data.points4 > data.points3 && data.points4 > data.points1)
        data.maxPoints = data.points4;

    return data;
}

void printQuestion (question_t data)
{
    printf("Question: %s\n", data.question);
    printf("A: %s\n", data.a);
    printf("B: %s\n", data.b);
    printf("C: %s\n", data.c);
    printf("D: %s\n", data.d);
}

void askRandomPeople(node **head)
{
    node *temp = *head;
    int n = 0, j, i = 0;
    char c;

    if(!(*head))  {
        printf("List is empty!\n");
        return;
    }

    n = countList(*head);
    srand(time(NULL));
    n= rand() % (n) ;

    while(1)
    {
        if(i == n)  {
            for(j = 0; j < temp->personData.numberOfQuestions; j++)
            {
                printQuestion(temp->personData.questionData[j]);

                while(1)  {
                    fflush(stdin);
                    printf("Choose <A/B/C/D>: ");
                    c = toupper(getchar());
                    if(c == 'A')
                        break;
                    if(c == 'B')
                        break;
                    if(c == 'C')
                        break;
                    if(c == 'D')
                        break;
                }

                switch (c)
                {
                    case 'A':
                        printf("Your answer is \"%s\". %.2f/%.2f points\n", temp->personData.questionData[j].a, temp->personData.questionData[j].points1, temp->personData.questionData[j].maxPoints);
                        break;
                    case 'B':
                        printf("Your answer is \"%s\". %.2f/%.2f points\n", temp->personData.questionData[j].b, temp->personData.questionData[j].points2, temp->personData.questionData[j].maxPoints);
                        break;
                    case 'C':
                        printf("Your answer is \"%s\". %.2f/%.2f points\n", temp->personData.questionData[j].c, temp->personData.questionData[j].points3, temp->personData.questionData[j].maxPoints);
                        break;
                    case 'D':
                        printf("Your answer is \"%s\". %.2f/%.2f points\n", temp->personData.questionData[j].d, temp->personData.questionData[j].points4, temp->personData.questionData[j].maxPoints);
                        break;
                }
            }
            if(j == temp->personData.numberOfQuestions)
                break;
            }
        }
    }

int randomGen(node **head)
{
    int n = countList(*head);
    srand(time(NULL));
    int r = rand() % (n) ;

    printf(">>>%d<<<", r);

    return r;
}

int countList(node *head)
{
    int n=1;

    while(1)  {
        if(head->next != NULL)  {
           n++;
           head = head->next;
        }
        else break;
    }
    return n;
}

void deletePerson(node **head)
{
    node *temp = *head;
    char searchID[16];
    int poss = 1;
    char c;

    if(!(*head))  {
        printf("Empty list!\n");
        return;
    }
    while(1)  {
        printf("Enter ID to delete a person: ");
        if(getID(*head, searchID, 2))   {
            break;
        }
    }
    if(isUnique(*head, searchID))   {
        printf("Not found!\n");
        return;
    }
    while(temp)   {
		if(!(strcmp(temp->personData.ID, searchID)))   {
			printPerson(temp->personData);
			printf("\nDo you want to delete this person<Y/N>: ");
			while(1)   {
				fflush(stdin);
				c = toupper(getchar());
				if(c == 'N')
					return;
				 if(c == 'Y')
					break;
				printf("Select <Y/N>: ");
			}
			printf("Person deleted!\n");
			return;
		}
		else   {
			temp = temp->next;
			++poss;
		}
	}
}

void printNode (node *head)
{
    int i=0;

    if(!head)   {
        printf("List is empty!\n");
        return;
    }

    while(head)   {
        printf("Person %d\n", ++i);
        printPerson(head->personData);
        head=head->next;
    }
}
