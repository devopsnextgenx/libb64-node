{
    "targets": [
        {
            "target_name": "libb64",
            "sources": [ "library/b64.cpp" ],
            "include_dirs": [
                "<!(node -e \"require('node-addon-api').include\")"
            ],
            "cflags!": [ "-fno-exceptions" ],
            "cflags!": [ "-fno-rtti" ],
            "cflags!": [ "-std=c++11" ],
            "cflags!": [ "-std=c++14" ],
            "cflags!": [ "-std=c++17" ],
            "cflags!": [ "-std=c++20" ]
        }
    ]
}