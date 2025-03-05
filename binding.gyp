{
  "targets": [
    {
      "target_name": "clipboard",

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
              "AdditionalOptions": ["/std:c++latest"]
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