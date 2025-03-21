const { promisify } = require("util");
const prebuildify = promisify(require("prebuildify"));

const platform = process.platform;

const getSupportedArchs = () => {
  if (platform === "darwin") {
    return ["x64", "arm64"];
  }

  if (platform === "win32") {
    return ["x64", "ia32"];
  }

  if (platform === "linux") {
    return ["x64", "arm64", "loong64"];
  }

  return [];
};

const nodeVersion = "20.17.0";

async function build() {
  for (const arch of getSupportedArchs()) {
    await prebuildify({
      platform,
      arch,
      napi: true,
      target: nodeVersion,
    });
  }
}

build();
