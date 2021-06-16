#include "queuelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_INDEX(queue,index) (queue->queue_array + queue->unit_size * index)

static enum QUEUE_STATE queueArray_init(Queue *queue,int queue_size,int unit_size)
{
    queue->queue_array = (void *)malloc((queue_size + 1) * unit_size);
    if(queue->queue_array == NULL)
    {
        printf("空间不足\n");
        return QUEUE_ERR;
    }
    queue->queuearray_head = 0;
    queue->queuearray_tail = 0;
    queue->queue_size = queue_size + 1;
    queue->unit_size = unit_size;
    return QUEUE_OK;
}

static enum QUEUE_STATE queueArray_push(Queue *queue,void *node)
{
    if (!queue || !queue->queue_array || (queue->queuearray_tail + 1) % (queue->queue_size) == queue->queuearray_head)
    {
        printf("队列空间不足\n");
        return QUEUE_ERR;
    }
    memcpy(QUEUE_INDEX(queue,queue->queuearray_tail),node,queue->unit_size);
    queue->queuearray_tail = (queue->queuearray_tail + 1) % (queue->queue_size);
    return QUEUE_OK;
}

static enum QUEUE_STATE queueArray_pop(Queue *queue,void *node)
{
    if(!queue || !queue->queue_array || (queue->queuearray_head == queue->queuearray_tail))
    {
        printf("队列为空\n");
        return QUEUE_OK;
    }
    memcpy(node,QUEUE_INDEX(queue,queue->queuearray_head),queue->unit_size);
    queue->queuearray_head = (queue->queuearray_head + 1) % (queue->queue_size);
    return QUEUE_OK;
}

static enum QUEUE_STATE queueArray_empty(Queue *queue)
{
    queue->queuearray_head = 0;
    queue->queuearray_tail = 0;
    return QUEUE_OK;
}

static enum QUEUE_STATE queueArray_destroy(Queue *queue)
{
    queueArray_empty(queue);
    free(queue->queue_array);
    queue->queue_array = NULL;
    return QUEUE_OK;
}

static enum QUEUE_STATE queueList_init(Queue *queue,int unit_size)
{
    queue->queue_list = (void *)malloc(sizeof(queue_unit));
    if(queue->queue_list == NULL)
    {
        printf("空间不足\n");
        return QUEUE_ERR;
    }
    queue->queuelist_tail = queue->queue_list;
    queue->queue_list->next = NULL;
    queue->unit_size = unit_size;
    return QUEUE_OK;
}

static enum QUEUE_STATE queueList_push(Queue *queue,void *node)
{
    if (!queue || !queue->queue_list)
    {
        printf("输入错误\n");
        return QUEUE_ERR;
    }
    queue_unit *tmp = (queue_unit *)malloc(sizeof(queue_unit));
    tmp->next = NULL;
    if(tmp == NULL || (tmp->data = (void *)malloc(queue->unit_size)) == NULL)
    {
        printf("空间不足\n");
        return QUEUE_ERR;
    }
    queue->queuelist_tail->next = tmp;
    memcpy(queue->queuelist_tail->next->data,node,queue->unit_size);
    queue->queuelist_tail = tmp;
    return QUEUE_OK;
}

static enum QUEUE_STATE queueList_pop(Queue *queue,void *node)
{
    if(!queue || !queue->queue_list || !queue->queue_list->next)
    {
        printf("队列为空\n");
        return QUEUE_ERR;
    }
    queue->queuelist_tail = (queue->queue_list->next == queue->queuelist_tail)?queue->queue_list:queue->queuelist_tail;
    queue_unit *tmp = queue->queue_list->next;
    memcpy(node,queue->queue_list->next->data,queue->unit_size);
    queue->queue_list->next = queue->queue_list->next->next;
    free(tmp->data);
    free(tmp);
    return QUEUE_OK;
}

static enum QUEUE_STATE queueList_empty(Queue *queue)
{
    queue_unit *tmp = NULL;
    while(queue->queue_list != NULL && queue->queue_list->next != NULL)
    {
        tmp = queue->queue_list->next;
        queue->queue_list->next = queue->queue_list->next->next;
        free(tmp->data);
        free(tmp);
    }
    queue->queuelist_tail = queue->queue_list;
    return QUEUE_OK;
}

static enum QUEUE_STATE queueList_destroy(Queue *queue)
{
    queueList_empty(queue);
    free(queue->queue_list);
    queue->queue_list = NULL;
    queue->queuelist_tail = queue->queue_list;
    return QUEUE_OK;
}

enum QUEUE_STATE queue_headchange(Queue *queue,void *data)
{
    if (queue->mode == QUEUE_ARRAY)
    {
        memcpy(QUEUE_INDEX(queue,queue->queuearray_head),data,queue->unit_size);
    }
    else if(queue->mode == QUEUE_LIST && queue->queue_list->next != NULL)
    {
        memcpy(queue->queue_list->next->data,data,queue->unit_size);
    }
    return QUEUE_OK;
}

enum QUEUE_STATE queue_tailchange(Queue *queue,void *data)
{
    if (queue->mode == QUEUE_ARRAY)
    {
        memcpy(QUEUE_INDEX(queue,queue->queuearray_tail),data,queue->unit_size);
    }
    else if(queue->mode == QUEUE_LIST && (queue->queue_list != queue->queuelist_tail))
    {
        memcpy(queue->queuelist_tail->data,data,queue->unit_size);
    }
    return QUEUE_OK;
}

enum QUEUE_STATE queue_init(Queue *queue,int queue_size,int unit_size,enum QUEUE_TYPE queue_type)
{
    enum QUEUE_STATE ret;
    switch (queue_type)
    {
    case QUEUE_ARRAY:
        ret = queueArray_init(queue,queue_size,unit_size);
        break;
    
    case QUEUE_LIST:
        ret = queueList_init(queue,unit_size);
        break;

    default:
        printf("模式错误\n");
        ret = QUEUE_ERR;
        break;
    }
    queue->mode = queue_type;
    return ret;
}

enum QUEUE_STATE queue_push(Queue *queue,void *node)
{
    enum QUEUE_STATE ret;
    switch (queue->mode)
    {
    case QUEUE_ARRAY:
        ret = queueArray_push(queue,node);
        break;
    
    case QUEUE_LIST:
        ret = queueList_push(queue,node);
        break;
        
    default:
        printf("模式错误\n");
        ret = QUEUE_ERR;
        break;
    }
    return ret;
}

enum QUEUE_STATE queue_pop(Queue *queue,void *node)
{
    enum QUEUE_STATE ret;
    switch (queue->mode)
    {
    case QUEUE_ARRAY:
        ret = queueArray_pop(queue,node);
        break;
    
    case QUEUE_LIST:
        ret = queueList_pop(queue,node);
        break;

    default:
        printf("模式错误\n");
        ret = QUEUE_ERR;
        break;
    }
    return ret;
}

enum QUEUE_STATE queue_empty(Queue *queue)
{
    enum QUEUE_STATE ret;
    switch (queue->mode)
    {
    case QUEUE_ARRAY:
        ret = queueArray_empty(queue);
        break;
    
    case QUEUE_LIST:
        ret = queueList_empty(queue);
        break;

    default:
        printf("模式错误\n");
        ret = QUEUE_ERR;
        break;
    }
    return ret;
}

enum QUEUE_STATE queue_destroy(Queue *queue)
{
    enum QUEUE_STATE ret;
    switch (queue->mode)
    {
    case QUEUE_ARRAY:
        ret = queueArray_destroy(queue);
        break;
    
    case QUEUE_LIST:
        ret = queueList_destroy(queue);
        break;

    default:
        printf("模式错误\n");
        ret = QUEUE_ERR;
        break;
    }
    return ret;
}