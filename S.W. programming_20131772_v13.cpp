#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <io.h>
#include <string>
#include <time.h>
#define MAX 9999
#define MAX_NUM 1000

//������ ��Ȯ��, �ֽż�, �̸������� ��带 �Է¹޾� �����ϴ� ���α׷�

//���� ���� �д� �Լ�
float Fileopen(char *filename, char *keyword);
//������ ��Ȯ���� �Ǻ��ϴ� �Լ�
float Finddata(char *data, char *keyword);
//�� ���� Swap �Լ�, �����ε�
void Swap(int* a, int* b);
void Swap(float* a, float* b);
void SwapFile(char *a, char *b);
//QuickSort �Լ�, �����ε�
void QuickSort(char **File, int* array, float* array2, int start, int end);
void QuickSort(char **File, float* array, int start, int end);
//����� ��� �Լ�
void PrintFile(char **File, int start, int end);
void PrintFile(char **File, float *FileContent, int start, int end);

void main()
{
	//���� ����
	char **File;             //�Լ��̸� �����ϴ� ������
	int filenumber = 0;      //�� ���� ������ ����
	int *FileSize;           //���� ũ�⸦ �����ϴ� ������
	int *Filetime;           //���� �ֱ� ������¥�� �����ϴ� ������
	float *FileContent;      //���� ��Ȯ�� ������ �����ϴ� ������
	float gap;
	
	struct _finddata_t fd;
	long handle;
	int result = 1, mode;
	char keyword[30];
	time_t startTime = 0, endTime = 0;
	
	printf("\n****SW programming Project 1\n");
	printf("\n****���� ��ġ ���α׷�\n");
	printf("\n--------------------------------------------------------------------------------");
	
	File = (char**)malloc(sizeof(char) * MAX_NUM);
	FileSize = (int*)malloc(sizeof(int) * MAX_NUM);
	FileContent = (float*)malloc(sizeof(float) * MAX_NUM);
	Filetime = (int*)malloc(sizeof(int) * MAX_NUM);

	printf("\n****ã�� Ű���带 ������ �ּ���\n");
	printf("\t>>");
	scanf("%s", keyword);

	printf("\n****�˻� �ɼ��� ������ �ּ���");
	printf("\n\t>>��Ȯ�� : 1 \n\t>>�̸��� : 2 \n\t>>�ֽż� : 3\n");
	scanf("\t%d", &mode);

	startTime = clock();
	//���� ��ġ
	handle = _findfirst(".\\*.txt", &fd);
	if (handle == -1)
		return;	

	while (result != -1)
	{
		//2���� ���� �Ҵ�
		File[filenumber] = (char*)malloc(sizeof(char) * fd.size);
		
		//���� �ð����� ��������
		Filetime[filenumber] = (int)fd.time_write;
		
		strcpy(File[filenumber], fd.name);
		FileSize[filenumber] = strlen(fd.name);
	
		FileContent[filenumber] = Fileopen(fd.name, keyword);
		result = _findnext(handle, &fd);
		filenumber++;
	}
	_findclose(handle);
		
	//Sort
	printf("\n--------------------------------------------------------------------------------");

	if (mode == 1){
		printf("\n<���� ��Ȯ�� �� ����");
		printf("- Keyword : %s>\n", keyword);
		QuickSort(File, FileContent, 0, filenumber-1);
		PrintFile(File, FileContent, filenumber, 0);
	}else if (mode == 2){
		printf("\n<���� �̸� �� ����");
		printf("- Keyword : %s>\n", keyword);
		PrintFile(File, FileContent, 0, filenumber);
	}else if (mode == 3){
		printf("\n<���� �ֽ� �� ����");
		printf("- Keyword : %s>\n", keyword);
		QuickSort(File, Filetime, FileContent, 0, filenumber-1);
		PrintFile(File, FileContent, filenumber, 0);
	}
	endTime = clock();
	gap = (float)(endTime - startTime) / (CLOCKS_PER_SEC);
	printf("���� �ð� : %f\n", gap);
}

float Fileopen(char *filename, char *keyword){
	FILE *fp;
	char data[MAX];
	float value;

	fp = fopen(filename, "r");

	while (fscanf(fp, "\n%[^^]%s\n", &data) != EOF){
		value = Finddata(data, keyword);
	}

	fclose(fp);
	return value;
}

float Finddata(char *data, char *keyword){
	float value, count;
	int lengthData, lengthWord;
	char *locate, *locate2;
	int i;

	value = 0;
	count = 0;
	lengthData = strlen(data);
	lengthWord = strlen(keyword);

	for (int i = 0; i<lengthWord; i++)
		if (keyword[i] >= 65 && keyword[i] <= 90)
			keyword[i] += 32;
	for (int i = 0; i<lengthData; i++)
		if (data[i] >= 65 && data[i] <= 90)
			data[i] += 32;

	locate = strstr(data, keyword);
	while (locate != NULL)
	{
		if (lengthWord*(value + 1) > lengthData)
			break;
		else
		{
			value++;
			locate = strstr(locate + lengthWord, keyword);
		}
	}
	locate2 = strpbrk(data, keyword);
	while (locate2 != NULL)
	{
		count++;
		locate2 = strpbrk(locate2 + 1, keyword);
	}
		
	return value + (0.9 * (count / lengthData));
}

void Swap(int* a, int* b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void Swap(float* a, float* b){
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

void SwapFile(char *a, char *b){
	char tmp[30];
	strcpy(tmp,	a);
	strcpy(a, b);
	strcpy(b, tmp);
}

void QuickSort(char **File, int* array, float* array2, int start, int end){
	int mid, pivot, p, q;

	if (start >= end)
		return;

	mid = (start + end) / 2;
	pivot = array[mid];

	Swap(&array[start], &array[mid]);
	SwapFile(File[start], File[mid]);
	Swap(&array2[start], &array2[mid]);

	p = start + 1, q = end;

	while (1){
		while (array[p] <= pivot){
			p++;
		}
		while (array[q]>pivot){
			q--;
		}
		if (p>q) break;
		Swap(&array[p], &array[q]);
		SwapFile(File[p], File[q]);
		Swap(&array2[p], &array2[q]);
	}
	Swap(&array[start], &array[q]);
	SwapFile(File[start], File[q]);
	Swap(&array2[start], &array2[q]);

	QuickSort(File, array, array2, start, q - 1);
	QuickSort(File, array, array2, q + 1, end);
}

void QuickSort(char **File, float* array, int start, int end){
	int mid, p, q;
	float pivot;

	if (start >= end)
		return;
	
	mid = (start + end) / 2;
	pivot = array[mid];
	
	Swap(&array[start], &array[mid]);
	SwapFile(File[start], File[mid]);

	p = start + 1, q = end;

	while (1){
		while (array[p] <= pivot){
			p++;
		}
		while (array[q]>pivot){
			q--;
		}
		if (p>q) break;
		Swap(&array[p], &array[q]);
		SwapFile(File[p], File[q]);
	}
	Swap(&array[start], &array[q]);
	SwapFile(File[start], File[q]);

	QuickSort(File, array, start, q - 1);
	QuickSort(File, array, q + 1, end);
}

void PrintFile(char **File, float *FileContent, int start, int end){
	int i;
	if (start < end)
	{
		for (i = start; i < end; i++)
		{
			if (FileContent[i] >= 1)
			{
				printf("\n\t%s", File[i]);
			}
		}
	}
	else
		for (i = start - 1; i >= end; i--)
		{
			if (FileContent[i] >= 1)
			{
				printf("\n\t%s", File[i]);
			}
		}
	printf("\n\n--------------------------------------------------------------------------------\n");
}