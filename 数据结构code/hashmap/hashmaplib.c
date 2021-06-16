#include "hashmaplib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_MAP_SIZE 128
#define HASH_MAP_CAP 0.75

//求x的y次方
static unsigned int pow(int x,int y)
{
    int tmp;
    int powx = x;
    for (tmp = 1;tmp < y;tmp++)
    {
        x *= powx;
    }
    return powx;
}

//使用BKDHash算法
static unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

//开放寻址找到可用节点（powx：开放幂，index：计算节点）
static hash_unit *hash_find(hash_map *hash,int powx,int *index)
{
    int index_x = 1;
    int tmp = 0;
    while((*index + tmp < hash->hash_len) || (*index - tmp < 0))
    {
        tmp = pow(index_x,powx);
        if ((*index + tmp < hash->hash_len) && (hash->array[*index + tmp].state == EMPTY))
        {
            *index = *index + tmp;
            printf("find node:%d\n",*index);
            return &(hash->array[*index]);
        }
        if (((*index - tmp < 0)) && (hash->array[*index - tmp].state == EMPTY))
        {
            *index = *index - tmp;
            printf("find node:%d\n",*index);
            return &(hash->array[*index]);
        }
        index_x++;
    }
    printf("not foud:%d\n",*index);
    return NULL;
}

static hash_unit *hash_find_name(hash_map *hash,int powx,char *name,int *index)
{
    int index_x = 1;
    int tmp = 0;
    while((*index + tmp < hash->hash_len) || (*index - tmp < 0))
    {
        tmp = pow(index_x,powx);
        if ((*index + tmp < hash->hash_len) && (hash->array[*index + tmp].state == INVALID) && (strncpy(hash->array[*index + tmp].hash_name,name,strlen(name)) == 0))
        {
            *index = *index + tmp;
            printf("find node:%d\n",*index);
            return &(hash->array[*index]);
        }
        if (((*index - tmp < 0)) && (hash->array[*index - tmp].state == INVALID) && (strncpy(hash->array[*index + tmp].hash_name,name,strlen(name)) == 0))
        {
            *index = *index - tmp;
            printf("find node:%d\n",*index);
            return &(hash->array[*index]);
        }
        index_x++;
    }
    printf("not foud:%d\n",*index);
    return NULL;
}

static enum HASH_STATE hash_relloc(hash_map *hash)
{
    hash_map new_hash;
    hash_init(&new_hash,hash->hash_callopse);
    new_hash.array = (void *)calloc(hash->hash_len * 2,sizeof(hash_unit));
    if (tmp == NULL)
    {
        printf("扩容错误\n");
        return HASH_ERR;
    }
    int index = 0;
    hash_unit *tmp = NULL;
    for (index = 0;index < hash->hash_len;index++)
    {
        if (hash->array[index].state == INVALID)
        {
            tmp = hash->array[index].next;
            while(tmp != NULL)
            {
                hash->array[index].next = tmp->next;
                hash_push(new_hash,tmp->data,tmp->unit_size,tmp->hash_name);
                free(tmp->data);
                free(tmp);
                tmp = hash->array[index].next;
            }
        }
    }
    free(hash->array);
    hash->array = new_hash.array;
    return HASH_OK;
}

//返回hash可用节点
static hash_unit *hash_node(hash_map *hash,char *hash_code)
{
    int index = BKDRHash(hash_code) % (hash->hash_len);
    hash_unit *tmp = NULL;
    if (hash->array[index].state == EMPTY)
    {
        return &(hash->array[index]);
    }
    switch (hash->hash_callopse)
    {
    case TEST_ONEPOW:
        tmp = hash_find(hash,1,&index);
        break;

    case TEST_TWOPOW:
        tmp = hash_find(hash,2,&index);
        break;
    
    case TEST_ZIP:
        tmp = hash->array[index].next;
        while(tmp != NULL)
        {
            tmp = tmp->next;
        }
        tmp = (void *)malloc(sizeof(hash_unit));
        break;

    default:
        printf("无效的类型\n");
        break;
    }
    return tmp;
}

enum HASH_STATE hash_destroy(hash_map *hash)
{
    int index = 0;
    hash_unit *tmp = NULL;
    for (index = 0;index < hash->hash_len;index++)
    {
        if (hash->array[index].state == INVALID)
        {
            tmp = hash->array[index].next;
            while(tmp != NULL)
            {
                hash->array[index].next = tmp->next;
                free(tmp->data);
                free(tmp);
                tmp = hash->array[index].next;
            }
        }
    }
    free(hash->array);
    return HASH_OK;
}

enum HASH_STATE hash_empty(hash_map *hash)
{
    int index = 0;
    hash_unit *tmp = &(hash->array[index]);
    for (index = 0;index < hash->hash_len;index++)
    {
        hash->array[index].state = EMPTY;
        free(hash->array[index].data);
        tmp = hash->array[index].next;
        while (tmp != NULL)
        {
            hash->array[index].next = tmp->next->next;
            free(tmp->data);
            free(tmp);
            tmp = hash->array[index].next;
        }
        hash->array[index].next = NULL;
    }
    return HASH_OK;
}

enum HASH_STATE hash_pop(hash_map *hash,void *data,char *hash_name)
{
    int index = BKDRHash(hash_name) % (hash->hash_len);
    hash_unit *tmp = &(hash->array[index]);
    if (strncpy(hash_name,tmp->hash_name,strlen(hash_name)) == 0)
    {
        printf("find node:%d\n",index);
        if (hash->hash_callopse == TEST_ZIP)
        {
            if (tmp->next == NULL)
            {
                tmp->state = EMPTY;
                free(tmp->data);
            }
            else
            {
                free(tmp->data);
                tmp.data = tmp->next->data;
                strncpy(tmp.hash_name,tmp->next->hash_name,20);
                tmp->state = tmp->next->state;
                tmp->unit_size = tmp->next->unit_size;
                tmp->next = tmp->next->next;
            }
        }
        return HASH_OK;
    }
    else
    {
        switch (hash->hash_callopse)
        {
        case TEST_ONEPOW:
            tmp = hash_find_name(hash,1,hash_name,&index);
            break;

        case TEST_TWOPOW:
            tmp = hash_find_name(hash,2,hash_name.&index);

        case TEST_ZIP:
            hash_unit *tmp_ = &(hash->array[index]);
            tmp = hash->array[index].next;
            while(tmp != NULL)
            {
                if (strncpy(tmp->hash_name,hash_name,strlen(hash_name)) == 0)
                {
                    break;
                }
                tmp_ = tmp;
                tmp = tmp->next;
            }
            if (tmp != NULL)
            {
                tmp_->next = tmp->next;
            }
        default:
            printf("无效的类型\n");
            break;
        }
    }
    if (tmp == NULL)
    {
        printf("没有节点\n");
        return HASH_ERR;
    }
    else
    {
        free(tmp->data);
        tmp->state = EMPTY;
        return HASH_OK;
    }
}

enum HASH_STATE hash_get(hash_map *hash,void *data,char *hash_name)
{
    int index = BKDRHash(hash_name) % (hash->hash_len);
    hash_unit *tmp = &(hash->array[index]);
    if (strncpy(hash_name,tmp->hash_name,strlen(hash_name)) == 0)
    {
        printf("find node:%d\n",index);
    }
    else
    {
        switch (hash->hash_callopse)
        {
        case TEST_ONEPOW:
            tmp = hash_find_name(hash,1,hash_name,&index);
            break;

        case TEST_TWOPOW:
            tmp = hash_find_name(hash,2,hash_name.&index);

        case TEST_ZIP:
            tmp = hash->array[index].next;
            while(tmp != NULL)
            {
                if (strncpy(tmp->hash_name,hash_name,strlen(hash_name)) == 0)
                {
                    break;
                }
                tmp = tmp->next;
            }
        default:
            printf("无效的类型\n");
            break;
        }
    }
    if (tmp == NULL)
    {
        printf("没有节点\n");
        return HASH_ERR;
    }
    else
    {
        memcpy(data,tmp->data,tmp->unit_size);
        return HASH_OK;
    }
}

enum HASH_STATE hash_init(hash_map *hash,enum HASH_CALLOPSE hash_callopse)
{
    hash->array = (void *)calloc(HASH_MAP_SIZE,sizeof(hash_unit));
    if (hash->array == NULL)
    {
        printf("空间不足\n");
        return HASH_ERR;
    }
    hash->unit_size = unit_size;
    hash->hash_len = HASH_MAP_SIZE;
    hash->hash_callopse = hash_callopse;
    return HASH_OK;
}

enum HASH_STATE hash_push(hash_map *hash,void *data,int unit_size,char *hash_str)
{
    enum HASH_STATE ret = HASH_OK;
    if (!hash->array)
    {
        printf("输入错误\n");
        return HASH_ERR;
    }
    if ((hash->hash_size + 1) <= hash->hash_len * HASH_MAP_CAP)
    {
        ret = hash_relloc(hash);
    }
    if (ret != HASH_OK)
    {
        return ret;
    }
    hash_unit *tmp = hash_node(hash,hash_str);
    if (tmp == NULL)
    {
        printf("push无节点\n");
        return HASH_ERR;
    }
    if ((tmp->data = (void *)malloc(unit_size)) == NULL)
    {
        ret = HASH_ERR;
        return ret;
    }
    hash->hash_size = hash->hash_size + 1;
    memcpy(tmp->data,data,unit_size);
    strncpy(tmp->hash_name,hash_str,strlen(hash_str));
    tmp->next = NULL;
    tmp->state = INVALID;
    tmp->unit_size = unit_size;
    return ret;
}