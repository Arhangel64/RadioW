# RadioW

## Dependencies

1. cmake >= 2.8.12
2. qt 5.*
  1. qt(5)-base
  2. qt(5)-websockets
3. nodejs
4. npm
5. libssh

## Building

Attention! During the first build internet connection is mandatory. There are some nodejs dependencies, which npm is going to unstall during configuration.

1. Create a build directory and checkout there. For example if you are in project directory, and want to build in subdirectory run
  ```bash
  mkdir build
  cd build
  ```
2. Run cmake to configure the project, giving the path to project root directory. For example
  ```bash
  cmake ../
  ```
3. Run make to build the project. For example
  ```bash
  make
  ```
