/*
提供数组操作接口
arraylib.h,arraylib.c
*/
#ifndef __ARRAYLIB_H__
#define __ARRAYLIB_H__

typedef struct _ARRAYSTRUCT_
{
    void *head;
    int volumn;
    int unit_size;
    int array_length;
}Array;

enum Array_Error
{
    No_error = 0,
    No_space,       //扩充空间错误
    Not_found,      //找不到目标数据
    No_Conserve,    //没剩余空间
    INVALID,
    Unknow
};

//初始化数组
enum Array_Error Init_Array(Array *array_list,int Min_Capicity,int unit_size);

//插入节点
enum Array_Error Array_Add(Array *array_list,void *node,int index);

//删除节点
enum Array_Error Array_Delete(Array *array_list,int index);

//索引节点
void *Array_Index(Array *array_list,int index);

//寻找节点
int Array_Find(Array *array_list,int( *fuc)(void *node));

//改变节点
enum Array_Error Array_Change(Array *array_list,void *data,int index);

//清空数组
enum Array_Error Array_Empty(Array *array_list);

//销毁数组
enum Array_Destroy(Array *array_list);

#endif