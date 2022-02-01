# Compile and run

## Download dependancies
    *  cd cryptopp860;make; sudo make install;cd ..

    * Install boost_1_78_0, https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install

    * Alternativly, instead of compiling from vs code, you can run `make` to build 
    and ./main to run.

My vscode tasks.json file looks like this:

```json
{
	"version": "2.0.0",
	"tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++ build active file",
            "command": "/usr/bin/g++",
            "args": [
                "-Wall",
                "-std=c++17",
                "-lcryptopp",
                "-I",
                "${fileDirname}/include/",
                "-g",
                "${fileDirname}/*.cpp",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "compiler: /usr/bin/g++"
        }
    ]
}
```
