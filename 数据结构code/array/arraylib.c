#include "arraylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static enum Array_Error New_Volumn(Array *array_list)
{
    if (array_list == NULL || array_list->head == NULL)
    {
        printf("输出为空\n");
        return INVALID;
    }
    void *tmp = NULL;
    int volumn = 1024 * array_list->unit_size;
    do
    {
        tmp = (void *)realloc(array_list->head,array_list->volumn + volumn);
        volumn = volumn/2;
    } while (tmp == NULL && volumn >= 1);
    
    if(tmp != NULL && tmp != array_list->head)
    {
        array_list->head = tmp;
        return No_error;
    }
    else if(tmp == NULL)
    {
        return INVALID;
    }
    return No_error;
}

enum Array_Error Init_Array(Array *array_list,int Min_Capicity,int unit_size)
{
    if(array_list == NULL || Min_Capicity < 0)
    {
        printf("输入错误\n");
        return INVALID;
    }
    array_list->head = (void *)malloc(unit_size * Min_Capicity);
    array_list->volumn = Min_Capicity;
    array_list->array_length = 1;
    array_list->unit_size = unit_size;
    if (array_list->head == NULL)
    {
        printf("内存不足，无法创建数组\n");
        return No_space;
    }
    return No_error;
}

enum Array_Error Array_Add(Array *array_list,void *node,int index)
{
    int point;
    if (!array_list || index > array_list->array_length - 1|| index < 0 || !array_list->head)
    {
        printf("输入无效\n");
        return INVALID;
    }
    if(array_list->array_length + 1 > array_list->volumn)
    {
        if(New_Volumn(array_list) != No_error)
        {
            printf("没有足够空间分配\n");
            return No_space;
        }
    }
    for (point = array_list->array_length - 1;point > index;point--)
    {
        memcpy(Array_Index(array_list,point),Array_Index(array_list,point - 1),array_list->unit_size);
    }
    memcpy(Array_Index(array_list, point),node,array_list->unit_size);
    array_list->array_length = array_list->array_length + 1;
    return No_error;

}

enum Array_Error Array_Delete(Array *array_list,int index)
{
    int point;
    if (!array_list || index > array_list->array_length - 1 || index < 0 || !array_list->head)
    {
        printf("输入无效\n");
        return INVALID;
    }
    if(array_list->array_length == 0)
    {
        printf("数组为空\n");
        return INVALID;
    }
    if (array_list->array_length - 1 > 1)
    {
        for(point = index;point < array_list->array_length - 1;point++)
        {
            memcpy(Array_Index(array_list,point),Array_Index(array_list,point + 1),array_list->unit_size);
        }
    }
    array_list->array_length = array_list->array_length - 1;
    return No_error;
}

void *Array_Index(Array *array_list,int index)
{
    return (array_list->head + array_list->unit_size * index);
}

int Array_Find(Array *array_list,int( *fuc)(void *node))
{
    int index;
    if (!array_list || !array_list->head || !fuc)
    {
        printf("输入错误\n");
        return -1;
    }
    for(index = 0;index < array_list->array_length - 1;index++)
    {
        if (fuc(Array_Index(array_list,index)))
        {
            printf("找到节点:%d\n",index);
            return index;
        }
    }
    printf("找不到节点\n");
    return Not_found;
}

enum Array_Destroy(Array *array_list)
{
    if (!array_list || !array_list->head)
    {
        printf("输入无效\n");
        return INVALID;
    }
    free(array_list->head);
    array_list->head = NULL;
    return No_error;
}

enum Array_Error Array_Change(Array *array_list,void *data,int index)
{
    if (index > array_list->array_length || index < 0)
    {
        printf("输入无效\n");
        return INVALID;
    }
    memcpy(Array_Index(array_list,index),data,array_list->unit_size);
    return No_error;
}

enum Array_Error Array_Empty(Array *array_list)
{
    if (array_list == NULL)
    {
        printf("输入错误\n");
        return INVALID;
    }
    array_list->array_length = 1;
}
