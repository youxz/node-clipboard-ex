#include <node_api.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <vector>
#include <string>
#include <cstring>

napi_value GetClipboardFiles(napi_env env, napi_callback_info info) {
  std::vector<std::string> filePaths;
  
  Display* display = XOpenDisplay(NULL);
  if (!display) {
    napi_throw_error(env, "EOPEN_DISPLAY", "无法打开X11显示连接");
    return nullptr;
  }
  {
    Window window = XGetSelectionOwner(display, XInternAtom(display, "CLIPBOARD", False));
    if (window) {
      Atom type;
      int format;
      unsigned long nitems, after;
      unsigned char* data = NULL;
      
      int prop_status = XGetWindowProperty(display, window, XInternAtom(display, "_GTK_URI_LIST", False),
                            0, (~0L), False, AnyPropertyType, &type, &format,
                            &nitems, &after, &data);
      
      if (prop_status != Success) {
        fprintf(stderr, "XGetWindowProperty失败，状态码：%d\n", prop_status);
      } else if (data) {
        char* uri_list = (char*)data;
        char* uri = strtok(uri_list, "\r\n");
        while (uri) {
          if (strncmp(uri, "file://", 7) == 0) {
            filePaths.push_back(uri + 7);
          }
          uri = strtok(NULL, "\r\n");
        }
        XFree(data);
      }
    }
    XCloseDisplay(display);
  }

  napi_value result;
  napi_create_array(env, &result);

  for (size_t i = 0; i < filePaths.size(); ++i) {
    napi_value file_path;
    napi_create_string_utf8(env, filePaths[i].c_str(), NAPI_AUTO_LENGTH, &file_path);
    napi_set_element(env, result, i, file_path);
  }

  if (filePaths.empty()) {
    napi_throw_error(env, "ECLIPBOARD", "剪贴板中没有文件路径或格式不受支持");
  }
  return result;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc = {"getClipboardFiles", 0, GetClipboardFiles, 0, 0, 0, napi_default, 0};
  napi_define_properties(env, exports, 1, &desc);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)