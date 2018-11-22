# jMirror

jMirror is a free, open source piece of software for you to run on your smart mirror.

- Core is built entirely in C++
- Use latest HTML5 technologies to build custom interfaces
- Highly customizable and portable

# Features

- HTML5 compatibility
- Integrated web server
- Most aspects customizable
- Built on top of Chromium Embedded Framework

# Installation

In order to build jMirror out of the box, ensure the following folders and files exist:

| Library | Path |
| ------- | ---- |
| CEF | /usr/lib/cef/libcef_dll_wrapper.a |
| CEF | /usr/include/cef/(CEF Includes) |
| SDL2 | /usr/lib/x86_65-linux-gnu/libSDL2.so |
| SDL2 | /usr/include/SDL2/(SDL Includes) |
| jConfig | /usr/lib/jconfig.so |
| jConfig | /usr/include/jconfig/(jConfig Includes) |

Then, simply run the following command in the same directory as the Makefile:

```sh
$ sudo make all
```

If everything is set up correctly, a jMirror excecutable should be built in the *bin* folder.
