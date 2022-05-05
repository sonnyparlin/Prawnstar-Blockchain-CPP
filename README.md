# Dependancies Needed and Startup Instructions

### Download dependancies
* Install boost_1_78_0

   [Boost download](https://www.boost.org/users/download/)
   
   [Boost installation info](https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install)

*  Install [OpenSSL 3](https://www.openssl.org/). 

### Building on UNIX style systems including OSX.
* Check `CMakeLists.txt` and make sure the include and library paths are correct for your system.
* mkdir build
* cd build
* cmake ..
* make
* cp ../*.pem .
* cp ../*.pem PrawnstarTests/tests/

### Building on Windows using mingw64
* Install [mingw64](https://www.mingw-w64.org/)
* install Boost 1.7.8
* Install OpenSSL 3
* Check `CMakeLists.txt` and make sure the include and library paths are correct for your system.
* cmake .. -G "Unix Makefiles" 
* Follow the UNIX style directions after `cmake ..`

### Running the app
* Edit `config.json` and add your master ip address and port.
* Node 1 (master node): ./Prawnstar 127.0.0.1 10001 8001
* Node 2: ./Prawnstar 127.0.0.1 10002 127.0.0.1 8002
* Node 3: ./Prawnstar 127.0.0.1 10003 127.0.0.1 8003

The format for the master node:

./Prawnstar ip_of_this_node p2p_port_of_this_node api_port_of_this_node

The format of all client nodes:

./Prawnstar ip_of_this_node p2p_port_of_this_node ip_of_master_node api_port_of_this_node

### Testing
1. `python ApiTest.py <number_of_transactions>`
2. `build/tests/RunTests` (Google test suite)

Or run both at once with `sh runtests.sh` on UNIX systems. 