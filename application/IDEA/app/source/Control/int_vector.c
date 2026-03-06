#include "int_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ==================== 内存管理 ====================
//从数组中创建vector
int_vector* int_vector_from_array(const int* arr, size_t count) {
    if (!arr || count == 0) return NULL;

    int_vector* vec = int_vector_create(count);
    if (!vec) return NULL;

    if (!int_vector_append(vec, arr, count)) {
        int_vector_destroy(vec);
        return NULL;
    }

    return vec;
}
int_vector* int_vector_create(size_t initial_capacity) {
    int_vector* vec = (int_vector*)malloc(sizeof(int_vector));
    if (!vec) return NULL;

    vec->capacity = initial_capacity > 0 ? initial_capacity : 1;
    vec->size = 0;
    vec->data = (int32_t*)malloc(sizeof(int32_t) * vec->capacity);

    if (!vec->data) {
        free(vec);
        return NULL;
    }

    return vec;
}

void int_vector_destroy(int_vector* vec) {
    if (!vec) return;
    free(vec->data);
    free(vec);
}

// ==================== 内部辅助函数 ====================

int32_t int_vector_internal_resize(int_vector* vec, size_t new_capacity) {
    if (new_capacity == 0) return 0;

    int32_t* new_data = (int32_t*)realloc(vec->data, sizeof(int32_t) * new_capacity);
    if (!new_data) return 0;

    vec->data = new_data;
    vec->capacity = new_capacity;

    // 如果新容量小于当前大小，调整大小
    if (new_capacity < vec->size) {
        vec->size = new_capacity;
    }

    return 1;
}

int32_t int_vector_ensure_capacity(int_vector* vec, size_t min_capacity) {
    if (vec->capacity >= min_capacity) return 1;

    // 使用倍增策略
    size_t new_capacity = vec->capacity * 2;
    if (new_capacity < min_capacity) {
        new_capacity = min_capacity;
    }

    return int_vector_internal_resize(vec, new_capacity);
}

// ==================== 容量管理 ====================

int32_t int_vector_reserve(int_vector* vec, size_t new_capacity) {
    if (!vec) return 0;
    if (new_capacity <= vec->capacity) return 1;
    return int_vector_internal_resize(vec, new_capacity);
}

int32_t int_vector_shrink_to_fit(int_vector* vec) {
    if (!vec) return 0;
    if (vec->size == vec->capacity) return 1;
    return int_vector_internal_resize(vec, vec->size);
}

size_t int_vector_capacity(const int_vector* vec) {
    return vec ? vec->capacity : 0;
}

size_t int_vector_size(const int_vector* vec) {
    return vec ? vec->size : 0;
}

int32_t int_vector_empty(const int_vector* vec) {
    return vec ? (vec->size == 0) : 1;
}

// ==================== 元素访问 ====================

int32_t int_vector_at(const int_vector* vec, size_t index, int32_t* out_value) {
    if (!vec || !out_value || index >= vec->size) return 0;
    *out_value = vec->data[index];
    return 1;
}

int32_t int_vector_get(const int_vector* vec, size_t index) {
   
    return vec->data[index];
}

int32_t int_vector_front(const int_vector* vec) {
  
    return vec->data[0];
}

int32_t int_vector_back(const int_vector* vec) {
  
    return vec->data[vec->size - 1];
}

int32_t* int_vector_data(int_vector* vec) {
    return vec ? vec->data : NULL;
}

// ==================== 修改元素 ====================

int32_t int_vector_set(int_vector* vec, size_t index, int32_t value) {
    if (!vec || index >= vec->size) return 0;
    vec->data[index] = value;
    return 1;
}

int32_t int_vector_push_back(int_vector* vec, int32_t value) {
    if (!vec) return 0;

    if (!int_vector_ensure_capacity(vec, vec->size + 1)) {
        return 0;
    }

    vec->data[vec->size] = value;
    vec->size++;
    return 1;
}

void int_vector_pop_back(int_vector* vec) {
    if (vec && vec->size > 0) {
        vec->size--;
    }
}

int32_t int_vector_insert(int_vector* vec, size_t index, int32_t value) {
    if (!vec || index > vec->size) return 0;

    if (!int_vector_ensure_capacity(vec, vec->size + 1)) {
        return 0;
    }

    // 移动元素腾出位置
    if (index < vec->size) {
        memmove(&vec->data[index + 1], &vec->data[index],
            (vec->size - index) * sizeof(int32_t));
    }

    vec->data[index] = value;
    vec->size++;
    return 1;
}

int32_t int_vector_erase(int_vector* vec, size_t index) {
    if (!vec || index >= vec->size) return 0;

    // 移动元素覆盖要删除的元素
    if (index < vec->size - 1) {
        memmove(&vec->data[index], &vec->data[index + 1],
            (vec->size - index - 1) * sizeof(int32_t));
    }

    vec->size--;
    return 1;
}

void int_vector_clear(int_vector* vec) {
    if (vec) {
        vec->size = 0;
    }
}

// ==================== 批量操作 ====================

int32_t int_vector_append(int_vector* vec, const int32_t* values, size_t count) {
    if (!vec || !values || count == 0) return 0;

    if (!int_vector_ensure_capacity(vec, vec->size + count)) {
        return 0;
    }

    memcpy(&vec->data[vec->size], values, count * sizeof(int32_t));
    vec->size += count;
    return 1;
}

int32_t int_vector_insert_range(int_vector* vec, size_t index, const int32_t* values, size_t count) {
    if (!vec || !values || index > vec->size || count == 0) return 0;

    if (!int_vector_ensure_capacity(vec, vec->size + count)) {
        return 0;
    }

    // 移动现有元素
    if (index < vec->size) {
        memmove(&vec->data[index + count], &vec->data[index],
            (vec->size - index) * sizeof(int32_t));
    }

    // 插入新元素
    memcpy(&vec->data[index], values, count * sizeof(int32_t));
    vec->size += count;
    return 1;
}

// ==================== 查找操作 ====================

int32_t int_vector_find(const int_vector* vec, int32_t value) {
    if (!vec) return -1;

    for (size_t i = 0; i < vec->size; i++) {
        if (vec->data[i] == value) {
            return (int32_t)i;
        }
    }
    return -1;
}

int32_t int_vector_binary_search(const int_vector* vec, int32_t value) {
    if (!vec) return -1;

    int32_t left = 0;
    int32_t right = (int32_t)vec->size - 1;

    while (left <= right) {
        int32_t mid = left + (right - left) / 2;
        int32_t mid_val = vec->data[mid];

        if (mid_val == value) {
            return mid;
        }
        else if (mid_val < value) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

// ==================== 工具函数 ====================

void int_vector_swap(int_vector* vec1, int_vector* vec2) {
    if (!vec1 || !vec2) return;

    // 交换数据指针
    int32_t* temp_data = vec1->data;
    vec1->data = vec2->data;
    vec2->data = temp_data;

    // 交换大小
    size_t temp_size = vec1->size;
    vec1->size = vec2->size;
    vec2->size = temp_size;

    // 交换容量
    size_t temp_capacity = vec1->capacity;
    vec1->capacity = vec2->capacity;
    vec2->capacity = temp_capacity;
}

int32_t int_vector_resize(int_vector* vec, size_t new_size, int32_t default_value) {
    if (!vec) return 0;

    if (new_size > vec->size) {
        // 需要扩容
        if (!int_vector_ensure_capacity(vec, new_size)) {
            return 0;
        }

        // 填充新元素
        for (size_t i = vec->size; i < new_size; i++) {
            vec->data[i] = default_value;
        }
    }

    vec->size = new_size;
    return 1;
}

// 快速排序的辅助函数
void int_vector_quick_sort(int32_t* arr, int32_t left, int32_t right) {
    if (left >= right) return;

    int32_t pivot = arr[(left + right) / 2];
    int32_t i = left, j = right;

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            int32_t temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

    int_vector_quick_sort(arr, left, j);
    int_vector_quick_sort(arr, i, right);
}

void int_vector_sort(int_vector* vec) {
    if (!vec || vec->size < 2) return;
    int_vector_quick_sort(vec->data, 0, (int32_t)vec->size - 1);
}