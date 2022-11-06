# 6nake


## Build instructions for Linux

You'll need to have some development tools and libraries available. On Debian based systems (like Ubuntu) the following should suffice:

```bash
sudo apt install cmake build-essential libsdl2-dev libsdl2-ttf-dev
```

Then to clone and build, the following can be used:

```bash
git clone --recursive git@github.com:KayEss/6nake.git
cd 6nake
cmake -B ./build.tmp -DCMAKE_BUILD_TYPE=Release .
make -C ./build.tmp
```

You can run it straight from the build tree without installing:

```bash
./build.tmp/6nake
```
