const { getClipboardFiles } = require('./build/Release/clipboard');


const readFilePaths = () => {
    return getClipboardFiles();
}   

module.exports = {
    readFilePaths,
}