{
    "targets": [
        {
            "target_name": "libb64",
            "sources": [ "src/b64.cpp" ],
            "include_dirs": [
                "<!(node -e \"require('node-addon-api').include\")",
                "include"
            ],
            "libraries": [
                "-L<(module_root_dir)/libraries -lb64"
            ],
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-rtti" ],
            "cflags_cc": ["-std=c++14"],
            "ldflags": ["-Wl,-rpath,'$$ORIGIN'"]
        }
    ]
}