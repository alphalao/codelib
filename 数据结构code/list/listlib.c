#include "listlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum List_Error Init_List(List *link_list,int unit_size)
{
    if(link_list->head == NULL)
    {
        link_list->head = (List_Unit *)malloc(sizeof(List_Unit));
        if (link_list->head == NULL)
        {
            printf("分配空间不足\n");
            return INVALID;
        }
        link_list->list_length = 0;
        link_list->unit_size = unit_size;
        link_list->head->next = NULL;
        link_list->tail = link_list->head;
    }
    return No_error;
}

enum List_Error List_Add(List *link_list,void *node,List_Unit *pre)
{
    if (!link_list || !link_list->head || !pre)
    {
        printf("无效输入\n");
        return INVALID;
    }
    List_Unit *tmp = NULL;
    tmp = (List_Unit *)malloc(sizeof(List_Unit));
    if (tmp == NULL)
    {
        printf("分配空间不足\n");
        return INVALID;
    }
    tmp->data = (void *)malloc(sizeof(link_list->unit_size));
    if (tmp->data == NULL)
    {
        printf("分配空间不足\n");
        return INVALID;
    }
    memcpy(tmp->data,node,link_list->unit_size);
    if (pre == link_list->tail)
    {
        link_list->tail->next = tmp;
        tmp->next = NULL;
        link_list->tail = tmp;
        link_list->list_length = link_list->list_length + 1;
        return No_error;
    }
    link_list->list_length = link_list->list_length + 1;
    tmp->next = pre->next;
    pre->next = tmp;
    return No_error;
}

enum List_Error List_Delete(List *link_list,List_Unit *node)
{
    if (!link_list || !link_list->head || !node || link_list->list_length == 0)
    {
        printf("无效输入\n");
        return INVALID;
    }
    List_Unit *tmp = NULL;
    for (tmp = link_list->head;tmp->next != node;tmp = tmp->next);
    tmp->next = node->next;
    node->next = NULL;
    free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
    link_list->tail = tmp->next;
    link_list->list_length = link_list->list_length - 1;
    if (link_list->list_length == 0)
    {
        link_list->tail = link_list->head;
    }
    return No_error;
}

List_Unit *List_Find(List *link_list,int( *fuc)(struct void *))
{
    if (!link_list || !link_list->head || !fuc || link_list->list_length == 0)
    {
        printf("无效输入\n");
        return NULL;
    }
    List_Unit *tmp = NULL;
    for (tmp = link_list->head;tmp != NULL;tmp = tmp->next)
    {
        if (fuc(tmp->data))
        {
            return tmp;
        }
    }
    printf("找不到节点\n");
    return NULL;
}

enum List_Error List_Change(List *link_list,void *node,List_Unit *pre)
{
    if (!link_list || !link_list->head || !pre || link_list->list_length == 0)
    {
        printf("输入错误\n");
        return INVALID;
    }
    memcpy(pre->data,node,link_list->unit_size);
    return No_error;
}

enum List_Error List_Destroy(List *link_list)
{
    if (No_error != List_Empty(link_list))
    {
        printf("清除链表错误\n");
        return INVALID;
    }
    free(link_list->head);
    link_list->head = NULL;
    link_list->tail = NULL;
    return No_error;
}

enum List_Error List_Empty(List *link_list)
{
    if(!link_list ||!link_list->head || link_list->list_length == 0)
    {
        printf("输入错误\n");
        return INVALID;
    }
    List_Unit *tmp = NULL;
    for (tmp = link_list->head->next;tmp != NULL;tmp = link_list->head->next)
    {
        link_list->head->next = tmp->next;
        tmp->next = NULL;
        free(tmp->data);
        tmp->data = NULL;
        free(tmp);
    }
    link_list->tail = link_list->head;
    link_list->list_length = 0;
    return No_error;
}