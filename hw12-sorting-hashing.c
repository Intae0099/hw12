/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);
int freeArray(int *a);
void printArray(int *a);

int selectionSort(int *a);
int insertionSort(int *a);
int bubbleSort(int *a);
int shellSort(int *a);
/* recursive function으로 구현 */
int quickSort(int *a, int n);


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable);
			printArray(hashtable);
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);
			printArray(hashtable);
			index = search(hashtable, key);
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a) // 배열 동적할당 해제
{
	if(a != NULL)
		free(a);
	return 0;
}

void printArray(int *a) //배열 출력
{
	if (a == NULL) { //배열이 비어있을 경우
		printf("nothing to print.\n"); //비었다고 출력
		return; //함수 종료
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //배열의 위치 출력
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //배열의 값 출력
		printf("%5d ", a[i]);
	printf("\n");
}

/*
선택정렬
배열을 탐색해 가장 작은값을 맨 앞으로 옮기고, 그 다음으로 작은 값을 2번째로 옮기는 식으로
자리교환을 하는 정렬
*/

int selectionSort(int *a) 
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬 전의 배열 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i; 
		min = a[i];
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)
		{
			if (min > a[j]) //a[i]값이 a[j]값보다 크면
			{
				min = a[j]; //최솟값을 a[j] 로 바꾸고
				minindex = j; //minindex를 j로 설정
			}
		}
		a[minindex] = a[i]; //j번째 값에 a[i]값을 넣고
		a[i] = min; //a[i]값에 min값을 넣는다
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬된 후의 배열 출력
	return 0; //0리턴함으로서 함수 종료
}

/*
삽입 정렬
배열을 정렬 된부분, 정렬 안된부분을 집합으로 나누어
정렬 안된 부분집합의 맨 앞 원소와 정렬된 부분집합 내에서 크기 비교를 한 후,
정렬 된 부분의 위치에 삽입하는 정렬
*/

int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬 전의 배열 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i]; //t값을 a[i]값으로 설정
		j = i; //j값을 i값으로 설정
		while (a[j-1] > t && j > 0) //a[j-1]값이 t보다 클경우 그리고 j가 0보다 클경우 반복문 실행
		{
			a[j] = a[j-1]; //a[j]값을 a[j-1]값으로 설정
			j--; //j--
		}
		a[j] = t; //a[j]값을 t값으로 설정
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬된 후의 배열 출력

	return 0; //0리턴함으로서 함수 종료
}

/*
버블정렬
배열의 앞 뒤값을 비교하여 값이 더 큰경우 뒤에 순서로 보내
최댓값을 가장 뒤로 보내는 정렬방식
*/

int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬 전의 배열 출력

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j]) //a[j-1]의 값이 a[j]값보다 큰 경우
			{
				t = a[j-1]; //t 값을 a[j-1]값으로 설정
				a[j-1] = a[j]; //a[j-1]값을 a[j]값으로 설정
				a[j] = t; //a[j]값을 t값으로 설정
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬된 후의 배열 출력

	return 0; //0을 리턴함으로서 함수 종료
}

/*
셸 정렬

*/
int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬 전의 배열 출력

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) //h의 값은 MAX_ARRAY_SIZE/2부터 시작하고, 2씩 나누며, h>0일때만 반복문 실행
	{
		for (i = 0; i < h; i++) //i=0, i가 h보다 작을때, i++만큼 증가하며 반복문 실행
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h) //j가 i+h값부터 시작하고, 
			{
				v = a[j]; //v의 값을 a[j]값으로 설정
				k = j; //k의 값을 j로 설정
				while (k > h-1 && a[k-h] > v) //k값이 h-1보다 크고, a[k-h]값이 v보다 클경우 반복문 실행
				{
					a[k] = a[k-h]; //a[k]값을 a[k-h]값으로 설정
					k -= h; //k의 값을 k-h값으로 설정
				}
				a[k] = v; //a[k]의 값을 v로 변경
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬된 후의 배열 출력

	return 0; //0을 리턴함으로서 함수 종료
}
/*
퀵 정렬
기준값을 중심으로 왼쪽과 오른쪽 부분집합으로 나누어 정렬하는 방식이다
피봇값을 설정해 기준으로 잡아 피봇보다 작으면 왼쪽, 피봇보다 크면 오른쪽으로 정렬
*/
int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1) //n이 1보다 크면 즉 배열의 크기가 1보다 크면
	{
		v = a[n-1]; //v의 값을 a[n-1]의 값으로 설정 즉 피봇값을 설정
		i = -1; //i의 값을 -1로 설정
		j = n - 1; //j의 값을 n-1로 설정

		while(1) //항상 반복
		{
			while(a[++i] < v); //a[++i]값이 v보다 작을때까지 i값 증가
			while(a[--j] > v); //a[--j]값이 v보다 클때 까지 j값 감소

			if (i >= j) break; //i가 j보다 크거나 같으면 반복문 종료
			t = a[i]; //t값을 a[i]값으로 설정
			a[i] = a[j]; //a[i]값을 a[j]값으로 설정
			a[j] = t; //a[j]값을 t값으로 설정
		}
		t = a[i]; //t값을 a[i]값으로 설정
		a[i] = a[n-1]; //a[i]값을 a[n-1]값으로 설정
		a[n-1] = t; //a[n-1]값을 t값으로 설정

		quickSort(a, i); //재귀함수를 불러 함수 인자를 a, i를 갖는다
		quickSort(a+i+1, n-i-1); //재귀함수를 불러 함수 인자로 a+i+1, n-i-1을 갖는다
	}


	return 0; //0을 리턴함으로서 함수 종료
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE; //key와 MAX_HASH_TABLE_SIZE을 나눈 나머지 값을 반환
}

int hashing(int *a, int **ht) //해싱
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1; //해싱테이블의 모든 배열을 -1로 초기화한다

	
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i]; //k값을 a[i]값으로 설정한다
		hashcode = hashCode(key); //hashcode값을 hashCode(key)함수를 호출하여 값을 설정한다
		
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		
		if (hashtable[hashcode] == -1) //hashtable[hashcode] 가 -1일 경우
		{
			hashtable[hashcode] = key; //hashtable[hashcode]가 비었으므로 key값을 넣는다
		} else /* 그렇지 않는 경우*/	{

			index = hashcode; //index값을 hashcode값으로 설정한다

			while(hashtable[index] != -1) //hashtable[index]가 -1이 아닐때 즉 비어있지 않을때 반복문 실행
			{
				index = (++index) % MAX_HASH_TABLE_SIZE; //index값을 1 늘리고 MAX_HASH_TABLE_SIZE와 나눈 나머지로 설정
				
				printf("index = %d\n", index);
				
			}
			hashtable[index] = key; //hashtable[index]값을 key값으로 설정한다
		}
	}

	return 0; //0을 리턴함으로서 함수 종료
}

int search(int *ht, int key)
{
	int index = hashCode(key); //index값을 hashCode(key)함수를 호출해 나온 값으로 설정

	if(ht[index] == key) //ht[index]가 key인 경우
		return index; //index를 반환하고 함수 종료

	while(ht[++index] != key) //ht[++index]가 key가 아닌경우 반복문 실행
	{
		index = index % MAX_HASH_TABLE_SIZE; //index 값을 MAX_HASH_TABLE_SIZE로 나눈 나머지 값으로 설정
	}
	return index; //index를 반환하고 함수 종료
}



