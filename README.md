# Zygisk-SGameHook

王者正式服/体验服自定义名片 体验服自定义海报 QQ群：746855036

## 功能说明
- 正式服/体验服自定义游戏名片（jpg格式）
- 体验服自定义游戏海报（png格式）
- 通过 Zygisk 模块实现
- 可通过创建文件关闭模块对于部分游戏开启状态：
  在 /data/adb/modules/zygisk_sgamehook/ 目录下创建 skipzsf 文件可关闭模块对于正式服开启状态
  创建 skiptyf 文件可关闭模块对于体验服开启状态
  所有设置重启游戏后生效

## 文件说明
在游戏目录 `/data/data/gamepackage/files/` 下：
- `customtexjpg` - 自定义名片（jpg格式）
- `customtexpng` - 自定义海报（png格式）
- 在放置自定义名片或自定义海报文件后，请手动将对应文件的权限设置为777，以确保游戏能够正常读取文件。否则会出现文件无法读取的情况。

## 安装说明
1. 安装 Magisk v24 或更高版本并启用 Zygisk 或使用Zygisk Next
2. 安装本模块
3. 启动游戏即可生效

## 编译方法
```bash
# 使用 Android Studio 编译
./gradlew :module:assembleRelease

# 编译后的 zip 包将生成在 `out` 文件夹下
```

## 参考项目
本项目参考自 [Zygisk-Il2CppDumper](https://github.com/Perfare/Zygisk-Il2CppDumper/)

## 其他说明
1. 如已安装 Xposed 模块且非模拟器推荐使用 Xposed 模块 [Xposed-SGameHook](https://github.com/huajiqaq/Xposed-SGameHook/)
2. 如果想脱离 Zygisk 单独加载使用，可参考 [frida_android_x86_64_so_loader](https://github.com/huajiqaq/frida_android_x86_64_so_loader/)