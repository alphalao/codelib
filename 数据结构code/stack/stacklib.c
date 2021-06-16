#include <stdio.h>
#include <string.h>
#include "stacklib.h"

#define STACK_INDEX(array,index) (array->stack + index * array->unit_size)

enum STACK_STATE stack_init(STACK_ARRAY *stack_,int stack_size,int unit_size)
{
    stack_->stack = NULL;
    stack_->stack = (void *)malloc(stack_size * unit_size);
    if (NULL != stack_->stack)
    {
        printf("init stack\n");
        stack_->stack_head = 0;
        stack_->stack_bottom = stack_->stack_head;
        stack_->volumn = stack_size;
        stack_->unit_size = unit_size;
        return STACK_OK;
    }
    return STACK_ERR;
}

enum STACK_STATE stack_pop(STACK_ARRAY *stack_,void *node)
{
    if(!stack_->stack || stack_->stack_bottom == stack_->stack_head)
    {
        printf("stack is empty\n");
        return STACK_ERR;
    }
    memcpy(node,STACK_INDEX(stack_,(stack_->stack_head - 1)),stack_->unit_size);
    stack_->stack_head = stack_->stack_head - 1;
    return STACK_OK;
}

enum STACK_STATE stack_push(STACK_ARRAY *stack_,void *node)
{
    if(!stack_->stack || stack_->stack_head == stack_->volumn)
    {
        printf("stack not enough space\n");
        return STACK_ERR;
    }
    memcpy(STACK_INDEX(stack_,stack_->stack_head),node,stack_->unit_size);
    stack_->stack_head = stack_->stack_head + 1;
    return STACK_OK;
}

enum STACK_STATE stack_destroy(STACK_ARRAY *stack_)
{
    stack_empty(stack_);
    free(stack_->stack_head);
    stack_->stack = NULL;
}

enum STACK_STATE stack_empty(STACK_ARRAY *stack_)
{
    stack_->stack_head = 0;
    stack_->stack_bottom = stack_->stack_head;
    return STACK_OK;
}

enum STACK_STATE stack_change(STACK_ARRAY *stack_,void *node)
{
    if(stack_->stack_head == stack_->stack_bottom)
    {
        printf("stack is empty\n");
        return STACK_ERR;
    }
    memcpy(STACK_INDEX(stack_,(stack_->stack_head - 1)),node,stack_->unit_size);
    return STACK_OK;
}