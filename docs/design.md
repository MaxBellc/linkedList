# linkedList — 双向链表 设计文档

## 1. 设计目标

嵌入式 C 项目用，**只做 push_back**，够用就好。

| 特性 | 说明 |
|------|------|
| 类型 | 双向链表（非循环） |
| 插入 | 只有 `push_back`，O(1) |
| 删除 | `list_remove` 已知节点，O(1) |
| 数据存储 | `void *data` 指针，不拷贝用户数据 |
| 节点内存 | `push_back` 时 malloc，`remove` 时 free |
| 依赖 | `<stdint.h>` `<stddef.h>` `<stdlib.h>` |
| 标准 | C11 |
| 形态 | `.h` + `.c` |

---

## 2. 数据结构

```c
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
```

```
空链表:                      含 2 个节点:
head                         head
┌──────────────┐            ┌──────────────┐
│ first: NULL  │            │ first ───────┼──→ [node] ⇄ [node] ←──┐
│ last:  NULL  │            │ last  ───────┼──→ [node] ⇄ [node] ←──┘
│ count: 0     │            │ count: 2     │
└──────────────┘            └──────────────┘
                                 │              │
                            用户数据A        用户数据B
```

4 字节对齐：两个结构体均为 8+8+8=24 字节，24%4=0，无需 `_res`。

---

## 3. API

```c
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
 * 插入 / 删除
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
 * @param[in] node 要删除的节点
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
 * @brief 遍历链表
 *
 * @param pos  循环变量，每次迭代指向当前节点的 data
 * @param head 链表头指针
 */
#define list_for_each(pos, head) \
    for (LIST_NODE *_n = (head)->first; \
         (NULL != _n) && ((pos) = _n->data, 1); \
         _n = _n->next)

/**
 * @brief 清空链表 —— 释放所有节点
 *
 * 不会释放 data 指向的用户数据。
 *
 * @param[in] head 链表头指针
 */
void list_clear(LIST_HEAD *head);
```

---

## 4. 复杂度

```
create       O(1)
destroy      O(n)
push_back    O(1)
remove       O(1)
first/last   O(1)
count/empty  O(1)
clear        O(n)
for_each     O(n)
```

---

## 5. 使用示例

```c
LIST_HEAD *list = list_create();
if (NULL == list)
{
    return -1;
}

sensor_t a = {1, 25.0};
sensor_t b = {2, 26.5};

list_push_back(list, &a);
list_push_back(list, &b);

/* 遍历 */
sensor_t *s = NULL;
list_for_each(s, list)
{
    printf("sensor %d\n", s->id);
}

/* 查找 + 删除 —— 遍历中 _n 指向当前 LIST_NODE */
list_for_each(s, list)
{
    if (0 == s->id)
    {
        list_remove(list, _n);
        break;
    }
}

list_destroy(list);
```

---

## 6. 设计取舍

| 决策 | 选择 | 权衡 |
|------|------|------|
| 插入方式 | 只有 push_back | 够用，API 最小化 |
| 删除方式 | 已知节点 O(1) | 查找需调用者遍历 |
| 数据存储 | `void *` 指针 | 通用但无类型检查 |
| 节点内存 | malloc/free | 嵌入式中可替换为内存池 |
| 查找 | 不提供 list_find | 调用者遍历时自己判断更灵活 |
| 线程安全 | 不提供 | 单线程嵌入式够用 |

---

## 7. 不做什么

- 不 push_front、不 pop_front、不 insert
- 不 list_find —— 调用者遍历时自己判断
- 不深拷贝 —— `void *data` 只存指针
- 不做线程安全

---

## 8. 文件规划

```
linkedList/
├── include/
│   └── list.h              # 公共 API
├── src/
│   └── list.c              # 实现
├── tests/
│   ├── unity.h             # 复用 timer_sched 测试框架
│   └── test_list.c         # 单元测试
├── examples/
│   └── list_demo.c         # 演示
├── docs/
│   └── design.md           # 本文件
└── CMakeLists.txt
```
