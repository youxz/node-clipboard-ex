const { readFilePaths } = require('./index.js');


console.log('当前平台:', process.platform);
try {
  const files = readFilePaths();
  console.log('剪贴板中的文件路径：', files);
} catch (err) {
  console.error('获取剪贴板文件失败：', err);
}
