/*
提供栈操作接口
stacklib.h stacklib.c
*/
#ifndef __STACKLIB_H__
#define __STACKLIB_H__

typedef struct _STACK_ARRAY
{
    int volumn;
    int stack_head;
    int stack_bottom;
    int unit_size;
    void *stack;
}STACK_ARRAY;

enum STACK_STATE
{
    STACK_OK = 0,
    STACK_ERR
};

//初始化栈
enum STACK_STATE stack_init(STACK_ARRAY *stack_,int stack_size,int unit_size);

//元素出栈
enum STACK_STATE stack_pop(STACK_ARRAY *stack_,void *node);

//元素压栈
enum STACK_STATE stack_push(STACK_ARRAY *stack_,void *node);

//清空栈
enum STACK_STATE stack_empty(STACK_ARRAY *stack_);

//销毁栈
enum STACK_STATE stack_destroy(STACK_ARRAY *stack_);

//修改栈顶
enum STACK_STATE stack_change(STACK_ARRAY *stack_,void *node);

#endif