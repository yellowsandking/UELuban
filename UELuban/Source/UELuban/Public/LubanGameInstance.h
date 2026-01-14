// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LubanConfigTest.h"
#include "LubanGameInstance.generated.h"

// 前向声明
namespace cfg
{
	class Tables;
}

/**
 * Luban GameInstance
 * 在游戏启动时自动加载配置表
 */
UCLASS(BlueprintType)
class UELUBAN_API ULubanGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULubanGameInstance(const FObjectInitializer& ObjectInitializer);

	/**
	 * 游戏实例初始化
	 * 在这里加载配置表
	 */
	virtual void Init() override;

	/**
	 * 游戏实例关闭
	 */
	virtual void Shutdown() override;

	/**
	 * 获取配置表实例（C++ 访问）
	 */
	cfg::Tables* GetConfigTables() const;

	/**
	 * 检查配置表是否已加载
	 */
	UFUNCTION(BlueprintPure, Category = "Luban Config")
	bool IsConfigLoaded() const;

	/**
	 * 打印所有配置表内容（用于调试）
	 */
	UFUNCTION(BlueprintCallable, Category = "Luban Config")
	void PrintAllConfigTables();

protected:
	/** 配置表测试类实例 */
	UPROPERTY()
	ULubanConfigTest* ConfigTest;
};
