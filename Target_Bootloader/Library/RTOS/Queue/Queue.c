/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Queue.c
 *  Module:  Data Structure/Queue
 *  Description:  Queue Implementation
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Queue.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
* Syntax          : queue_error Queue_Dequeue_Node(Queue_t *my_queue,u8 Node_Number)
* Description     : Delete Specific Node In Queue
* Parameters (in) : (Ptr To Queue) (Node Number)
* Parameters (out): None
* Return value:   : queue_error
********************************************************************/
queue_error Queue_Dequeue_Node(Queue_t *my_queue,u8 Node_Number)
{
     queue_error flag=Queue_Ok;
     if((my_queue->size))
     {
          for(u8 Counter=Node_Number-ONE;Counter<my_queue->size;Counter++)
          {
               my_queue->elements[Counter]=my_queue->elements[Counter+1];
          }
          my_queue->size--;
     }
     else flag=Queue_Empty;
     return flag;
}
/********************************************************************
* Syntax          : queue_error Queue_Enqueue(Queue_t *my_queue,Storage_Type data)
* Description     : Add Elements To Queue
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To Queue) (Copy Of Data)
* Parameters (out): None
* Return value:   : queue_error
********************************************************************/
queue_error Queue_Enqueue(Queue_t *my_queue,Storage_Type data)
{
     queue_error flag=Queue_Ok;
     if((my_queue->size)<Queue_Size)
     {
          my_queue->rear=(my_queue->rear+ONE)%(s8)Queue_Size;
          my_queue->elements[my_queue->rear]=data;
          my_queue->size++;
     }
     else flag=Queue_Full;
     return flag;
}
/********************************************************************
* Syntax          : queue_error Queue_Dequeue(Queue_t *my_queue,Storage_Type *data)
* Description     : Get An Element From Queue
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To Queue) (Ptr To Storage Data)
* Parameters (out): None
* Return value:   : queue_error
********************************************************************/
queue_error Queue_Dequeue(Queue_t *my_queue,Storage_Type *data)
{
     queue_error flag=Queue_Ok;
     if(my_queue->size)
     {
          *data=my_queue->elements[my_queue->front];
          my_queue->front=((my_queue->front+ONE)%(s8)Queue_Size);
          my_queue->size--;
     }
     else flag=Queue_Empty;
     return flag;
}
/********************************************************************
* Syntax          : queue_error Queue_Initialization(Queue_t *my_queue)
* Description     : Initialize Queue
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To Queue)
* Parameters (out): None
* Return value:   : queue_error
********************************************************************/
queue_error Queue_Initialization(Queue_t *my_queue)
{
     queue_error flag=Queue_Ok;
     my_queue->size=ZERO;
     my_queue->front=ZERO;
     my_queue->rear=-ONE;
     return flag;
}
/********************************************************************
* Syntax          : s8 Queue_Is_Empty(Queue_t *my_queue)
* Description     : Check If Queue Is Empty
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To Queue)
* Parameters (out): None
* Return value:   : s8 (True) (False)
********************************************************************/
s8 Queue_Is_Empty(Queue_t *my_queue)
{
     return !(my_queue->size);
}
/********************************************************************
* Syntax          : queue_error Queue_Is_Full(Queue_t *my_queue)
* Description     : Check If Queue Is Full
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To Queue)
* Parameters (out): None
* Return value:   : s8 (True) (False)
********************************************************************/
s8 Queue_Is_Full(Queue_t *my_queue)
{
     return (my_queue->size==Queue_Size);
}
/********************************************************************
 *  END OF FILE: Queue.c
********************************************************************/
