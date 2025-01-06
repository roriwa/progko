# progko

> ![NOTE]
> This Program was developed and tested under linux (ubuntu).
> The steps required to build and run the program may differ on another operating system.

## Setup

```shell
sudo apt install build-essential cmake libopencv-dev libopenmpi-dev openmpi-bin gcc-multilib libomp-dev
```

## Building

```shell
mkdir -p build && cd build
cmake ..
make
```

## Running

```shell
cd build
./progko [...args]
```

## Performance Testing

```shell
bash test.sh
```
