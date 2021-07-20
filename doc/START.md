# Getting Started

## Clone Repositories

Clone [nDPI](https://github.com/ntop/nDPI) and this repository in the same directory.

```
clone https://github.com/ntop/nDPI.git
clone https://github.com/globalcitycorp/gcap-monitor.git
```

## Build nDPI

See [this instruction](https://github.com/ntop/nDPI) and build nDPI.

```
pushd nDPI
./autogen.sh
./configure
make
popd
```

## Build gCap Monitor

```
pushd gcap-monitor
make
```

## Run gCap Monitor
