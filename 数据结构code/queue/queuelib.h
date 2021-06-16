/*
提供队列接口
*/

#ifndef __QUEUE_H_
#define __QUEUE_H_

enum QUEUE_TYPE
{
    QUEUE_ARRAY = 60,
    QUEUE_LIST,
};
enum QUEUE_STATE
{
    QUEUE_OK = 0,
    QUEUE_ERR
};

typedef struct QUEUE_UNIT
{
    void *data;
    struct QUEUE_UNIT *next;
}queue_unit;

typedef struct QUEUE
{
    queue_unit *queue_list;
    queue_unit *queuelist_tail;
    enum QUEUE_TYPE mode;
    void *queue_array;
    int queuearray_head;
    int queuearray_tail;
    int queue_size;
    int unit_size;
}Queue;

//初始化队列
enum QUEUE_STATE queue_init(Queue *queue,int queue_size,int unit_size,enum QUEUE_TYPE);

//入队
enum QUEUE_STATE queue_push(Queue *queue,void *node);

//出队
enum QUEUE_STATE queue_pop(Queue *queue,void *node);

//清空队列
enum QUEUE_STATE queue_empty(Queue *queue);

//销毁队列
enum QUEUE_STATE queue_destroy(Queue *queue);

//修改队头
enum QUEUE_STATE queue_headchange(Queue *queue,void *data);

//修改队尾
enum QUEUE_STATE queue_tailchange(Queue *queue,void *data);

#endif