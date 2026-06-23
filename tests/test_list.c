/**
 * @file    test_list.c
 * @brief   双向链表单元测试
 *
 * @details 覆盖:
 *          - 创建/销毁
 *          - push_back / remove
 *          - first / last / count / empty
 *          - 遍历 + 查找删除
 *          - clear
 *          - NULL 参数防御
 *
 * @author  linkedList
 * @version 1.0.0
 * @date    2024
 *
 * @copyright MIT License
 */

#include "list.h"
#include "unity.h"
#include <stdio.h>

/*===========================================================================
 * 测试上下文
 *===========================================================================*/

/**
 * @brief 测试用数据项
 */
typedef struct _TEST_ITEM_
{
    int32_t id;
    double  value;
} TEST_ITEM;

/*===========================================================================
 * 测试用例
 *===========================================================================*/

/**
 * @brief 测试: 创建和销毁
 */
static void test_create_destroy(void)
{
    LIST_HEAD *head = list_create();

    TEST_ASSERT_NOT_NULL(head);

    list_destroy(head);
}

/**
 * @brief 测试: push_back 单个元素
 */
static void test_push_back_single(void)
{
    TEST_ITEM  item = {1, 10.0};
    LIST_HEAD *head = list_create();

    LIST_NODE *node = list_push_back(head, &item);

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL(1, list_count(head));
    TEST_ASSERT_EQUAL(0, list_empty(head));
    TEST_ASSERT_EQUAL(&item, list_first(head));
    TEST_ASSERT_EQUAL(&item, list_last(head));

    list_destroy(head);
}

/**
 * @brief 测试: push_back 多个 + 顺序
 */
static void test_push_back_multiple(void)
{
    TEST_ITEM  a = {1, 1.0};
    TEST_ITEM  b = {2, 2.0};
    TEST_ITEM  c = {3, 3.0};
    LIST_HEAD *head = list_create();
    TEST_ITEM *item = NULL;
    LIST_NODE *n = NULL;
    int32_t    expected = 1;

    list_push_back(head, &a);
    list_push_back(head, &b);
    list_push_back(head, &c);

    TEST_ASSERT_EQUAL(3, list_count(head));

    /* 验证遍历顺序 = 插入顺序 */
    n = head->first;

    while (NULL != n)
    {
        item = (TEST_ITEM *) n->data;
        TEST_ASSERT_EQUAL(expected, item->id);
        expected++;
        n = n->next;
    }

    list_destroy(head);
}

/**
 * @brief 测试: remove 首节点
 */
static void test_remove_first(void)
{
    TEST_ITEM  a = {1, 1.0};
    TEST_ITEM  b = {2, 2.0};
    LIST_HEAD *head = list_create();
    void      *data = NULL;
    LIST_NODE *first_node = NULL;

    list_push_back(head, &a);
    first_node = head->first;
    list_push_back(head, &b);

    data = list_remove(head, first_node);

    TEST_ASSERT_EQUAL(&a, data);
    TEST_ASSERT_EQUAL(1, list_count(head));
    TEST_ASSERT_EQUAL(&b, list_first(head));

    list_destroy(head);
}

/**
 * @brief 测试: remove 尾节点
 */
static void test_remove_last(void)
{
    TEST_ITEM  a = {1, 1.0};
    TEST_ITEM  b = {2, 2.0};
    LIST_HEAD *head = list_create();
    void      *data = NULL;

    list_push_back(head, &a);
    list_push_back(head, &b);

    data = list_remove(head, head->last);

    TEST_ASSERT_EQUAL(&b, data);
    TEST_ASSERT_EQUAL(1, list_count(head));
    TEST_ASSERT_EQUAL(&a, list_last(head));

    list_destroy(head);
}

/**
 * @brief 测试: remove 中间节点
 */
static void test_remove_middle(void)
{
    TEST_ITEM  a = {1, 1.0};
    TEST_ITEM  b = {2, 2.0};
    TEST_ITEM  c = {3, 3.0};
    LIST_HEAD *head = list_create();
    void      *data = NULL;
    LIST_NODE *mid = NULL;

    list_push_back(head, &a);
    mid = list_push_back(head, &b);
    list_push_back(head, &c);

    data = list_remove(head, mid);

    TEST_ASSERT_EQUAL(&b, data);
    TEST_ASSERT_EQUAL(2, list_count(head));
    TEST_ASSERT_EQUAL(&a, list_first(head));
    TEST_ASSERT_EQUAL(&c, list_last(head));

    list_destroy(head);
}

/**
 * @brief 测试: 遍历中查找并删除
 */
static void test_for_each_find_remove(void)
{
    TEST_ITEM  a = {1, 1.0};
    TEST_ITEM  b = {2, 2.0};
    TEST_ITEM  c = {3, 3.0};
    LIST_HEAD *head = list_create();
    TEST_ITEM *item = NULL;
    LIST_NODE *n = NULL;

    list_push_back(head, &a);
    list_push_back(head, &b);
    list_push_back(head, &c);

    n = head->first;

    while (NULL != n)
    {
        item = (TEST_ITEM *) n->data;
        if (2 == item->id)
        {
            list_remove(head, n);
            break;
        }
        n = n->next;
    }

    TEST_ASSERT_EQUAL(2, list_count(head));

    list_destroy(head);
}

/**
 * @brief 测试: clear
 */
static void test_clear(void)
{
    TEST_ITEM  a = {1, 1.0};
    TEST_ITEM  b = {2, 2.0};
    LIST_HEAD *head = list_create();

    list_push_back(head, &a);
    list_push_back(head, &b);

    list_clear(head);

    TEST_ASSERT_EQUAL(0, list_count(head));
    TEST_ASSERT_NULL(list_first(head));
    TEST_ASSERT_NULL(list_last(head));
    TEST_ASSERT_EQUAL(1, list_empty(head));

    list_destroy(head);
}

/**
 * @brief 测试: 空链表查询
 */
static void test_empty_list_queries(void)
{
    LIST_HEAD *head = list_create();

    TEST_ASSERT_EQUAL(0, list_count(head));
    TEST_ASSERT_EQUAL(1, list_empty(head));
    TEST_ASSERT_NULL(list_first(head));
    TEST_ASSERT_NULL(list_last(head));

    list_destroy(head);
}

/**
 * @brief 测试: NULL 参数防御
 */
static void test_null_param_defense(void)
{
    TEST_ITEM  item = {1, 1.0};
    LIST_NODE *node = NULL;

    /* NULL head */
    list_destroy(NULL);
    list_clear(NULL);

    TEST_ASSERT_EQUAL(0, list_count(NULL));
    TEST_ASSERT_EQUAL(1, list_empty(NULL));
    TEST_ASSERT_NULL(list_first(NULL));
    TEST_ASSERT_NULL(list_last(NULL));
    TEST_ASSERT_NULL(list_push_back(NULL, &item));

    /* NULL data */
    LIST_HEAD *head = list_create();
    node = list_push_back(head, NULL);

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_NULL(node->data);

    list_destroy(head);

    /* NULL node to remove */
    head = list_create();
    TEST_ASSERT_NULL(list_remove(head, NULL));
    list_destroy(head);
}

/**
 * @brief 测试: list_push_back 返回节点指针正确
 */
static void test_push_back_return_node(void)
{
    TEST_ITEM  item = {42, 99.9};
    LIST_HEAD *head = list_create();
    LIST_NODE *node = NULL;

    node = list_push_back(head, &item);

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL(&item, node->data);
    TEST_ASSERT_NULL(node->prev);
    TEST_ASSERT_NULL(node->next);
    TEST_ASSERT_EQUAL(node, head->first);
    TEST_ASSERT_EQUAL(node, head->last);

    list_destroy(head);
}

/*===========================================================================
 * 测试入口
 *===========================================================================*/

TEST_MAIN(RUN_TEST(test_create_destroy); RUN_TEST(test_push_back_single);
          RUN_TEST(test_push_back_multiple); RUN_TEST(test_remove_first);
          RUN_TEST(test_remove_last); RUN_TEST(test_remove_middle);
          RUN_TEST(test_for_each_find_remove); RUN_TEST(test_clear);
          RUN_TEST(test_empty_list_queries); RUN_TEST(test_null_param_defense);
          RUN_TEST(test_push_back_return_node);)
