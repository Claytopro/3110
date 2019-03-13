/*
By:Clayton Provan
Assignement 3

*/


#include "stdio.h"
#include "sys/timeb.h"
#include <stdlib.h>
#include <string.h>


typedef struct processStruct{
    int processID;
    int startTime;
    int exitTime;
    int blockTime;
    int waitTime;

    struct processStruct* previous;
    struct processStruct* next;
} Node;


typedef struct queueHead{
    Node* head;
    Node* tail;
    int length;

} qHead;

qHead* initializeHead();
Node* initlizeNode(int pID,int sTime);
void addToQueue(qHead *front,Node*toadd);
Node* removeFromHead(qHead *front);
void printQueue(qHead *front);
void deleteQueue(qHead *front);


int main(){
  FILE *fp;
  char buffer[80];
  char *temp;

  //qHead *processQueue = initializeHead();

  fp=fopen("test0.in","r");

  while(fgets(buffer, 80,fp)){
    //printf("%s\n",buffer );
    temp = strtok(buffer," ");
    while(temp != NULL){
    //  printf("%s\n", temp);
      temp = strtok(NULL," ");
    }
  }

//  deleteQueue(processQueue);

  fclose(fp);
  return 1;
}

/*
create head of queue
*/
qHead* initializeHead(){
    qHead *temp = malloc(sizeof(qHead));
    temp->head = NULL;
    temp->tail = NULL;
    temp->length =0;

return temp;
}

/*
initlize Node for queue manipulation
*/
Node* initlizeNode(int pID,int sTime){
  Node*temp = malloc(sizeof(Node));
  temp->next = NULL;
  temp->previous = NULL;
  temp->processID = pID;
  temp->startTime = sTime;

  return temp;
}

void addToQueue(qHead *front,Node*toadd){

    if(front ==NULL || toadd ==NULL){
      printf("passed in null to add queue\n");
      return;
    }
    if(front->head == NULL){
      front->head = toadd;
      front->tail =toadd;
    }else{
      toadd->next = front->tail;
      front->tail->previous = toadd;
      front->tail = toadd;
    }

    front->length = front->length +1;
}

Node* removeFromHead(qHead *front){
    Node*temp = front->head;
    front->head = temp->previous;

    front->length = front->length -1;

    return temp;
}

void printQueue(qHead *front){

  Node *temp = front->tail;
  printf("QUEUE:\n" );
  while(temp != NULL){
    printf("process ID :%d ,startTime: %d\n",temp->processID,temp->startTime);
    temp = temp->next;
  }

}

void deleteQueue(qHead *front){
  Node *temp1;
  Node *temp2;

  if(front == NULL) return;

  temp1 = front->tail;
  while(temp1 !=NULL){

    temp2 = temp1->next;
    printf("freeing process ID %d\n",temp1->processID);
    free(temp1);
    temp1 =temp2;
  }

  free(front);

}
