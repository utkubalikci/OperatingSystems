#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<wait.h>

int main(){
	int segment_id;

	int * shmPtr;

	const int size = 4 * sizeof(char);

	segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
	
	shmPtr = (int *) shmat (segment_id, NULL, 0);
	
	printf("Server has received a shared memory of four integers...\n");
	
	printf("Server has attached the shared memory...\n");

	for (int i=0;i<4;i++){
		scanf("%d",&shmPtr[i]);
	}
		
	printf("Server has filled %d %d %d %d in shared memory...\n",shmPtr[0],shmPtr[1],shmPtr[2],shmPtr[3]);
	
	printf("Server is about to fork a child process...\n");


	fflush(stdout);
	int pid;
	
	pid = fork();

	if(pid==0){
		printf("   Client process started\n");
		
		printf("   Client found %d %d %d %d in shared memory\n",shmPtr[0],shmPtr[1],shmPtr[2],shmPtr[3]);
		
		int i;
		int sum = 0;
		for (i=0;i<4;i++){
			sum += shmPtr[i];
		}
		shmPtr[0] = sum;
		
		printf("   Client sum of shared integer values %d \n",shmPtr[0]);
		
		printf("   Client is about to exit\n");
		
		return 0;
	}
	
	printf("Server has detected the completion of its child...\n");

	if(pid>0){
		wait(NULL);
		
		printf("Parent sum of shared memory %d \n",shmPtr[0]);
	}
	
	shmdt(shmPtr);
	printf("Server has detached its shared memory...\n");
	
	shmctl(segment_id, IPC_RMID, NULL);
	printf("Server has removed its shared memory...\n");
	
	printf("Server exits...\n"); 

	return 0;
}
