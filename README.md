# 6nake


## Build instructions for Linux

You'll need to have `cmake`, `build-essential` and the `libsdl2-dev` and `libsdl2-ttf-dev` packages.

```bash
git clone --recursive git@github.com:KayEss/6nake.git
cd 6nake
cmake -B ./build.tmp -DCMAKE_BUILD_TYPE=Release .
make -C ./build.tmp
```

You can run it straight from the build tree without installing:

```bash
./build.tmp/src/6nake
```
