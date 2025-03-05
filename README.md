# Clipboard Files Module

## 功能
跨平台Node.js扩展模块，用于获取系统剪贴板中的文件路径（支持macOS/Windows/Linux）

## 安装
```bash
npm install
```

## 使用示例
```javascript
const { getClipboardFiles } = require('./build/Release/clipboard');

try {
  const files = getClipboardFiles();
  console.log('剪贴板文件路径:', files);
} catch (err) {
  console.error('错误:', err);
}
```

## API
### getClipboardFiles()
- 返回: 字符串数组 - 剪贴板中的文件绝对路径
- 异常: 当剪贴板无文件或访问失败时抛出错误

## 开发要求
- macOS: Xcode命令行工具
- Windows: Python 3.x + Visual Studio Build Tools
- Linux: X11开发库

## 构建
```bash
node-gyp rebuild
```

## 测试
```bash
npm test
```

## 该项目使用Trae AI开发，目前只在在MacOs上测试成功