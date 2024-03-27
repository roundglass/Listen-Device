#include "ftpQueue.h"

ftpQueue::ftpQueue(){ }

void ftpQueue::enqueue(String filename){

    if(isFull())
    { }
    else{
        if (Front == - 1){
            Front = 0;}

        Rear = Rear + 1;
        data[Rear] = filename;
    }

}

int ftpQueue::isFull()
{
    if (Rear == QUEUE_SIZE - 1){
        return 1;
    }
    else{
        return 0;
    }
}

String ftpQueue::dequeue(){
    
    if (Front == - 1 || Front > Rear)
    {
        Serial.print("Underflow \n");
        return ;
    }
    else{
        String filename = data[Front];
        Front = Front+1;
        return filename;
    }
}

void ftpQueue::show(){
    if (Front == - 1)
        Serial.print("Empty Queue \n\r");
    else
    {
        Serial.print("Queue: \n\r");
        for (int i = Front; i <= Rear; i++)
            Serial.printf("%s ", data[i]);
        Serial.print("\n\r");
    }
}

int ftpQueue::isempty(){
    if(Front == - 1)
    {
        Serial.print("Empty Queue \n\r");
        return 1;
    }
    else{
        return 0;
    }
}