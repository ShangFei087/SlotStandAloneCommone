#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include "qs.h"

typedef struct {
    int32_t* data;    // 底层数据数组
    size_t size;      // 当前元素数量
    size_t capacity;  // 当前容量
} int_vector;

// 从数组创建 vector
int_vector* int_vector_from_array(const int* arr, size_t count);
// 随机移除一个元素（安全接口）
int int_vector_random_remove(int_vector* vec, int* out_value);
// 基础生命周期
int_vector* int_vector_create(size_t initial_capacity);
void int_vector_destroy(int_vector* vec);

// 容量相关
int32_t int_vector_reserve(int_vector* vec, size_t new_capacity);
int32_t int_vector_shrink_to_fit(int_vector* vec);
size_t int_vector_capacity(const int_vector* vec);
size_t int_vector_size(const int_vector* vec);
int32_t int_vector_empty(const int_vector* vec);

// 元素访问
int32_t int_vector_at(const int_vector* vec, size_t index, int32_t* out_value);
int32_t int_vector_get(const int_vector* vec, size_t index);  // 更轻量的读取接口
int32_t int_vector_front(const int_vector* vec);
int32_t int_vector_back(const int_vector* vec);
int32_t* int_vector_data(int_vector* vec);  // 获取底层原始指针

// 修改元素
int32_t int_vector_set(int_vector* vec, size_t index, int32_t value);
int32_t int_vector_push_back(int_vector* vec, int32_t value);
void int_vector_pop_back(int_vector* vec);
int32_t int_vector_insert(int_vector* vec, size_t index, int32_t value);
int32_t int_vector_erase(int_vector* vec, size_t index);
void int_vector_clear(int_vector* vec);

// 批量操作
int32_t int_vector_append(int_vector* vec, const int32_t* values, size_t count);
int32_t int_vector_insert_range(int_vector* vec, size_t index, const int32_t* values, size_t count);

// 查找与搜索
int32_t int_vector_find(const int_vector* vec, int32_t value);
int32_t int_vector_binary_search(const int_vector* vec, int32_t value);  // 二分查找（需有序）

// 其他工具函数
void int_vector_swap(int_vector* vec1, int_vector* vec2);
int32_t int_vector_resize(int_vector* vec, size_t new_size, int32_t default_value);
void int_vector_sort(int_vector* vec);  // 升序排序

#endif // INT_VECTOR_H