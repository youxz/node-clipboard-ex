{
  "targets": [
    {
      "target_name": "clipboard",
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        ["OS==\"mac\"", {
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LIBRARY": "libc++",
            "MACOSX_DEPLOYMENT_TARGET": "10.7"
          }
        }],
        ["OS==\"win\"", {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1
            }
          }
        }],
        ["OS==\"linux\"", {
          "cflags_cc": ["-fexceptions"]
        }]
      ],
      "conditions": [
        ["OS==\"mac\"", {
          "sources": ["src/clipboard.mm"],
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS": ["-std=c++11"],
            "OTHER_LDFLAGS": ["-framework AppKit", "-framework Cocoa", "-framework Foundation"],
            "link_settings": {
              "libraries": []
            }
          }
        }],
        ['OS=="win"', {
          "sources": ["src/clipboard_win.cpp"],
          "defines": ["WIN32_LEAN_AND_MEAN"],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "AdditionalOptions": ["/std:c++17"]
            }
          }
        }],
        ['OS=="linux"', {
          "sources": ["src/clipboard_linux.cpp"],
          "libraries": ["-lX11"]
        }]
      ]
    }
  ]
}