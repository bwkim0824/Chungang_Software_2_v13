#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <io.h>
#include <string>
#include <time.h>
#define MAX 9999
#define MAX_NUM 1000

//파일을 정확도, 최신순, 이름순으로 모드를 입력받아 정렬하는 프로그램

//파일 내용 읽는 함수
float Fileopen(char *filename, char *keyword);
//내용의 정확성을 판별하는 함수
float Finddata(char *data, char *keyword);
//두 문자 Swap 함수, 오버로딩
void Swap(int* a, int* b);
void Swap(float* a, float* b);
void SwapFile(char *a, char *b);
//QuickSort 함수, 오버로딩
void QuickSort(char **File, int* array, float* array2, int start, int end);
void QuickSort(char **File, float* array, int start, int end);
//결과값 출력 함수
void PrintFile(char **File, int start, int end);
void PrintFile(char **File, float *FileContent, int start, int end);

void main()
{
	//변수 선언
	char **File;             //함수이름 저장하는 포인터
	int filenumber = 0;      //총 파일 개수를 저장
	int *FileSize;           //파일 크기를 저장하는 포인터
	int *Filetime;           //파일 최근 수정날짜를 저장하는 포인터
	float *FileContent;      //파일 정확성 정보를 저장하는 포인터
	float gap;
	
	struct _finddata_t fd;
	long handle;
	int result = 1, mode;
	char keyword[30];
	time_t startTime = 0, endTime = 0;
	
	printf("\n****SW programming Project 1\n");
	printf("\n****파일 서치 프로그램\n");
	printf("\n--------------------------------------------------------------------------------");
	
	File = (char**)malloc(sizeof(char) * MAX_NUM);
	FileSize = (int*)malloc(sizeof(int) * MAX_NUM);
	FileContent = (float*)malloc(sizeof(float) * MAX_NUM);
	Filetime = (int*)malloc(sizeof(int) * MAX_NUM);

	printf("\n****찾을 키워드를 설정해 주세요\n");
	printf("\t>>");
	scanf("%s", keyword);

	printf("\n****검색 옵션을 설정해 주세요");
	printf("\n\t>>정확성 : 1 \n\t>>이름순 : 2 \n\t>>최신순 : 3\n");
	scanf("\t%d", &mode);

	startTime = clock();
	//파일 서치
	handle = _findfirst(".\\*.txt", &fd);
	if (handle == -1)
		return;	

	while (result != -1)
	{
		//2차원 동적 할당
		File[filenumber] = (char*)malloc(sizeof(char) * fd.size);
		
		//파일 시간정보 가져오기
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
		printf("\n<파일 정확성 순 정렬");
		printf("- Keyword : %s>\n", keyword);
		QuickSort(File, FileContent, 0, filenumber-1);
		PrintFile(File, FileContent, filenumber, 0);
	}else if (mode == 2){
		printf("\n<파일 이름 순 정렬");
		printf("- Keyword : %s>\n", keyword);
		PrintFile(File, FileContent, 0, filenumber);
	}else if (mode == 3){
		printf("\n<파일 최신 순 정렬");
		printf("- Keyword : %s>\n", keyword);
		QuickSort(File, Filetime, FileContent, 0, filenumber-1);
		PrintFile(File, FileContent, filenumber, 0);
	}
	endTime = clock();
	gap = (float)(endTime - startTime) / (CLOCKS_PER_SEC);
	printf("측정 시간 : %f\n", gap);
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