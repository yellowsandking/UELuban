# Luban 配置表二进制文件目录

此目录用于存放 Luban 生成的二进制配置数据文件（.bytes 文件）。

## 文件路径

配置表文件应放在：
```
Content/Luban/ConfigBytes/*.bytes
```

## 文件命名规则

Luban 生成的二进制文件命名规则通常为：
- 格式：`{namespace}_{tablename}.bytes` 或 `{tablename}.bytes`
- 示例：
  - `global_tbpreloadres.bytes`
  - `tbautoimport1.bytes`

文件名必须与 Luban 生成的 schema.h 中 Tables::load() 方法期望的文件名一致。

## 使用方法

### 蓝图中使用

1. 在蓝图中调用 `Luban Config Test Get` 节点获取测试类实例
2. 调用 `Load Config Tables` 节点加载配置表
3. 调用 `Is Config Loaded` 节点检查加载状态

### C++ 中使用

```cpp
// 获取测试类实例
ULubanConfigTest* Test = ULubanConfigTest::Get();

// 加载配置表
if (Test->LoadConfigTables())
{
    // 获取 Tables 实例
    cfg::Tables* Tables = Test->GetTables();
    
    // 使用配置表
    // 例如：Tables->TbAutoImport1.get(1);
}
```

## 参考实现

参考 `Cpp_sharedptr_bin/App.cpp` 的实现方式：

```cpp
cfg::Tables tables;
if (tables.load([](::luban::ByteBuf& buf, const std::string& s) {
    return buf.loadFromFile("../GenerateDatas/bytes/" + s + ".bytes");
}))
{
    std::cout << "== load succ == " << std::endl;
}
```

在 UE 中的对应实现已在 `ULubanConfigTest::LoadConfigTables()` 中完成。

## 注意事项

- 确保所有 `.bytes` 文件都放在此目录下
- 文件名必须与 Luban 生成的文件名一致
- 文件路径在代码中硬编码为 `Content\Luban\ConfigBytes\`
