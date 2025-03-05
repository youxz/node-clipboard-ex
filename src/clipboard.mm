#include <node_api.h>
#include <Cocoa/Cocoa.h>
#include <AppKit/AppKit.h>

napi_value GetClipboardFiles(napi_env env, napi_callback_info info) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  NSArray *files = [pasteboard readObjectsForClasses:@[[NSURL class]] options:nil];
  
  napi_value result;
  napi_create_array(env, &result);
  
  uint32_t index = 0;
  for (NSURL *url in files) {
    if ([url isFileURL]) {
      napi_value file_path;
      napi_create_string_utf8(env, [[url path] UTF8String], NAPI_AUTO_LENGTH, &file_path);
      napi_set_element(env, result, index++, file_path);
    }
  }
  
  [pool release];
  return result;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc = {
    "getClipboardFiles",
    NULL,
    GetClipboardFiles,
    NULL,
    NULL,
    NULL,
    napi_default,
    NULL
  };
  napi_define_properties(env, exports, 1, &desc);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)