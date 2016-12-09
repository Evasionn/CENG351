#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define mainFile "SampleFile.txt"
#define firstFile "first.txt"
#define secondFile "second.txt"

#define N 1000000

void createAndDivideText(FILE * mFile)
{
	int count = 0;
	int buffer;

	FILE * first = fopen(firstFile, "w");
	FILE * second = fopen(secondFile, "w");

	while (fscanf(mFile, "%d", &buffer) != EOF)
	{
		if (count < N / 2) fprintf(first, "%d\n", buffer);
		else fprintf(second, "%d\n", buffer);

		++count;
	}
	fclose(first);
	fclose(second);
}


void txtFileToArr(int * arr, char * txtName)
{

	FILE * textFile = fopen(txtName, "r");
	int buffer;
	int i = 0;
	while (fscanf(textFile, "%d", &buffer) != EOF)
	{
		arr[i] = buffer;
		i++;
	}
	fclose(textFile);
}

void quickSort(int arr[], int left, int right)
{
	int i = left;
	int j = right;
	int pivot = arr[(left + right) / 2];
	int temp;
	while (i <= j)
	{
		while (arr[i] < pivot) i++;
		while (arr[j] > pivot) j--;
		if (i <= j)
		{
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	}
	if (left < j) quickSort(arr, left, j);
	if (i < right) quickSort(arr, i, right);
}



void saveFile(int arr[], char * textFile)
{
	FILE * txtFile = fopen("temp.txt", "w");
	int size = N / 2;
	for (int i = 0; i < size; i++)
		fprintf(txtFile, "%d\n", arr[i]);

	fclose(txtFile);
	remove(textFile);
	rename("temp.txt", textFile);
}

void sortAndSave() {
	int size = N / 2;
	int * arr = (int *)malloc(sizeof(int)* size);
	int * arr2 = (int *)malloc(sizeof(int) * size);

	txtFileToArr(arr, firstFile);
	quickSort(arr, 0, size - 1);
	saveFile(arr, firstFile);
	free(arr);
	
	txtFileToArr(arr2, secondFile);
	quickSort(arr2, 0, size-1);
	saveFile(arr2, secondFile);
	free(arr2);
}

void merge()
{
	FILE * first = fopen(firstFile, "r");
	FILE * second = fopen(secondFile, "r");
	FILE * sortedFile = fopen("sorted.txt", "w");
	int topOfFirst, topOfSecond;
	fscanf(first, "%d", &topOfFirst);
	fscanf(second, "%d", &topOfSecond);

	int firstEnd = 0, secondEnd = 0;

	while (firstEnd == 0 && secondEnd == 0) {
		if (topOfFirst < topOfSecond)
		{
			fprintf(sortedFile, "%d\n", topOfFirst);
			if (fscanf(first, "%d", &topOfFirst) == EOF) firstEnd = 1;
		}
		else
		{
			fprintf(sortedFile, "%d\n", topOfSecond);
			if (fscanf(second, "%d", &topOfSecond) == EOF) secondEnd = 1;
		}
	}

	if (firstEnd == 1)
	{
		while (fscanf(second, "%d", &topOfSecond) != EOF) fprintf(sortedFile, "%d\n", topOfSecond);
	}
	if (secondEnd == 1)
	{
		while (fscanf(first, "%d", &topOfFirst) != EOF) fprintf(sortedFile, "%d\n", topOfFirst);
	}
	fclose(sortedFile);
	fclose(first);
	fclose(second);
}

void sortAllFile()
{
	FILE * mFile = fopen(mainFile, "r");
	if (mFile == NULL)
	{
		puts("There is not a file to read.");
		return;
	}
	createAndDivideText(mFile);
	fclose(mFile);

	sortAndSave();
	merge();

}


int main()
{
	sortAllFile();

	return 0;
}