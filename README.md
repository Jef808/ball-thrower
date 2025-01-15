# Draw With Nested Circles

## How to build an executable binary

1. Make sure Git and CMake are intalled on your system.

2. Clone the repository: in a shell execute the command

``` shell
git clone https://github.com/Jef808/ball-thrower
```

3. Now navigate to the project's root and set up the CMake build template

``` shell
cd ball-thrower

cmake -S . -B build -DCMAKE_BUILD_TYPE=release
```

4. Once cmake has setup everything, you are ready to build the executable binary.
By using the following command, the binary will be compiled and located at `build/bin/main`

``` shell
cmake --build build
```

## Run the thing

Now that an executable binary was successfully built, simply execute it with

``` shell
./build/bin/main
```

