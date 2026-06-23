/**
 * @file    list.h
 * @brief   双向链表
 *
 * @details 嵌入式 C 用双向链表，只做 push_back，够用就好。
 *          - 数据指针式 (void *)，不拷贝用户数据
 *          - push_back / remove O(1)
 *          - 非循环，首尾有 NULL 边界
 *
 * @author  linkedList
 * @version 1.0.0
 * @date    2024
 *
 * @copyright MIT License
 */

#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================
 * 数据结构 — typedef struct _TAG_ { ... } NAME
 *===========================================================================*/

/**
 * @brief 链表节点
 *
 * 双向节点，持有用户数据指针。
 */
typedef struct _LIST_NODE_
{
    struct _LIST_NODE_ *next; /**< 后继节点 */
    struct _LIST_NODE_ *prev; /**< 前驱节点 */
    void               *data; /**< 用户数据指针 */
} LIST_NODE;

/**
 * @brief 链表头
 *
 * 持有首尾指针和计数，O(1) 查询。
 */
typedef struct _LIST_HEAD_
{
    LIST_NODE *first; /**< 首节点 */
    LIST_NODE *last;  /**< 尾节点 */
    size_t     count; /**< 节点数 */
} LIST_HEAD;

/*===========================================================================
 * 生命周期
 *===========================================================================*/

/**
 * @brief 创建链表
 *
 * @return 成功返回链表头指针，失败返回 NULL
 */
LIST_HEAD *list_create(void);

/**
 * @brief 销毁链表，释放所有节点
 *
 * 不会释放 data 指向的用户数据。
 *
 * @param[in] head 链表头指针，传 NULL 无操作
 */
void list_destroy(LIST_HEAD *head);

/*===========================================================================
 * 插入 / 删除 — O(1)
 *===========================================================================*/

/**
 * @brief 尾插 —— 在链表末尾追加节点
 *
 * @param[in] head 链表头指针
 * @param[in] data 用户数据指针
 * @return 成功返回新节点指针，失败返回 NULL
 */
LIST_NODE *list_push_back(LIST_HEAD *head, void *data);

/**
 * @brief 删除已知节点
 *
 * @param[in] head 链表头指针
 * @param[in] node 要删除的节点（必须是本链表中的节点）
 * @return 节点持有的 data 指针
 */
void *list_remove(LIST_HEAD *head, LIST_NODE *node);

/*===========================================================================
 * 查询 — O(1)
 *===========================================================================*/

/**
 * @brief 获取首节点数据
 *
 * @param[in] head 链表头指针
 * @return 首节点 data，空链表返回 NULL
 */
void *list_first(const LIST_HEAD *head);

/**
 * @brief 获取尾节点数据
 *
 * @param[in] head 链表头指针
 * @return 尾节点 data，空链表返回 NULL
 */
void *list_last(const LIST_HEAD *head);

/**
 * @brief 获取节点数量
 *
 * @param[in] head 链表头指针
 * @return 节点数，head 为 NULL 返回 0
 */
size_t list_count(const LIST_HEAD *head);

/**
 * @brief 判断链表是否为空
 *
 * @param[in] head 链表头指针
 * @return 1 = 空，0 = 非空
 */
int32_t list_empty(const LIST_HEAD *head);

/*===========================================================================
 * 遍历 / 清空
 *===========================================================================*/

/**
 * @brief 清空链表 —— 释放所有节点
 *
 * 不会释放 data 指向的用户数据。
 *
 * @param[in] head 链表头指针
 */
void list_clear(LIST_HEAD *head);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */
