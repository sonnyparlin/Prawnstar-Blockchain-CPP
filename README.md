# Dependancies Needed and Startup Instructions

### Download dependancies
* Install boost_1_78_0

   [Boost download](https://www.boost.org/users/download/)
   
   [Boost installation info](https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install)

*  Install [OpenSSL 3](https://www.openssl.org/). 

### Making and Running the on UNIX style systems including OSX.
* Check `CMakeLists.txt` and make sure the include and library paths are correct for your system.
* mkdir build
* cd build
* cmake ..
* make
* cp ../*.pem .
* `./Prawnstar your_ip_address 10001 8001`
* start second node: `./Prawnstar your_ip_address 10001 ip_of_master_server 8002`

### Windows
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
2. `python ApiTest.py <number_of_transactions>`
3. `build/tests/RunTests` (Google test suite)