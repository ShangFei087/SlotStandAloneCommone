#include "TableControlPersistManager.h"
#include "RtpConfig.h"
#include "idea_qs.h"
#include "Test.h"
#include <string.h>

/* 绑定 Flash 的全局镜像，与 gTableControl.runtime 互为 pack/unpack 关系 */
TableControlPersistManager g_TableControlPersistBlob = { 0 };

void table_control_persist_reset_blob(void)
{
	memset(&g_TableControlPersistBlob, 0, sizeof(g_TableControlPersistBlob));
	g_TableControlPersistBlob.blobMagic = TABLE_CONTROL_PERSIST_BLOB_MAGIC;
	g_TableControlPersistBlob.version = TABLE_CONTROL_PERSIST_VERSION;
}

int8_t table_control_persist_is_blob_valid(void)
{
	return (g_TableControlPersistBlob.blobMagic == TABLE_CONTROL_PERSIST_BLOB_MAGIC &&
		g_TableControlPersistBlob.version == TABLE_CONTROL_PERSIST_VERSION) ? 1 : 0;
}

void table_control_persist_pack(TableControlPersistManager* blob)
{
	if (blob == NULL) return;
	TableControl_ExportRuntime(&blob->runtime);
}

void table_control_persist_unpack(const TableControlPersistManager* blob)
{
	if (blob == NULL) return;
	TableControl_ImportRuntime(&blob->runtime);
}

void TableControlPersist_Init(void)
{
	/* 注册 Flash 存储区：后续对 g_TableControlPersistBlob 的修改会由 database_exec 落盘 */
	(void)idea_database_connect(&g_TableControlPersistBlob, sizeof(g_TableControlPersistBlob));
	if (!table_control_persist_is_blob_valid())
	{
		/* 首次启动或版本升级：空白镜像 + 产品默认国内难度 3（仅此次，之后由 Flash 恢复） */
		table_control_persist_reset_blob();
		TableControl_ResetRuntimeDefaults();
		(void)TableControl_SetDifficultyLevel(3);
		QS_LOG("\r\n [TC] runtime persist blob init/reinit version=%u defaultLv=3", (unsigned)TABLE_CONTROL_PERSIST_VERSION);
		return;
	}

	/* 镜像合法：恢复到 RAM，使池子/窗口/档位与断电前一致 */
	TableControlPersist_TryRestore();
}

void TableControlPersist_Save(void)
{
	if (!table_control_persist_is_blob_valid())
	{
		table_control_persist_reset_blob();
	}
	table_control_persist_pack(&g_TableControlPersistBlob);
}

void TableControlPersist_Clear(void)
{
	table_control_persist_reset_blob();
	TableControl_ResetRuntimeDefaults();
	TableControlPersist_Save();
}

void TableControlPersist_TryRestore(void)
{
#ifdef _LocalDebug
	/* 本地调试不写 Flash，也不从 Flash 恢复 */
	return;
#else
	if (!table_control_persist_is_blob_valid()) return;
	table_control_persist_unpack(&g_TableControlPersistBlob);
#endif
}
