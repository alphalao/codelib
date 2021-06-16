/*
提供链表操作接口
listlib.h,listlib.c
*/
#ifndef __LISTLIB_H__
#define __LISTLIB_H__

typedef struct _LIST_UNIT_
{
    void *data;
    struct _LIST_UNIT_ *next;
}List_Unit;

typedef struct _LIST_STRUCT_
{
    int unit_size;
    List_Unit *head;
    List_Unit *tail;
    int list_length;
}List;

enum List_Error
{
    No_error = 0,
    Insert_error,   //插入错误
    Not_found,      //找不到目标数据
    INVALID,
    Unknow
};

//初始化链表
enum List_Error Init_List(List *link_list,int unit_size);

//插入节点
enum List_Error List_Add(List *link_list,void *node,List_Unit *pre);

//删除节点
enum List_Error List_Delete(List *link_list,List_Unit *node);

//寻找节点
List_Unit *List_Find(List *link_list,int( *fuc)(void *));

//改变节点
enum List_Error List_Change(List *link_list,void *node,List_Unit *pre);

//销毁链表
enum List_Error List_Destroy(List *link_list);

//清空链表
enum List_Error List_Empty(List *link_list);

#endif