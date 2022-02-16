# Compile and run

## Download dependancies
    * Install boost_1_78_0, 
        * https://www.boost.org/users/download/
        * https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install

    *  cd cryptopp860;make; sudo make install;cd ..

    * Run `make all` to build and ./main to run.

If you want to compile from vscode, set your tasks.json file to look like this:

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
                "-std=c++14",
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