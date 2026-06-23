# linkedList

嵌入式 C 双向链表，数据指针式，只做 push_back。

## 特性

- 双向链表，push_back / remove O(1)
- `void *data` 指针存储，不拷贝用户数据
- 不管理 data 生命周期（链表只释放自己的节点壳）
- C11，零第三方依赖
- CMake 构建，支持交叉编译

## 构建

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

产物：
- `liblinkedlist.a` — 静态库
- `test_list` — 单元测试
- `list_demo` — 演示程序

## API

```c
#include "list.h"

// 生命周期
LIST_HEAD *list_create(void);
void       list_destroy(LIST_HEAD *head);

// 插入/删除
LIST_NODE *list_push_back(LIST_HEAD *head, void *data);
void      *list_remove(LIST_HEAD *head, LIST_NODE *node);

// 查询
void      *list_first(const LIST_HEAD *head);
void      *list_last(const LIST_HEAD *head);
size_t     list_count(const LIST_HEAD *head);
int32_t    list_empty(const LIST_HEAD *head);

// 遍历
LIST_NODE *n = head->first;
while (NULL != n) { data = n->data; ... n = n->next; }

// 清空
void list_clear(LIST_HEAD *head);
```

## 示例

```c
LIST_HEAD *list = list_create();

sensor_t a = {1, 25.0};
sensor_t b = {2, 26.5};

list_push_back(list, &a);
list_push_back(list, &b);

LIST_NODE *n = list->first;

while (NULL != n)
{
    sensor_t *s = (sensor_t *) n->data;
    printf("sensor %d: %.1f\n", s->id, s->temp);
    n = n->next;
}

list_destroy(list);
```

## 测试

```bash
./build/test_list
# 11 项测试，全部通过
```

## 文件结构

```
linkedList/
├── CMakeLists.txt
├── include/
│   └── list.h
├── src/
│   └── list.c
├── tests/
│   ├── unity.h
│   └── test_list.c
├── examples/
│   └── list_demo.c
├── docs/
│   └── design.md
└── README.md
```

## 许可

MIT
