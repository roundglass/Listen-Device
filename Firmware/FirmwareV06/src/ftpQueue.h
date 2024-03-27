#ifndef FTPQUEUE_H_
#define FTPQUEUE_H_

#define QUEUE_SIZE 10

#include <WProgram.h>

class ftpQueue{
public:

ftpQueue();
void enqueue(String);
String dequeue();
void show();
int isFull();
int isempty();


private:

String data[QUEUE_SIZE];
int Front = -1;
int Rear = -1;


};




#endif