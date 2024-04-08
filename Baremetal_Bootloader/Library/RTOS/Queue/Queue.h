/*******************************************************************
 *  FILE DESCRIPTION
--------------------------------------------------------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Queue.h
 *  Module:  Data Structure/Queue
 *  Description:  Queue Implementation
*******************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_
/********************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "RTOS_Scheduler.h"
/*****************************************
----------   Configuration    ------------
*****************************************/
#define Storage_Type          RTOS_TCB_t*
#define Queue_Size            Maximum_Tasks
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/*------------- Type Defs --------------*/
typedef enum queue_error 
{
     Queue_Full                =0,
     Queue_Ok                  =1,
     Queue_Empty               =2,
     Queue_Allocation_Error    =3,
}queue_error;
/*------------ Data Base --------------*/
typedef struct __attribute__((packed))Queue_t 
{
     u8 size;
     s8 front;
     s8 rear;
     Storage_Type elements[Queue_Size];
}Queue_t;
/*----------- Functins To Use ---------*/
queue_error Queue_Dequeue_Node(Queue_t *my_queue,u8 Node_Number);
queue_error Queue_Dequeue(Queue_t *my_queue,Storage_Type *data);
queue_error Queue_Enqueue(Queue_t *my_queue,Storage_Type data);
queue_error Queue_Initialization(Queue_t *my_queue);
queue_error Queue_Clear(Queue_t *my_queue);
s8 Queue_Is_Empty(Queue_t *my_queue);
s8 Queue_Is_Full(Queue_t *my_queue);
#endif
/********************************************************************
 *  END OF FILE: Queue.h
********************************************************************/
