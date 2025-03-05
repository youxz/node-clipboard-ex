const { getClipboardFiles } = require('./build/Release/clipboard');

console.log('当前平台:', process.platform);
try {
  const files = getClipboardFiles();
  console.log('剪贴板中的文件路径：', files);
} catch (err) {
  console.error('获取剪贴板文件失败：', err);
}

// 测试前请先复制文件到剪贴板
// 运行命令: npm test