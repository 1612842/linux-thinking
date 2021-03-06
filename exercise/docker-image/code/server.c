#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <time.h>

#define PORT 8080
#define MAX 1024
#define MAX_NUM 100

struct Client
{
    int sock;
    int active;
    int isOver;
    int isRecvFile;
} * clientInfo;

int cur_clients = 0;
int max_clients = 4;
int *arr;
int sizeOfArr = 5;
int posNum = 0;
pthread_mutex_t lock;
int numOfFileCalcedSum = 0;
int isOver =0;
int isOutOfSlot=0;
struct Rank
{
    int clientId;
    int sum;
}*rankClient;

void *connection_handler(void *);

int main(int argc, char *argv[])
{
    if (argc >= 3){
        sizeOfArr=atoi(argv[2]);
    }
    if (argc >=2){
        max_clients=atoi(argv[1]);
    }

    int socket_descriptor, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    printf("Max clients: %d\n",max_clients);
    randomArr(sizeOfArr);
    printArr();
    initializeClientInfo();
    initializeRankClient();

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    //Create socket
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_descriptor == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Set info socket
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind socket
    if (bind(socket_descriptor, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_descriptor, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while (client_sock = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t *)&c))
    {
        cur_clients++;
        if (cur_clients>max_clients){
            puts("Out of slots!!!");
            isOutOfSlot=1;
        }
        else
            puts("Connection accepted");

        char buff[MAX];
        pthread_t thread_id;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&thread_id, NULL, connection_handler, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        int cid = 0;
        for (int i = 0; i < max_clients; i++)
        {
            if (clientInfo[i].active == 0)
            { 
                clientInfo[i].active = 1;
                clientInfo[i].sock = *(int *)new_sock;
                cid = i;
                break;
            }
        }

        if (isOutOfSlot==0)
        {
            bzero(buff, sizeof(buff));
            sprintf(buff, "%d", cid);
            write(clientInfo[cid].sock, buff, sizeof(buff));
            printf("Client %s is assigned\n", buff);
        }
        else
        {
            char tmp[MAX];
            bzero(tmp,MAX);
            strcpy(tmp,"Out of slots!!!");
            
            write(*(int *)new_sock, tmp, sizeof(tmp));
        }
        
        
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    close(client_sock);
    pthread_mutex_destroy(&lock);
    return 0;
}

void *connection_handler(void *socket_descriptor)
{
    //Get the socket descriptor
    int sock = *(int *)socket_descriptor;
    char buff[MAX];
    int n;

    int idClient = getIdClient(sock);


    while (1)
    {
        
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        int index = read(sock, buff, sizeof(buff));

	if (strcmp(buff, "post\n") == 0)
        {
                clientInfo[idClient].isOver = 1;
        }
        
        if (index == 0)
        {
            if (cur_clients>max_clients){
                cur_clients--;
                
                if (cur_clients<=max_clients)
                    isOutOfSlot=0;
                break;                
            }
            else
            {   
                clientInfo[idClient].active = 0;
                printf("Client %d quit\n", idClient);
                cur_clients--;
                break;
            }
        }

	

        if (clientInfo[idClient].isOver == 1)
        {

	    if (strcmp(buff, "post\n")==0)
            	{
	     		char tmp[MAX];
            		strcpy(tmp, "server waits file");
            		write(sock, tmp, sizeof(buff));
  		}
		else
		{
            char filename[MAX];
            sprintf(filename, "server-%d.txt", idClient);
            FILE *f = fopen(filename, "w");
            fprintf(f, "%s ", buff);
            
            fflush(f);
            fclose(f);
            char tmp[MAX];
            strcpy(tmp, "server has received file");
            write(sock, tmp, sizeof(buff));
            clientInfo[idClient].isRecvFile=1;
		}
            
        }else if (strcmp(buff, "get\n")!=0&&strcmp(buff, "post\n")!=0){
                strcpy(buff, "?");
                write(sock, buff, sizeof(buff));
        }

        // print buffer which contains the client contents
        printf("\nFrom client %d: %s",idClient, buff);

        
        if (checkAllFileReceived()==1){
            calculateSum();
            rankingAllSum();

            for (int i = 0; i < numOfFileCalcedSum; i++)
            {
                printf("\n%d: %d \n",rankClient[i].clientId, rankClient[i].sum);
            }

            char content[MAX]="";
            buildStringOfRank(content);
            printf("%s\n", content);

            
            for (int i = 0; i < max_clients; i++)
            {
                if (clientInfo[i].active==1){
                    write(clientInfo[i].sock, content, sizeof(content));
                }
            }
            

            printf("all file recv...\n");
            
            
        }

        if (strcmp(buff, "get\n") == 0)
            {
                pthread_mutex_lock(&lock);

                bzero(buff, MAX);
                int num;
                if (posNum == sizeOfArr)
                {
                    isOver=1;
                    strcpy(buff, "over");
                    write(sock, buff, sizeof(buff));
                }
                else
                {
                    num = arr[posNum];
                    sprintf(buff, "%d", num);
                    write(sock, buff, sizeof(buff));
                    posNum++;
                }

                
                pthread_mutex_unlock(&lock);
            }

        
           
	if (strcmp(buff, "post\n") == 0)
        {
                clientInfo[idClient].isOver = 1;
                printf("post....\n");
        }
        
        
    }
    
    close(sock);

    return 0;
}

void printArr()
{
    for (int i = 0; i < sizeOfArr; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void randomArr(int size)
{
    arr = (int *)malloc(size * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % MAX_NUM;
    }
}

void initializeClientInfo()
{
    clientInfo = malloc(max_clients * sizeof(struct Client));
    for (int i = 0; i < max_clients; i++)
    {
        clientInfo[i].active = 0;
        clientInfo[i].sock = 0;
        clientInfo[i].isOver = 0;
        clientInfo[i].isRecvFile = 0;
    }
}

void initializeRankClient(){
    rankClient = malloc(max_clients * sizeof(struct Rank));
    for (int i = 0; i < max_clients; i++)
    {
        rankClient[i].clientId=0;
        rankClient[i].sum=0;
    }
}

int getIdClient(int sock)
{
    for (int i = 0; i < max_clients; i++)
    {
        if (clientInfo[i].sock == sock)
        {
            return i;
        }
    }
    return 0;
}

int checkAllFileReceived(){
    int numRecvFile=0, numActive=0;
    for (int i = 0; i < max_clients; i++)
    {
        if (clientInfo[i].isRecvFile==1)
            numRecvFile++;
        if (clientInfo[i].active==1)
            numActive++;
    }

    if (numRecvFile==numActive)
        return 1;
    else
    {
        return 0;
    }
}



void calculateSum(){
    FILE* f;
    char filename[MAX];
    
    for (int i = 0; i < max_clients; i++)
    {
        if (clientInfo[i].active==1){
            
            sprintf(filename, "server-%d.txt", i);
            f=fopen(filename,"r");
            
            int tmp;
            while (1) {
                
                int ret = fscanf(f, "%d", &tmp);
                if (ret==EOF)
                    break;
                rankClient[numOfFileCalcedSum].sum+=tmp;
            }
            fclose(f);
            rankClient[numOfFileCalcedSum].clientId=i;
            numOfFileCalcedSum++;
        }
    }
    
    for (int i = 0; i < numOfFileCalcedSum; i++)
    {
        printf("\n%d:- %d \n----",rankClient[i].clientId, rankClient[i].sum);
    }
    
}


void rankingAllSum(){
    for(int i = 0; i < numOfFileCalcedSum-1; i++)
    {
        for (int j = i+1; j < numOfFileCalcedSum; j++)
        {
            if (rankClient[i].sum<rankClient[j].sum){
                struct Rank temp = rankClient[i];
                rankClient[i]=rankClient[j];
                rankClient[j]=temp;
            }
        }
        
        
    }
    
}

void buildStringOfRank(char* content){
    
    for (int i = 0; i < numOfFileCalcedSum; i++)
    {
        char tmp[MAX];
        sprintf(tmp, "Rank: %d - Client: %d - Sum: %d\n",i+1, rankClient[i].clientId,rankClient[i].sum);
        strcat(content,tmp);
    }

}
