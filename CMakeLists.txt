cmake_minimum_required(VERSION 3.10)
project(linkedList VERSION 1.0.0 LANGUAGES C)

#===========================================================================
# 库: liblinkedlist (静态库 + 动态库)
#===========================================================================

# 静态库
add_library(linkedlist STATIC
    src/list.c
)

target_include_directories(linkedlist PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(linkedlist PRIVATE
    -Wall -Wextra -Werror
    -Wdeclaration-after-statement
    -O2
    -std=c11
)

# 动态库
add_library(linkedlist_shared SHARED
    src/list.c
)

set_target_properties(linkedlist_shared PROPERTIES
    OUTPUT_NAME linkedlist
)

target_include_directories(linkedlist_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(linkedlist_shared PRIVATE
    -Wall -Wextra -Werror
    -Wdeclaration-after-statement
    -O2
    -std=c11
    -fPIC
)

#===========================================================================
# 示例: list_demo
#===========================================================================
add_executable(list_demo
    examples/list_demo.c
)
target_link_libraries(list_demo PRIVATE linkedlist)

#===========================================================================
# 单元测试
#===========================================================================
add_executable(test_list
    tests/test_list.c
)
target_link_libraries(test_list PRIVATE linkedlist)

target_include_directories(test_list PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_compile_options(test_list PRIVATE
    -Wall -Wextra -O0 -g
)

#===========================================================================
# 安装规则
#===========================================================================
install(TARGETS linkedlist linkedlist_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/
    DESTINATION include
)
