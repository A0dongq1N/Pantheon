# C/C++ 语法知识点

## 可变参数（Variadic Arguments）

### 概述
在C/C++中，`...` 是**可变参数（variadic arguments）**语法，表示函数可以接受任意数量的额外参数。这个特性从C语言继承而来，在C++中完全支持。

### 语法示例
```c
extern int scanf (const char *__restrict __format, ...) __wur;
extern int printf (const char *__restrict __format, ...) __wur;
```

### 实际使用
```c
// scanf 的典型用法
int a, b;
char str[100];
scanf("%d %d %s", &a, &b, str);  // 3个额外参数
scanf("%d", &a);                 // 1个额外参数
scanf("%d %d %d %d", &a, &b, &c, &d);  // 4个额外参数

// printf 的典型用法
printf("Hello %s, you are %d years old\n", name, age);  // 2个参数
printf("Number: %d\n", num);                            // 1个参数
printf("Hello world\n");                                // 0个额外参数
```

### 实现可变参数函数
```c
#include <stdarg.h>

void my_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);  // 初始化参数列表
    
    // 处理可变参数
    int value = va_arg(args, int);  // 获取下一个int参数
    
    va_end(args);  // 清理
}
```

### 相关宏和函数
- `va_list` - 可变参数列表类型
- `va_start(ap, last)` - 初始化参数列表
- `va_arg(ap, type)` - 获取下一个指定类型的参数
- `va_end(ap)` - 清理参数列表

### 注意事项
1. **类型安全问题**：编译器无法检查参数类型是否匹配
2. **运行时错误**：如果格式字符串与参数不匹配，会导致未定义行为
3. **参数提升**：char和short会被提升为int，float会被提升为double

### 现代C++替代方案

#### C++11 可变模板参数
```cpp
template<typename... Args>
void modern_printf(const std::string& format, Args... args) {
    // 更安全的实现，编译时类型检查
}
```

#### C++20 std::format
```cpp
#include <format>
std::string result = std::format("Hello {}, you are {} years old", name, age);
```

### 常见的可变参数函数
- `printf` 系列：printf, sprintf, snprintf, fprintf
- `scanf` 系列：scanf, sscanf, fscanf
- `execl` 系列：execl, execlp

### 最佳实践
1. 优先考虑使用现代C++的模板可变参数
2. 如果必须使用传统可变参数，确保格式字符串与参数匹配
3. 考虑使用类型安全的替代方案，如std::format
4. 在函数文档中明确说明参数要求和格式
