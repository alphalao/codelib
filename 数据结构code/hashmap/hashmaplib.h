/*
提供链表操作接口
listlib.h,listlib.c
*/
#ifndef __HASHMAPLIB_H__
#define __HASHMAPLIB_H__

typedef struct _HASHUNIT_
{
    void *data;
    char hash_name[20];
    int unit_size;
    enum UNIT_STATE state;
    struct _HASHUNIT_ *next;    //拉链法头指针
}hash_unit;

typedef struct _HASHMAP_
{
    hash_unit *array;
    enum HASH_CALLOPSE hash_callopse;
    int hash_len;   //hash表容量
    int hash_size;  //hash表已用
    int map_len;
}hash_map;

enum UNIT_STATE
{
    EMPTY = 0,
    INVALID
};

enum HASH_CALLOPSE
{
    TEST_ONEPOW = 0,
    TEST_TWOPOW,
    TEST_ZIP
};

enum HASH_STATE
{
    HASH_OK = 0,
    HASH_CALLOPSE,
    HASH_ERR
};

//初始化哈希表
enum HASH_STATE hash_init(hash_map *hash,int unit_size,enum HASH_CALLOPSE hash_callopse);

//哈希添加元素
enum HASH_STATE hash_push(hash_map *hash,void *data,int unit_size,char *hash_str);

//哈希寻找元素
enum HASH_STATE hash_get(hash_map *hash,void *data,char *hash_name);

//哈希删除元素
enum HASH_STATE hash_pop(hash_map *hash,void *data,char *hash_name);

//清空哈希表
enum HASH_STATE hash_empty(hash_map *hash);

//销毁哈希表
enum HASH_STATE hash_destroy(hash_map *hash);