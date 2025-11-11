# Zygisk-SGameHook

王者正式服/体验服自定义名片 体验服自定义海报 QQ群：746855036

## 功能说明
- 正式服/体验服自定义游戏名片（jpg格式）
- 体验服自定义游戏海报（png格式）
- 通过 Magisk Zygisk 模块实现

## 文件说明
在游戏目录 `/data/data/gamepackage/files/` 下：
- `customtexjpg` - 自定义名片（jpg格式）
- `customtexpng` - 自定义海报（png格式）

## 安装说明
1. 安装 Magisk v24 或更高版本并启用 Zygisk
2. 在 Magisk 中安装本模块
3. 启动游戏即可生效

## 编译方法
```bash
# 使用 Android Studio 编译
./gradlew :module:assembleRelease

# 编译后的 zip 包将生成在 `out` 文件夹下
```

## 参考项目
本项目参考自 [Zygisk-Il2CppDumper](https://github.com/Perfare/Zygisk-Il2CppDumper/)