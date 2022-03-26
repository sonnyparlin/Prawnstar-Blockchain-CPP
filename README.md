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
* ./Prawnstar <your_ip_address> 10001 8001
* start second node: ./Prawnstar <your_ip_address> 10001 <ip_of_master_server> 8002

### Windows
* Install [mingw64](https://www.mingw-w64.org/)
* install Boost 1.7.8
* Install OpenSSL 3
* Check `CMakeLists.txt` and make sure the include and library paths are correct for your system.
* cmake .. -G "Unix Makefiles" 

### Running the app

* Node 1 (master node): ./Prawnstar 127.0.0.1 10001 8001

* Node 2: ./Prawnstar 127.0.0.1 10002 127.0.0.1 8002

* Node 3: ./Prawnstar 127.0.0.1 10003 127.0.0.1 8003

The format for the master node:

./Prawnstar ip_of_this_node p2p_port_of_this_node api_port_of_this_node

The format of all client nodes:

./Prawnstar ip_of_this_node p2p_port_of_this_node ip_of_master_node api_port_of_this_node

### Testing

`python ApiTest.py <number_of_transactions>` (keep this to 500 or 1000 for testing purposes on a personal machine)

### Using Postman

Use [Postman](https://www.postman.com) to send a transaction

```json
{"transaction":
    {
        "sender": "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac",
        "receiver": "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7",
        "amount": 500,
        "type": "EXCHANGE"
    }
}
```

Send a second transaction:

```json
{"transaction":
    {
        "sender": "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7",
        "receiver": "pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42",
        "amount": 50,
        "type": "TRANSFER"
    }
}
```
