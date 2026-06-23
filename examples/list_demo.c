/**
 * @file    list_demo.c
 * @brief   双向链表使用演示
 *
 * @details 演示 push_back、遍历、查找删除、clear 操作。
 *
 * @author  linkedList
 * @version 1.0.0
 * @date    2024
 *
 * @copyright MIT License
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 传感器数据
 */
typedef struct _SENSOR_
{
    int32_t id;
    double  temp;
} SENSOR;

/**
 * @brief 程序入口
 *
 * @return EXIT_SUCCESS
 */
int32_t main(void)
{
    LIST_HEAD *list = NULL;
    SENSOR     a = {1, 25.0};
    SENSOR     b = {2, 26.5};
    SENSOR     c = {3, 24.2};
    SENSOR    *s = NULL;

    printf("=== 双向链表演示 ===\n\n");

    list = list_create();
    if (NULL == list)
    {
        return EXIT_FAILURE;
    }

    /* push_back */
    list_push_back(list, &a);
    list_push_back(list, &b);
    list_push_back(list, &c);

    printf("push_back 后: count=%zu\n\n", list_count(list));

    /* 遍历 */
    printf("遍历:\n");
    LIST_NODE *n = list->first;

    while (NULL != n)
    {
        s = (SENSOR *) n->data;
        printf("  sensor %d: %.1f C\n", s->id, s->temp);
        n = n->next;
    }

    /* 查找 + 删除 */
    printf("\n删除 id=2...\n");
    n = list->first;

    while (NULL != n)
    {
        s = (SENSOR *) n->data;
        if (2 == s->id)
        {
            list_remove(list, n);
            break;
        }
        n = n->next;
    }

    printf("删除后: count=%zu\n", list_count(list));
    printf("首节点: sensor %d\n", ((SENSOR *) list_first(list))->id);
    printf("尾节点: sensor %d\n", ((SENSOR *) list_last(list))->id);

    list_destroy(list);
    printf("\n销毁完成。\n");

    return EXIT_SUCCESS;
}
