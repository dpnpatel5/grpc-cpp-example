# Build Procedure

The build process for this project is straightforward. Run `make` to build the project, but ensure that gRPC is installed before proceeding.

## gRPC Installation

### Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y build-essential autoconf libtool pkg-config
sudo apt-get install -y libgflags-dev libgtest-dev
```
### Clone the gRPC repository
```bash
git clone --recurse-submodules -b v1.54.0 https://github.com/grpc/grpc
cd grpc
```
### Build and install Protobuf
```bash
cd third_party/protobuf
git submodule update --init --recursive
mkdir -p cmake/build
cd cmake/build
cmake ..
make -j $(nproc)
sudo make install
sudo ldconfig
```

### Build and install gRPC
```bash
cd ../../../
mkdir -p cmake/build
cd cmake/build
cmake ..
make -j $(nproc)
sudo make install
sudo ldconfig
```




