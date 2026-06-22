/**
 * @file    list.c
 * @brief   双向链表实现
 *
 * @details 数据指针式双向链表。
 *          - 非循环，head->first/head->last 为 NULL 时为空
 *          - push_back O(1)，remove O(1)
 *          - 不拷贝用户数据，不管理 data 生命周期
 *
 * @author  linkedList
 * @version 1.0.0
 * @date    2024
 *
 * @copyright MIT License
 */

#include "list.h"

#include <stdlib.h>

/*===========================================================================
 * 公共 API 实现
 *===========================================================================*/

/**
 * @brief 创建链表
 *
 * @return 成功返回链表头指针，失败返回 NULL
 */
LIST_HEAD *list_create(void)
{
    LIST_HEAD *head = calloc(1, sizeof(LIST_HEAD));

    return head;
}

/**
 * @brief 销毁链表，释放所有节点
 *
 * @param[in] head 链表头指针，传 NULL 无操作
 */
void list_destroy(LIST_HEAD *head)
{
    if (NULL == head)
    {
        return;
    }

    list_clear(head);
    free(head);

    return;
}

/**
 * @brief 尾插 —— 在链表末尾追加节点
 *
 * @param[in] head 链表头指针
 * @param[in] data 用户数据指针
 * @return 成功返回新节点指针，失败返回 NULL
 */
LIST_NODE *list_push_back(LIST_HEAD *head, void *data)
{
    LIST_NODE *node = NULL;

    if (NULL == head)
    {
        return NULL;
    }

    node = calloc(1, sizeof(LIST_NODE));

    if (NULL == node)
    {
        return NULL;
    }

    node->data = data;

    if (NULL == head->last)
    {
        /* 空链表：首尾都指向新节点 */
        head->first = node;
        head->last = node;
    }
    else
    {
        /* 非空：接在尾部 */
        node->prev = head->last;
        head->last->next = node;
        head->last = node;
    }

    head->count++;

    return node;
}

/**
 * @brief 删除已知节点
 *
 * @param[in] head 链表头指针
 * @param[in] node 要删除的节点
 * @return 节点持有的 data 指针
 */
void *list_remove(LIST_HEAD *head, LIST_NODE *node)
{
    void *data = NULL;

    if ((NULL == head) || (NULL == node))
    {
        return NULL;
    }

    data = node->data;

    /* 更新前驱 */
    if (NULL != node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        /* node 是首节点 */
        head->first = node->next;
    }

    /* 更新后继 */
    if (NULL != node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        /* node 是尾节点 */
        head->last = node->prev;
    }

    head->count--;
    free(node);

    return data;
}

/**
 * @brief 获取首节点数据
 *
 * @param[in] head 链表头指针
 * @return 首节点 data，空链表返回 NULL
 */
void *list_first(const LIST_HEAD *head)
{
    if (NULL == head)
    {
        return NULL;
    }

    if (NULL == head->first)
    {
        return NULL;
    }

    return head->first->data;
}

/**
 * @brief 获取尾节点数据
 *
 * @param[in] head 链表头指针
 * @return 尾节点 data，空链表返回 NULL
 */
void *list_last(const LIST_HEAD *head)
{
    if (NULL == head)
    {
        return NULL;
    }

    if (NULL == head->last)
    {
        return NULL;
    }

    return head->last->data;
}

/**
 * @brief 获取节点数量
 *
 * @param[in] head 链表头指针
 * @return 节点数，head 为 NULL 返回 0
 */
size_t list_count(const LIST_HEAD *head)
{
    if (NULL == head)
    {
        return 0;
    }

    return head->count;
}

/**
 * @brief 判断链表是否为空
 *
 * @param[in] head 链表头指针
 * @return 1 = 空，0 = 非空
 */
int32_t list_empty(const LIST_HEAD *head)
{
    if (NULL == head)
    {
        return 1;
    }

    return (NULL == head->first) ? 1 : 0;
}

/**
 * @brief 清空链表 —— 释放所有节点
 *
 * @param[in] head 链表头指针
 */
void list_clear(LIST_HEAD *head)
{
    LIST_NODE *curr = NULL;
    LIST_NODE *next = NULL;

    if (NULL == head)
    {
        return;
    }

    curr = head->first;

    while (NULL != curr)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }

    head->first = NULL;
    head->last = NULL;
    head->count = 0;

    return;
}
