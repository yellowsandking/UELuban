# Luban GameInstance 使用说明

## 概述

`ULubanGameInstance` 是一个自定义的 GameInstance 类，在游戏启动时自动加载 Luban 配置表。

## 功能特性

- **自动初始化**：在游戏启动时（`Init()` 方法中）自动加载配置表
- **全局访问**：通过 GameInstance 可以在任何地方访问配置表
- **蓝图支持**：提供蓝图可调用的方法

## 设置步骤

### 步骤 1: 在项目设置中配置 GameInstance

1. 打开项目设置（**Edit -> Project Settings**）
2. 在左侧找到 **Game** 分类
3. 展开 **General** 部分
4. 找到 **Game Instance Class** 设置
5. 选择 `LubanGameInstance` 或创建蓝图继承自 `LubanGameInstance`

### 步骤 2: 确保配置文件存在

确保 Luban 生成的 `.bytes` 文件在以下目录：
```
Content/Luban/ConfigBytes/*.bytes
```

### 步骤 3: 编译并运行

1. 编译项目
2. 启动游戏（PIE 或 Standalone）
3. 配置表会在游戏启动时自动加载

## 使用方法

### 在 C++ 中访问配置表

```cpp
// 在任何地方获取 GameInstance
ULubanGameInstance* GameInstance = Cast<ULubanGameInstance>(GetGameInstance());
if (GameInstance && GameInstance->IsConfigLoaded())
{
    // 获取配置表实例
    cfg::Tables* Tables = GameInstance->GetConfigTables();
    
    if (Tables)
    {
        // 使用配置表
        // 例如：访问 TbAutoImport1 表
        auto Item = Tables->TbAutoImport1.get(1);
        if (Item)
        {
            UE_LOG(LogTemp, Log, TEXT("Item ID: %d, x10: %s"), 
                Item->id, UTF8_TO_TCHAR(Item->x10.c_str()));
        }
    }
}
```

### 在蓝图中访问配置表

1. **获取 GameInstance**
   - 使用 `Get Game Instance` 节点
   - 转换为 `Luban Game Instance` 类型

2. **检查配置是否已加载**
   - 调用 `Is Config Loaded` 节点

3. **打印配置表内容（调试用）**
   - 调用 `Print All Config Tables` 节点

### 在 Actor 中使用示例

```cpp
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 获取 GameInstance
    ULubanGameInstance* GameInstance = Cast<ULubanGameInstance>(GetGameInstance());
    if (GameInstance && GameInstance->IsConfigLoaded())
    {
        cfg::Tables* Tables = GameInstance->GetConfigTables();
        if (Tables)
        {
            // 使用配置表数据
            // ...
        }
    }
}
```

## API 参考

### 公共方法

#### `GetConfigTables()`
- **返回类型**：`cfg::Tables*`
- **说明**：获取配置表实例（C++ 访问）
- **使用**：用于直接访问配置表数据

#### `IsConfigLoaded()`
- **返回类型**：`bool`
- **说明**：检查配置表是否已加载
- **蓝图**：可调用

#### `PrintAllConfigTables()`
- **返回类型**：`void`
- **说明**：打印所有配置表内容到日志（用于调试）
- **蓝图**：可调用

## 初始化流程

1. **游戏启动** → `ULubanGameInstance::Init()` 被调用
2. **创建 ConfigTest 实例** → 创建 `ULubanConfigTest` 对象
3. **加载配置表** → 调用 `LoadConfigTables()`
4. **配置表可用** → 通过 `GetConfigTables()` 访问

## 日志输出

游戏启动时，会在 Output Log 中看到以下日志：

```
LogTemp: LubanGameInstance: Initializing...
LogTemp: LubanConfigTest: Config tables loaded successfully
LogTemp: LubanGameInstance: Config tables loaded successfully
```

如果加载失败：
```
LogTemp: LubanGameInstance: Initializing...
LogTemp: LubanConfigTest: Failed to load config tables
LogTemp: LubanGameInstance: Failed to load config tables
```

## 注意事项

1. **GameInstance 生命周期**
   - GameInstance 在游戏启动时创建，在整个游戏运行期间存在
   - 配置表只加载一次，在 `Init()` 中加载

2. **配置表路径**
   - 配置文件路径硬编码为 `Content\Luban\ConfigBytes\`
   - 确保所有 `.bytes` 文件都在此目录下

3. **错误处理**
   - 如果配置表加载失败，`IsConfigLoaded()` 会返回 `false`
   - 在使用配置表前，应该先检查 `IsConfigLoaded()`

4. **蓝图继承**
   - 可以创建蓝图继承自 `LubanGameInstance`
   - 在蓝图中可以添加额外的初始化逻辑

## 故障排除

### 问题：配置表未加载

**检查：**
- 确认在项目设置中已设置 GameInstance 为 `LubanGameInstance`
- 检查 `Content/Luban/ConfigBytes/` 目录下是否有 `.bytes` 文件
- 查看 Output Log 中的错误信息

### 问题：无法访问配置表

**检查：**
- 确认 `IsConfigLoaded()` 返回 `true`
- 确认 `GetConfigTables()` 不为 `nullptr`
- 检查配置表文件名是否正确

## 示例：在 Actor 中使用配置表

```cpp
UCLASS()
class UELUBAN_API AMyGameActor : public AActor
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        
        // 获取配置表
        ULubanGameInstance* GameInstance = Cast<ULubanGameInstance>(GetGameInstance());
        if (GameInstance && GameInstance->IsConfigLoaded())
        {
            cfg::Tables* Tables = GameInstance->GetConfigTables();
            if (Tables)
            {
                // 访问 TbAutoImport1 表
                auto Item = Tables->TbAutoImport1.get(1);
                if (Item)
                {
                    UE_LOG(LogTemp, Log, TEXT("Found item: ID=%d, x10=%s"), 
                        Item->id, UTF8_TO_TCHAR(Item->x10.c_str()));
                }
                
                // 遍历所有数据
                const auto& DataList = Tables->TbAutoImport1.getDataList();
                for (const auto& Data : DataList)
                {
                    if (Data)
                    {
                        UE_LOG(LogTemp, Log, TEXT("Item ID: %d"), Data->id);
                    }
                }
            }
        }
    }
};
```

## 相关文件

- `Source/UELuban/Public/LubanGameInstance.h` - GameInstance 头文件
- `Source/UELuban/Private/LubanGameInstance.cpp` - GameInstance 实现
- `Source/UELuban/Public/Tests/LubanConfigTest.h` - 配置表测试类
- `Source/UELuban/Private/Tests/LubanConfigTest.cpp` - 配置表测试类实现
