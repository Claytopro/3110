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
    /*
    state describes what the process is doing
    1: running
    2: ready
    3: blocked
    */
    int state;

    int rdyStart;
    int rdyEnd;
    int rdyTotal;

    int blckStart;
    int blckEnd;
    int blckTotal;

    int runStart;
    int runEnd;
    int runTotal;

    struct processStruct* previous;
    struct processStruct* next;
} Node;


typedef struct queueHead{
    Node* head;
    Node* tail;
    int length;

} qHead;

qHead* initializeHead();
Node* initlizeNode(int pID);
void addToQueue(qHead *front,Node*toadd);
Node* removeFromQueue(qHead *front,int findID);
Node* removeFromHead(qHead *front);
void printQueue(qHead *front);
void deleteQueue(qHead *front);


int main(){
  FILE *fp;
  char buffer[80];
  char eventType ;
  char *temp = NULL;
  int time = 0;
  int processID=0;

  qHead *readyQueue = initializeHead();
  //only need on queue because there will never be an out of line resource request for something not running
  qHead *resourceQueue = initializeHead();
  qHead *exitQueue = initializeHead();

  Node *tmpProcess;
  Node *system = initlizeNode(0);
  system->runStart = 0;


  fp=fopen("test4.in","r");

  while(fgets(buffer, 80,fp) ){
    printf("buf:%s",buffer );
    if(strcmp(buffer,"\n")==0)break;

    temp = strtok(buffer," ");
    time = atoi(temp);


    //  printf("%s\n", temp);
      temp = strtok(NULL," ");
      eventType = temp[0];

      switch (eventType) {
        case 'C':
          temp = strtok(NULL," ");
          processID = atoi(temp);
          tmpProcess = initlizeNode(processID);
          addToQueue(readyQueue,tmpProcess);

          //if the new process is first one then it begins running right away
          if(readyQueue->length == 1){
            tmpProcess->runStart = time;
            tmpProcess->state =1;
          }else{
            tmpProcess->rdyStart = time;
            tmpProcess->state =2;
          }

        break;
        case 'E':
          temp = strtok(NULL," ");
          processID = atoi(temp);
          //search through all queues to find
          printQueue(readyQueue);

          tmpProcess = removeFromQueue(readyQueue,processID);
          //if tmp process was not in ready queue,
          if(tmpProcess == NULL){
            tmpProcess = removeFromQueue(resourceQueue,processID);
            tmpProcess->blckEnd = time;
            tmpProcess->blckTotal += (tmpProcess->blckEnd - tmpProcess->blckStart);
          } else{
            //prcess was running
            if(tmpProcess->state == 1){
              tmpProcess->runEnd =time;
              tmpProcess->runTotal += (tmpProcess->runEnd - tmpProcess->runStart);
              addToQueue(exitQueue,tmpProcess);

              //deals with new head time allocation calculations
              if(readyQueue->head != NULL){
                tmpProcess = readyQueue->head;
                tmpProcess->rdyEnd = time;
                tmpProcess->rdyTotal += (tmpProcess->rdyEnd - tmpProcess->rdyStart);
                tmpProcess->runStart = time;
                tmpProcess->state =1;
              }

            }else{
              //process was ready
              tmpProcess->rdyEnd =time;
              tmpProcess->rdyTotal += (tmpProcess->rdyEnd - tmpProcess->rdyStart);
              addToQueue(exitQueue,tmpProcess);

            }

          }

        break;
        case 'R':

        break;
        case 'I':

        break;
        case 'T':
          printf("Queue is currenlt :\n" );
          printQueue(readyQueue);
          //if queue is not 1 then remove from and and add to end of queue
          if(readyQueue->length !=1){
            tmpProcess = removeFromHead(readyQueue);
            tmpProcess->runEnd =time;
            tmpProcess->runTotal += (tmpProcess->runEnd - tmpProcess->runStart);
            tmpProcess->rdyStart = time;
            tmpProcess->state =2;
            addToQueue(readyQueue,tmpProcess);

            tmpProcess = readyQueue->head;
            tmpProcess->rdyEnd =time;
            tmpProcess->rdyTotal += (tmpProcess->rdyEnd - tmpProcess->rdyStart);
            tmpProcess->runStart = time;
            tmpProcess->state =1;

          }

          printf("Queue became :\n" );
          printQueue(readyQueue);
        break;
        default:
          printf("invalid file, event not recognized\n" );
      }

  }

 printQueue(exitQueue);

  deleteQueue(readyQueue);
  deleteQueue(resourceQueue);
  deleteQueue(exitQueue);
  free(system);

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
Node* initlizeNode(int pID){
  Node*temp = calloc(1,sizeof(Node));
  temp->next = NULL;
  temp->previous = NULL;
  temp->processID= pID;


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
      toadd->previous = NULL;
      front->tail->previous = toadd;
      front->tail = toadd;

    }

    front->length = front->length +1;
}

Node* removeFromHead(qHead *front){
    Node*temp = front->head;
    front->head = temp->previous;
    front->head->next = NULL;

    temp->next = NULL;
    temp->previous = NULL;

    front->length = front->length -1;

    return temp;
}

void printQueue(qHead *front){

  Node *temp = front->tail;
  printf("QUEUE:\n" );
  while(temp != NULL){
    printf("process ID :%d ,run Time: %d, Block time: %d, ready time %d\n",temp->processID,temp->runTotal,temp->blckTotal,temp->rdyTotal);
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


//used as for exit call
Node* removeFromQueue(qHead *front,int findID){

  Node *temp = front->tail;



  if(temp->processID == findID){
    if(front->length ==1){

      front->head = NULL;
      front->tail = NULL;
      temp->next = NULL;
      temp->previous = NULL;
      return temp;
    }

    front->tail = temp->next;
    temp->next->previous = NULL;
    front->length--;

    temp->next = NULL;
    temp->previous = NULL;
    return temp;
  }

  while(temp != NULL){

    if(temp->processID == findID){

        if(temp == front->head){
          front->head = temp->previous;
          temp->previous->next = NULL;
          front->length--;

          temp->next = NULL;
          temp->previous = NULL;
          return temp;
        }

        temp->previous->next = temp->next;
        temp->next->previous = temp->previous;
        front->length--;

        temp->next = NULL;
        temp->previous = NULL;
        return temp;
    }

    temp = temp->next;
  }

  return NULL;
}
