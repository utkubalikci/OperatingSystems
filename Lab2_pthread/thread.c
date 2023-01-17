#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int indexNum = 0;
int count = 0;
double *numbers;
double sum = 0;
int threadId = 1;
sem_t sayac_sem;
int threadReadNum;
int kalan;

void * add(void *args){
	sem_wait(&sayac_sem);
	indexNum++;
	if (indexNum > count){
		sem_post(&sayac_sem);
		return NULL;
	}
	sum += numbers[indexNum-1];
	printf("Thread %d reads %d. line. The value is %.2f \n",threadId,indexNum,numbers[indexNum-1]);
	sem_post(&sayac_sem);
}

void printSum(){
	printf("Shared Resource: %.2f\n",sum);
	threadId ++;
}

void pthreadCreate(pthread_t t){
	if (indexNum >= count)
		return;
	
	int i;
	for (i = 0; i<threadReadNum; i++){
		pthread_create(&t,NULL,add,NULL);
	}
	if (kalan > 0){
		kalan--;
		pthread_create(&t,NULL,add,NULL);
	}
	pthread_join(t,NULL);
	printSum();
}

int findThreadNum(int numThread){
	if (count % numThread == 0)
		return count/numThread;
	else
		return count/numThread + 1;
	
}

void getNumbers()
{
	char * temp;
	char tempChar;
	int tempLen;
	int isEof = 1;
	while(isEof){
		temp = (char *)malloc(sizeof(char));
		tempLen = 0;
		while(1){
			tempChar = getchar();
			if (tempChar == 10) // '\n'
				break;
			if (tempChar == EOF){
				isEof = 0;
				break;
			}
			tempLen++;
			temp = realloc(temp,tempLen*sizeof(char));
			temp[tempLen-1] = tempChar;
		}
		count++;
		numbers = realloc(numbers,count*sizeof(double));
		numbers[count-1] = atof(temp);
		free(temp);
	}
}

int main(){
	int number;
	numbers = (double *)malloc(sizeof(double));
	
	getNumbers();
	
	threadReadNum = count / 5;
	kalan = count - (threadReadNum*5);
	//printf("count = %d th  %d\n",count,threadReadNum);
	pthread_t t1,t2,t3,t4,t5;
	pthread_t threads[5] = {t1,t2,t3,t4,t5};
	
	sem_init(&sayac_sem, 0, 1);
	
	int i;
	for (i=0; i<5; i++){
		pthreadCreate(threads[i]);
	}
	
	return 0;
}
