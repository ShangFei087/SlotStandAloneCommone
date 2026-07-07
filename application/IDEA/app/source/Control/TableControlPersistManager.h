#ifndef _TABLE_CONTROL_PERSIST_MANAGER_H_
#define _TABLE_CONTROL_PERSIST_MANAGER_H_

#include "TableControl.h"

/*
 * 闸门运行时（TableControlRuntime）Flash 持久化
 *
 * 整块绑定 idea_database_connect，由 idea_database_exec 周期刷写到 Flash。
 * 断电重启后恢复：区域/档位、五池余额、注入余量、概率门覆盖、自适应偏移、闭环调参窗口。
 * TableControlStats 累计统计不参与持久化。
 */
#define TABLE_CONTROL_PERSIST_BLOB_MAGIC   ((uint32_t)0x54435042u) /* 'TCPB' 数据魔数 */
#define TABLE_CONTROL_PERSIST_VERSION      ((uint32_t)1u)          /* 结构变更时递增，不匹配则丢弃历史快照 */

/** Flash 镜像：magic + version + 运行时快照 */
typedef struct TableControlPersistManager {
	uint32_t blobMagic;              /* 整包有效标记 */
	uint32_t version;                /* 数据版本号 */
	TableControlRuntime runtime;     /* 闸门运行时快照（与 RAM 中 gTableControl.runtime 对应） */
} TableControlPersistManager;

extern TableControlPersistManager g_TableControlPersistBlob;

/** 重置 Flash 镜像为空白合法包（保留 magic/version，runtime 清零） */
void table_control_persist_reset_blob(void);
/** 校验 Flash 镜像 magic/version 是否与本版本兼容 */
int8_t table_control_persist_is_blob_valid(void);
/** 将当前 RAM 运行时写入镜像 */
void table_control_persist_pack(TableControlPersistManager* blob);
/** 将镜像运行时导入 RAM（内部经 TableControl_ImportRuntime 校验） */
void table_control_persist_unpack(const TableControlPersistManager* blob);

/**
 * @brief 初始化持久化：绑定 Flash、校验镜像、尝试恢复到 RAM。
 * @note 在 idea_app_init 中调用；_LocalDebug 模式下由调用方跳过。
 */
void TableControlPersist_Init(void);
/**
 * @brief 将当前 RAM 运行时同步到 Flash 镜像（等待 database_exec 刷写）。
 * @note 每局结束、切档、联网 JP 派彩、概率覆盖变更后调用。
 */
void TableControlPersist_Save(void);
/**
 * @brief 清空 Flash 镜像并将 RAM 运行时恢复为默认值后写回 Flash。
 */
void TableControlPersist_Clear(void);
/**
 * @brief 从 Flash 镜像恢复运行时到 RAM（启动时或 Init 校验通过后调用）。
 */
void TableControlPersist_TryRestore(void);

#endif /* _TABLE_CONTROL_PERSIST_MANAGER_H_ */
