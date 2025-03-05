#include <node_api.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <vector>
#include <string>
#include <cstring>

napi_value GetClipboardFiles(napi_env env, napi_callback_info info) {
  std::vector<std::string> filePaths;
  std::string originalUriList; // 提升到函数作用域
  
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
      
      Atom targets[] = {XInternAtom(display, "_GTK_URI_LIST", False), XInternAtom(display, "text/uri-list", False)};
      
      for (int j = 0; j < 2; j++) {
        int prop_status = XGetWindowProperty(display, window, targets[j],
                            0, (~0L), False, AnyPropertyType, &type, &format,
                            &nitems, &after, &data);
        
        if (prop_status == Success && data) {
          fprintf(stderr, "成功获取剪贴板数据，格式: %s\n", XGetAtomName(display, targets[j]));
          break;
        }
      }
      
if (!data) {
    fprintf(stderr, "错误：未找到支持的剪贴板格式，当前可用格式:\n");
    
    // 调试输出所有支持的原子类型
    Atom supported_types[] = {
      XInternAtom(display, "x-special/gnome-copied-files", False),
      XInternAtom(display, "_GTK_URI_LIST", False),
      XInternAtom(display, "text/uri-list", False),
      XInternAtom(display, "UTF8_STRING", False),
      XInternAtom(display, "COMPOUND_TEXT", False)
    };
    
    for (int i = 0; i < sizeof(supported_types)/sizeof(supported_types[0]); ++i) {
      char* name = XGetAtomName(display, supported_types[i]);
      fprintf(stderr, " - %s (%s)\n",
        name,
        XGetAtomName(display, supported_types[i]) == name ? "存在" : "不存在");
      XFree(name);
    }
    
    XCloseDisplay(display);
    napi_throw_error(env, "ECLIPBOARD", "不支持的剪贴板格式");
    return nullptr;
}
      char* uri_list = (char*)data;
      originalUriList = uri_list;  // 赋值给函数级变量
      fprintf(stderr, "原始剪贴板数据: %s\n", uri_list);
      char* uri = strtok(uri_list, "\r\n");
      while (uri) {
        if (strncmp(uri, "file://", 7) == 0) {
          filePaths.push_back(uri + 7);
        }
        uri = strtok(NULL, "\r\n");
      }
      XFree(data);
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
    fprintf(stderr, "警告：解析到空文件列表，原始URI内容:\n%s\n", originalUriList.c_str());
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