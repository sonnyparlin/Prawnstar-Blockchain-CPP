# Dependancies Needed and Startup Instructions

### Download dependancies
* Install boost_1_78_0

   [Boost download](https://www.boost.org/users/download/)
   
   [Boost installation info](https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install)

*  Install [OpenSSL](https://www.openssl.org/). 

### Making and Running the App 
* cmake -B build -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl@3
* cd build
* make

### Running the app

* Node 1 (master node): ./main 127.0.0.1 10001 8001

* Node 2: ./main 127.0.0.1 10002 127.0.0.1 8002

* Node 3: ./main 127.0.0.1 10003 127.0.0.1 8003

The format for the master node:

./main ip_of_this_node p2p_port_of_this_node api_port_of_this_node

The format of all client nodes:

./main ip_of_this_node p2p_port_of_this_node ip_of_master_node api_port_of_this_node

### Testing it out

You can use `python test.py` to get the first 99 blocks into the blockchain or you can use Postman to send transactions to the blockchain.

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
