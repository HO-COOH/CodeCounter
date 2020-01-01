# CodeCounter
A convenient code counting tools to provide a statistic for your project.
## Build
This is a C++ project based on Visual Studio 2019 solution. To build from source, a ``C++17`` compatible compiler is required. 
## Run
Basic Usage: [optional flags] "[optional path]"
- optional flag:
  - -r: recursively read all the (source) files inside ``path``. If ``path`` is not given, the process is done in the current directory of the exectuable.
  - -d: show detailed information of all the (source) files read
- optional "path": if specified, read the source files under ``path`` instead of the current directory of the exectuable. Must be quoted.