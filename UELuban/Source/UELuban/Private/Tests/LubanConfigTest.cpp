// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tests/LubanConfigTest.h"
#include "schema.h"
#include "ByteBuf.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

ULubanConfigTest* ULubanConfigTest::Instance = nullptr;

ULubanConfigTest::ULubanConfigTest()
	: bIsLoaded(false)
	, TablesInstance(nullptr)
{
}

ULubanConfigTest::~ULubanConfigTest()
{
	if (TablesInstance)
	{
		delete TablesInstance;
		TablesInstance = nullptr;
	}
}

bool ULubanConfigTest::LoadConfigTables()
{
	if (bIsLoaded && TablesInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("LubanConfigTest: Config tables already loaded"));
		return true;
	}

	// 创建 Tables 实例
	if (!TablesInstance)
	{
		TablesInstance = new cfg::Tables();
	}

	// 创建加载器函数
	// 参考 App.cpp 的实现：
	// tables.load([](::luban::ByteBuf& buf, const std::string& s) {
	//     return buf.loadFromFile("../GenerateDatas/bytes/" + s + ".bytes");
	// });
	auto loader = [](::luban::ByteBuf& buf, const std::string& fileName) -> bool
		{
			return ULubanConfigTest::CreateFileLoader(buf, fileName);
		};

	// 加载所有配置表
	if (TablesInstance->load(loader))
	{
		bIsLoaded = true;
		UE_LOG(LogTemp, Log, TEXT("LubanConfigTest: Config tables loaded successfully"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LubanConfigTest: Failed to load config tables"));
		bIsLoaded = false;
		return false;
	}
}



bool ULubanConfigTest::CreateFileLoader(::luban::ByteBuf& buf, const std::string& fileName)
{
	// 构建文件路径：Content\Luban\ConfigBytes\{fileName}.bytes
	FString FileNameUE = UTF8_TO_TCHAR(fileName.c_str());
	FString FilePath = FPaths::ProjectContentDir() / TEXT("Luban") / TEXT("ConfigBytes") / (FileNameUE + TEXT(".bytes"));
	FilePath = FPaths::ConvertRelativePathToFull(FilePath);

	// 检查文件是否存在
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("LubanConfigTest: File not found: %s"), *FilePath);
		return false;
	}

	// 读取文件内容
	TArray<uint8> FileBytes;
	if (!FFileHelper::LoadFileToArray(FileBytes, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("LubanConfigTest: Failed to read file: %s"), *FilePath);
		return false;
	}

	// 将数据加载到 ByteBuf
	// ByteBuf 的 loadFromFile 需要 std::string 路径
	// 注意：ByteBuf::loadFromFile 使用标准 C++ 文件流，需要绝对路径
	std::string FilePathStd = TCHAR_TO_UTF8(*FilePath);

	// 清空 ByteBuf
	buf.clear();

	// 使用 loadFromFile 方法加载文件
	// 参考 App.cpp: return buf.loadFromFile("../GenerateDatas/bytes/" + s + ".bytes");
	if (!buf.loadFromFile(FilePathStd))
	{
		UE_LOG(LogTemp, Error, TEXT("LubanConfigTest: ByteBuf.loadFromFile failed for: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("LubanConfigTest: Loaded file successfully: %s (%d bytes)"), *FilePath, FileBytes.Num());
	return true;
}

ULubanConfigTest* ULubanConfigTest::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<ULubanConfigTest>();
		Instance->AddToRoot(); // 防止被垃圾回收
	}
	return Instance;
}

void ULubanConfigTest::PrintAllConfigTables()
{
	if (!TablesInstance || !bIsLoaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("LubanConfigTest: Config tables not loaded. Please call LoadConfigTables() first."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("========== Luban Config Tables Content =========="));

	// 打印 TbPreLoadRes 表
	UE_LOG(LogTemp, Log, TEXT("--- TbPreLoadRes Table ---"));
	const auto& PreLoadResList = TablesInstance->TbPreLoadRes.getDataList();
	UE_LOG(LogTemp, Log, TEXT("  Total count: %d"), PreLoadResList.size());
	for (size_t i = 0; i < PreLoadResList.size(); ++i)
	{
		const auto& item = PreLoadResList[i];
		if (item)
		{
			UE_LOG(LogTemp, Log, TEXT("  [%d] ID: %d, ResName: %s, FileType: %s, CreatAmount: %d"),
				i,
				item->id,
				UTF8_TO_TCHAR(item->ResName.c_str()),
				UTF8_TO_TCHAR(item->FileType.c_str()),
				item->CreatAmount
			);
		}
	}

	// 打印 TbAutoImport1 表
	UE_LOG(LogTemp, Log, TEXT("--- TbAutoImport1 Table ---"));
	const auto& AutoImport1List = TablesInstance->TbAutoImport1.getDataList();
	UE_LOG(LogTemp, Log, TEXT("  Total count: %d"), AutoImport1List.size());
	for (size_t i = 0; i < AutoImport1List.size(); ++i)
	{
		const auto& item = AutoImport1List[i];
		if (item)
		{
			UE_LOG(LogTemp, Log, TEXT("  [%d] ID: %d, x1: %s, x5: %lld, x6: %f, x8: %d, x10: %s"),
				i,
				item->id,
				item->x1 ? TEXT("true") : TEXT("false"),
				item->x5,
				item->x6,
				item->x8,
				UTF8_TO_TCHAR(item->x10.c_str())
			);

			// 打印数组 k1
			if (!item->k1.empty())
			{
				FString K1Str = TEXT("    k1: [");
				for (size_t j = 0; j < item->k1.size(); ++j)
				{
					if (j > 0) K1Str += TEXT(", ");
					K1Str += FString::Printf(TEXT("%d"), item->k1[j]);
				}
				K1Str += TEXT("]");
				UE_LOG(LogTemp, Log, TEXT("%s"), *K1Str);
			}

			// 打印数组 k2
			if (!item->k2.empty())
			{
				FString K2Str = TEXT("    k2: [");
				for (size_t j = 0; j < item->k2.size(); ++j)
				{
					if (j > 0) K2Str += TEXT(", ");
					K2Str += FString::Printf(TEXT("%d"), item->k2[j]);
				}
				K2Str += TEXT("]");
				UE_LOG(LogTemp, Log, TEXT("%s"), *K2Str);
			}

			// 打印 HashMap k8
			if (!item->k8.empty())
			{
				FString K8Str = TEXT("    k8: {");
				bool first = true;
				for (const auto& pair : item->k8)
				{
					if (!first) K8Str += TEXT(", ");
					K8Str += FString::Printf(TEXT("%d: %d"), pair.first, pair.second);
					first = false;
				}
				K8Str += TEXT("}");
				UE_LOG(LogTemp, Log, TEXT("%s"), *K8Str);
			}

			// 打印 Vector k10
			if (!item->k10.empty())
			{
				FString K10Str = TEXT("    k10: [");
				for (size_t j = 0; j < item->k10.size(); ++j)
				{
					if (j > 0) K10Str += TEXT(", ");
					if (item->k10[j])
					{
						K10Str += FString::Printf(TEXT("vec3(%.2f, %.2f, %.2f)"),
							item->k10[j]->x, item->k10[j]->y, item->k10[j]->z);
					}
					else
					{
						K10Str += TEXT("null");
					}
				}
				K10Str += TEXT("]");
				UE_LOG(LogTemp, Log, TEXT("%s"), *K10Str);
			}

			// 打印 Vector k11
			if (!item->k11.empty())
			{
				FString K11Str = TEXT("    k11: [");
				for (size_t j = 0; j < item->k11.size(); ++j)
				{
					if (j > 0) K11Str += TEXT(", ");
					if (item->k11[j])
					{
						K11Str += FString::Printf(TEXT("vec4(%.2f, %.2f, %.2f, %.2f)"),
							item->k11[j]->x, item->k11[j]->y, item->k11[j]->z, item->k11[j]->w);
					}
					else
					{
						K11Str += TEXT("null");
					}
				}
				K11Str += TEXT("]");
				UE_LOG(LogTemp, Log, TEXT("%s"), *K11Str);
			}

			// 打印 v11 (可选字段)
			if (item->v11)
			{
				UE_LOG(LogTemp, Log, TEXT("    v11: vec3(%.2f, %.2f, %.2f)"),
					item->v11->x, item->v11->y, item->v11->z);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("    v11: null"));
			}

			// 打印 v2
			if (item->v2)
			{
				UE_LOG(LogTemp, Log, TEXT("    v2: vec2(%.2f, %.2f)"), item->v2->x, item->v2->y);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("    v2: null"));
			}

			// 打印时间戳 t1
			UE_LOG(LogTemp, Log, TEXT("    t1: %lld"), item->t1);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("========== End of Config Tables Content =========="));
}
