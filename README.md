# QML Map Editor

[中文](README.md) | [English](README.en.md)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

一个基于 Qt Quick 和 QtLocation 的地图编辑器，支持在地图上绘制和编辑多种几何图形。

![截图](assets/qml-map-editor.png)

## 功能特性

- **标记点 (Marker)** - 在地图上添加标记点，支持拖动和逆地理编码显示地址
- **折线 (LineString)** - 绘制多点连接的折线，支持编辑各个顶点
- **多边形 (Polygon)** - 绘制闭合的多边形区域，支持编辑各个顶点
- **圆形 (Circle)** - 绘制圆形区域，支持拖动边缘调整半径
- **正方形 (Square)** - 绘制正方形区域，支持拖动边缘调整边长

## 编辑功能

- 🖱️ **双击左键** - 添加新的几何图形或顶点
- 🖱️ **右键单击** - 结束当前编辑（闭合图形）
- 🖱️ **Ctrl+点击** - 多选几何图形
- 🔒 **锁定模式** - 锁定编辑，防止误操作
- 🗑️ **删除** - 删除选中的几何图形或顶点
- 🧹 **清除** - 清除所有几何图形

## 项目结构

```
qml-map-editor/
├── src/
│   ├── main.cc                    # 程序入口
│   ├── App.qml                    # 主窗口
│   ├── EditorMap.qml              # 地图组件（含逆地理编码）
│   ├── MapEditorLayer.qml         # 编辑图层
│   ├── MapEditorToolBar.qml       # 编辑模式工具栏
│   ├── MapEditorActionBar.qml     # 操作工具栏
│   └── components/                # 组件库
│       ├── mapeditorcontroller.*  # 编辑控制器
│       ├── mapgeometry.*          # 几何基类
│       ├── mappoint.*             # 点
│       ├── maplinestring.*        # 折线
│       ├── mappolygon.*           # 多边形
│       ├── mapcircle.*            # 圆
│       ├── mapsquare.*            # 正方形
│       └── Map*Item.qml           # 对应的 QML 可视化组件
├── assets/                        # 图标资源
├── cmake/                         # CMake 模块
└── CMakeLists.txt                 # 项目配置
```

## 许可证

本项目采用 [MIT License](LICENSE.md) 许可证。