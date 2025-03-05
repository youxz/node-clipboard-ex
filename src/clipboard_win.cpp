#include <node_api.h>
#include <windows.h>
#include <shlobj.h>
#include <vector>
#include <string>

napi_value GetClipboardFiles(napi_env env, napi_callback_info info) {
  std::vector<std::wstring> filePaths;
  
  if (OpenClipboard(NULL)) {
    HDROP hDrop = (HDROP)GetClipboardData(CF_HDROP);
    if (hDrop) {
      UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
      for (UINT i = 0; i < fileCount; i++) {
        wchar_t path[MAX_PATH];
        DragQueryFileW(hDrop, i, path, MAX_PATH);
        filePaths.push_back(path);
      }
    }
    CloseClipboard();
  }

  napi_value result;
  napi_create_array(env, &result);

  for (size_t i = 0; i < filePaths.size(); ++i) {
    napi_value file_path;
    napi_create_string_utf8(env, 
      std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(filePaths[i]).c_str(),
      NAPI_AUTO_LENGTH, &file_path);
    napi_set_element(env, result, i, file_path);
  }

  return result;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc = {"getClipboardFiles", 0, GetClipboardFiles, 0, 0, 0, napi_default, 0};
  napi_define_properties(env, exports, 1, &desc);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)