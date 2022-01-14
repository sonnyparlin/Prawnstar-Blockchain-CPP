# Compile and run

## Download boost for uuid creation
    * https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#get-boost

    * Follow the install directions

    * https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install

    * Make sure to set the prefix path to your local build directory. So if you have your files 
    in /Users/johnny/development/my-project, you should set your prefix like so:

    * ./bootstrap.sh --prefix=/Users/johnny/development/my-project

    * Run ./b2 install

    * Now you should have a lib and include directory in your project. So you'll need to 
    add the include to your build. Make sure you add `-I /Users/johnny/development/my-project/include` 
    to your build.

    * Install cryptopp 8.6.0 https://www.cryptopp.com/#download, unzip in your development directory 
    and then run make;sudo make install, you will have to add the compiler flad -lcryptopp.

    * Alternativly, instead of compiling from vs code, you can also just run `make` to build 
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