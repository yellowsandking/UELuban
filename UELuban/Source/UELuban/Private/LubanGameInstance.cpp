// Copyright Epic Games, Inc. All Rights Reserved.

#include "LubanGameInstance.h"
#include "LubanConfigTest.h"

ULubanGameInstance::ULubanGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ConfigTest(nullptr)
{
}

void ULubanGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("LubanGameInstance: Initializing..."));

	// 创建配置表测试类实例
	ConfigTest = ULubanConfigTest::Get();
	if (ConfigTest)
	{
		// 加载配置表
		if (ConfigTest->LoadConfigTables())
		{
			UE_LOG(LogTemp, Log, TEXT("LubanGameInstance: Config tables loaded successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LubanGameInstance: Failed to load config tables"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LubanGameInstance: Failed to create ConfigTest instance"));
	}
}

void ULubanGameInstance::Shutdown()
{
	// 清理资源
	if (ConfigTest)
	{
		ConfigTest = nullptr;
	}

	Super::Shutdown();
}

cfg::Tables* ULubanGameInstance::GetConfigTables() const
{
	if (ConfigTest)
	{
		return ConfigTest->GetTables();
	}
	return nullptr;
}

bool ULubanGameInstance::IsConfigLoaded() const
{
	if (ConfigTest)
	{
		return ConfigTest->IsConfigLoaded();
	}
	return false;
}

void ULubanGameInstance::PrintAllConfigTables()
{
	if (ConfigTest)
	{
		ConfigTest->PrintAllConfigTables();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LubanGameInstance: ConfigTest is null, cannot print config tables"));
	}
}
