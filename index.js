const { promisify } = require("util");
const { getClipboardFiles } = require("node-gyp-build")(__dirname);

const readFilePaths = () => {
  return getClipboardFiles();
};

module.exports = {
  readFilePaths,
  readFilePathsAsync: promisify(readFilePaths),
};
