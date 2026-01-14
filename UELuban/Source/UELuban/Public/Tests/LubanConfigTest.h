// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ByteBuf.h"
#include "LubanConfigTest.generated.h"

// 前向声明
namespace cfg
{
	class Tables;
}

/**
 * Luban 配置表测试类
 * 提供蓝图方法，调用 App.cpp 的方式来读取配置表
 * 配置表路径为 Content\Luban\ConfigBytes 下面所有的 .bytes 文件
 */
UCLASS(BlueprintType)
class UELUBAN_API ULubanConfigTest : public UObject
{
	GENERATED_BODY()

public:
	ULubanConfigTest();
	virtual ~ULubanConfigTest();

	/**
	 * 加载配置表（蓝图可调用）
	 * 参考 App.cpp 的实现方式：
	 *   cfg::Tables tables;
	 *   tables.load([](::luban::ByteBuf& buf, const std::string& s) {
	 *       return buf.loadFromFile("../GenerateDatas/bytes/" + s + ".bytes");
	 *   });
	 *
	 * @return 是否加载成功
	 */
	UFUNCTION(BlueprintCallable, Category = "Luban Config Test")
	bool LoadConfigTables();

	/**
	 * 检查配置表是否已加载
	 */
	UFUNCTION(BlueprintPure, Category = "Luban Config Test")
	bool IsConfigLoaded() const { return bIsLoaded; }

	/**
	 * 获取配置表实例（用于 C++ 访问）
	 */
	cfg::Tables* GetTables() const { return TablesInstance; }

	/**
	 * 获取测试类单例
	 */
	UFUNCTION(BlueprintCallable, Category = "Luban Config Test")
	static ULubanConfigTest* Get();

	/**
	 * 遍历打印配置表内容（蓝图可调用）
	 * 打印所有配置表中的数据到日志
	 */
	UFUNCTION(BlueprintCallable, Category = "Luban Config Test")
	void PrintAllConfigTables();

protected:
	/** 配置表是否已加载 */
	UPROPERTY()
	bool bIsLoaded;

	/** Tables 实例 */
	cfg::Tables* TablesInstance;

private:
	/** 单例实例 */
	static ULubanConfigTest* Instance;

	/**
	 * 创建文件加载器函数
	 * 从 Content\Luban\ConfigBytes 目录加载 .bytes 文件
	 */
	static bool CreateFileLoader(::luban::ByteBuf& buf, const std::string& fileName);
};
