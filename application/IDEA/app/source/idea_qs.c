#include "idea_qs.h"

void idea_database_exec(uint32_t millisecond) {
	qs_database_exec(millisecond);
}

void idea_database_init(void) {
	qs_database_init();
}

BOOL idea_database_connect(void* pData, size_t DataSize)
{
	return qs_database_connect(pData, DataSize);
}

void idea_task_init(void) {
	qs_task_init();
}

void idea_task_exec(uint32_t millisecond) {
	qs_task_exec(millisecond);
}

void idea_task_disconnect(void (*pfnTask)(uint32_t millisecond)) {
	qs_task_disconnect(pfnTask);
}

void idea_task_connect(void (*pfnTask)(uint32_t millisecond)) {
	qs_task_connect(pfnTask);
}