#include <node_api.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <string>
#include <cstring>

napi_value GetClipboardFiles(napi_env env, napi_callback_info args) {
    Display* display = XOpenDisplay(NULL);
    Window window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, 1, 1, 0, 0, 0);
    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
    Atom targets_atom = XInternAtom(display, "TARGETS", False);
    Atom uri_list_atom = XInternAtom(display, "text/uri-list", False);

    XConvertSelection(display, clipboard, uri_list_atom, targets_atom, window, CurrentTime);
    XFlush(display);

    XEvent event;
    XNextEvent(display, &event);

    if (event.type == SelectionNotify && event.xselection.property != None) {
        char* data;
        unsigned long length;
        XGetWindowProperty(display, window, event.xselection.property, 0, (~0L), False, AnyPropertyType,
                           NULL, NULL, &length, NULL, (unsigned char**)&data);

        std::vector<std::string> filePaths;
        char* line = strtok(data, "\r\n");
        while (line != NULL) {
            if (strstr(line, "file://") == line) {
                std::string path(line + 7); // 去除"file://"前缀
                filePaths.push_back(path);
            }
            line = strtok(NULL, "\r\n");
        }

        // 转换为Node.js数组返回
        napi_value result;
        napi_create_array_with_length(env, filePaths.size(), &result);
        for (size_t i = 0; i < filePaths.size(); i++) {
            napi_value str;
            napi_create_string_utf8(env, filePaths[i].c_str(), NAPI_AUTO_LENGTH, &str);
            napi_set_element(env, result, i, str);
        }

        XFree(data);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
        return result;
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return nullptr;
}

NAPI_MODULE_INIT() {
    napi_value fn;
    napi_create_function(env, NULL, 0, GetClipboardFiles, NULL, &fn);
    napi_set_named_property(env, exports, "getClipboardFiles", fn);
    return exports;
}
