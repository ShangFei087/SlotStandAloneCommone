#include "ComputerData.h"

#include <stdarg.h>
#include <stdio.h>

int32_t JPWeight[4] = { 100, 60, 30, 10 };
//比如玩家游戏得分越高 / 投注越大，系统会选择更大的 JPDrawRateUnit 数值，让中奖概率按倍数提升（比如选 4 倍，则 “一份” 奖的概率从 100 / 万变成 400 / 万），是 “高投入高回报” 的核心逻辑。
int32_t JPDrawRateUnit[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
//中心奖池的抽取率配置，和 JPDrawRateUnit 一一对应，是最终中奖概率的 “绝对数值”（而非倍数）。
int32_t CenterJPDrawRate[8] = { 25, 50, 75, 100, 125, 150, 175, 200 };

// 嵌入式友好的轻量字符串拼接：避免依赖 vsnprintf 的库实现差异和较大体积。
 void append_raw(char* buffer, size_t buffer_size, size_t* used, const char* src)
{
	size_t i = 0;
	if (buffer == NULL || used == NULL || src == NULL || buffer_size == 0 || *used >= buffer_size) return;
	while (src[i] != '\0' && *used < (buffer_size - 1))
	{
		buffer[*used] = src[i];
		(*used)++;
		i++;
	}
	buffer[*used] = '\0';
}

 void append_char(char* buffer, size_t buffer_size, size_t* used, char c)
{
	if (buffer == NULL || used == NULL || buffer_size == 0 || *used >= (buffer_size - 1)) return;
	buffer[*used] = c;
	(*used)++;
	buffer[*used] = '\0';
}

 void append_uint64_dec(char* buffer, size_t buffer_size, size_t* used, uint64_t value)
{
	char tmp[21];
	int32_t idx = 0;
	if (value == 0)
	{
		append_char(buffer, buffer_size, used, '0');
		return;
	}
	while (value > 0 && idx < (int32_t)sizeof(tmp))
	{
		tmp[idx++] = (char)('0' + (value % 10));
		value /= 10;
	}
	while (idx > 0)
	{
		append_char(buffer, buffer_size, used, tmp[--idx]);
	}
}

 void append_int64_dec(char* buffer, size_t buffer_size, size_t* used, int64_t value)
{
	uint64_t abs_value = 0;
	if (value < 0)
	{
		append_char(buffer, buffer_size, used, '-');
		abs_value = (uint64_t)(-(value + 1)) + 1U;
	}
	else
	{
		abs_value = (uint64_t)value;
	}
	append_uint64_dec(buffer, buffer_size, used, abs_value);
}

 void append_format(char* buffer, size_t buffer_size, size_t* used, const char* format, ...)
{
	va_list args;
	size_t i = 0;

	if (buffer == NULL || used == NULL || format == NULL || buffer_size == 0) return;
	if (*used >= buffer_size)
	{
		*used = buffer_size - 1;
		buffer[*used] = '\0';
		return;
	}
	buffer[*used] = '\0';

	va_start(args, format);
	while (format[i] != '\0' && *used < (buffer_size - 1))
	{
		if (format[i] != '%')
		{
			append_char(buffer, buffer_size, used, format[i]);
			i++;
			continue;
		}

		i++;
		if (format[i] == '\0') break;

		if (format[i] == '%')
		{
			append_char(buffer, buffer_size, used, '%');
			i++;
			continue;
		}

		if (format[i] == 's')
		{
			const char* s = va_arg(args, const char*);
			append_raw(buffer, buffer_size, used, (s != NULL) ? s : "(null)");
			i++;
			continue;
		}

		if (format[i] == 'c')
		{
			int32_t c = va_arg(args, int32_t);
			append_char(buffer, buffer_size, used, (char)c);
			i++;
			continue;
		}

		if (format[i] == 'd' || format[i] == 'i')
		{
			int32_t v = va_arg(args, int32_t);
			append_int64_dec(buffer, buffer_size, used, (int64_t)v);
			i++;
			continue;
		}

		if (format[i] == 'u')
		{
			uint32_t v = va_arg(args, uint32_t);
			append_uint64_dec(buffer, buffer_size, used, (uint64_t)v);
			i++;
			continue;
		}

		if (format[i] == 'l' && format[i + 1] == 'l' && (format[i + 2] == 'd' || format[i + 2] == 'i'))
		{
			long long v = va_arg(args, long long);
			append_int64_dec(buffer, buffer_size, used, (int64_t)v);
			i += 3;
			continue;
		}

		// 默认分支：不识别的格式占位符，按原样输出
		append_char(buffer, buffer_size, used, '%');
		append_char(buffer, buffer_size, used, format[i]);
		i++;
	}
	va_end(args);
}