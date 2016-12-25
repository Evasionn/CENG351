#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define indexFile "index.txt" //Index file format is (INDEX ID)
#define studentFile "student.txt" //Student file format is (INDEX ID NAME)

typedef struct s
{
	int ID;
	char name[32];
}Student;

typedef struct
{
	Student * list;
	int size;
}StudentList;

void checkTheFile(FILE *);
StudentList createList();
Student createStudent(int, char *);
void getList(StudentList *);
void addStudent(StudentList * sList);
int isExist(StudentList, int);
void addToList(StudentList *, Student);
void addToFile(StudentList);
void copyTheList(StudentList*, StudentList*);
void fillIndexList(int[], StudentList *);
void updateIndexFile(StudentList*);
void quickSort(StudentList *, int, int, int[]);
int menu();
void searchStudent(StudentList *);
int findIndex(int);


int main()
{
	StudentList sList = createList();
	getList(&sList);
	int selection = -1;
	while(selection!=3)
	{ 
		selection = menu();
		switch (selection)
		{
		case 1:
			addStudent(&sList);
			break;
		case 2:
			searchStudent(&sList);
			break;
		}
	}
	

	return 0;
}
void checkTheFile(FILE * theFile)
{
	if (theFile == NULL)
	{
		printf("An error has been occured.");
		exit(0);
	}
}

int menu()
{
	int selection;
	puts("1.) Add a new student");
	puts("2.) Search a student by student ID");
	puts("3.) Exit");

	while (scanf("%d", &selection) != 1)
	{
		puts("1.) Add a new student");
		puts("2.) Search a student by student ID");
		puts("3.) Exit");
		while (getchar() != '\n');
	}
	return selection;
}

StudentList createList()
{
	StudentList sList;

	sList.list = (Student *)malloc(sizeof(Student));
	sList.size = 0;
	return sList;
}

Student createStudent(int ID, char* name)
{
	Student newStudent;
	newStudent.ID = ID;
	strcpy(newStudent.name, name);
	return newStudent;
}

void getList(StudentList * sList)
{
	FILE * theFile = fopen(studentFile, "r");
	checkTheFile(theFile);
	Student tempStudent;
	int index;
	while (fscanf(theFile, "%d %d %s", &index, &tempStudent.ID, tempStudent.name) != EOF)
	{
		addToList(sList, tempStudent);
	}
	fclose(theFile);
}

int isExist(StudentList sList, int ID)
{
	for (int i = 0; i < sList.size; i++)
	{
		if (sList.list[i].ID == ID) return 1;
	}
	return 0;
}

void addToList(StudentList * sList, Student newStudent)
{
	sList->list = (Student *)realloc(sList->list, sizeof(Student)*(sList->size + 1));
	sList->list[sList->size] = newStudent;
	sList->size++;
}

void addToFile(StudentList sList)
{
	FILE * theFile = fopen(studentFile, "a+");
	checkTheFile(theFile);
	fprintf(theFile, "%d %d %s\n", sList.size - 1, sList.list[sList.size - 1].ID, sList.list[sList.size - 1].name);
	fclose(theFile);
}

void copyTheList(StudentList* original, StudentList* copied)
{
	Student temp;
	for (int i = 0; i < original->size; i++)
	{
		temp = original->list[i];
		addToList(copied, temp);
	}
}
void fillIndexList(int arr[], StudentList * indexList)
{
	FILE * theFile = fopen(studentFile, "r");
	checkTheFile(theFile);
	int index;
	Student temp;
	for (int i = 0; i < indexList->size; i++)
	{
		fscanf(theFile, "%d %d %s", &index, &temp.ID, temp.name);
		arr[i] = index;
	}
	fclose(theFile);
}
void updateIndexFile(StudentList* indexList)
{
	int * iList = (int*)malloc(sizeof(int)*indexList->size);
	fillIndexList(iList, indexList);
	quickSort(indexList, 0, indexList->size - 1, iList);
	FILE * theFile = fopen(indexFile, "w");
	checkTheFile(theFile);
	for (int i = 0; i < indexList->size; i++)
	{
		fprintf(theFile, "%d %d\n", iList[i], indexList->list[i].ID);
	}
	fclose(theFile);
	free(iList);
}

void quickSort(StudentList *arr, int left, int right, int indexList[])
{
	int i = left;
	int j = right;
	int pivot = arr->list[(left + right) / 2].ID;
	Student temp;
	int tempIndex;
	while (i <= j)
	{
		while (arr->list[i].ID < pivot) i++;
		while (arr->list[j].ID > pivot) j--;
		if (i <= j)
		{
			temp = arr->list[i];
			arr->list[i] = arr->list[j];
			arr->list[j] = temp;

			tempIndex = indexList[i];
			indexList[i] = indexList[j];
			indexList[j] = tempIndex;

			i++;
			j--;
		}
	}

	if (left < j) quickSort(arr, left, j, indexList);
	if (i < right) quickSort(arr, i, right, indexList);

}
void addStudent(StudentList * sList)
{
	int ID;
	char name[32];

	printf("Enter Student ID: ");
	while (scanf("%d", &ID) != 1)
	{
		printf("Enter Student ID: ");
		while (getchar() != '\n');
	}
	if (isExist(*sList, ID))
	{
		puts("This student ID is already exist");
		return;
	}


	printf("Enter Student Name: ");
	scanf("%s", name);

	Student newStudent;
	newStudent = createStudent(ID, name);
	addToList(sList, newStudent);
	addToFile(*sList);

	StudentList indexList = createList();
	copyTheList(sList, &indexList);
	updateIndexFile(&indexList);
	free(indexList.list);
}


int findIndex(int ID)
{
	int tempIndex;
	int tempID;
	int result = -1;
	FILE * theFile = fopen(indexFile, "r");
	checkTheFile(theFile);
	while (fscanf(theFile, "%d %d", &tempIndex, &tempID) != EOF)
	{
		if (tempID == ID) result = tempIndex;
	}
	fclose(theFile);
	return result;
}

void searchStudent(StudentList * sList)
{
	int ID;
	int index;

	printf("Enter a Student ID: ");
	while (scanf("%d", &ID) != 1)
	{
		printf("Enter a Student ID: ");
		while (getchar() != '\n');
	}
	index = findIndex(ID);

	if (index == -1) puts("The student couldn't be found.");
	else printf("Student ID: %d\nStudent Name: %s\n", sList->list[index].ID, sList->list[index].name);
}