# Compile and run

## Download dependancies
    * Install boost_1_78_0, 
        * https://www.boost.org/users/download/
        * https://www.boost.org/doc/libs/1_78_0/more/getting_started/unix-variants.html#easy-build-and-install

    *  cd cryptopp860;make; sudo make install;cd ..

    * Run `make all` to build and ./main to run.

    * Node 1 (master node): ./main 127.0.0.1 10001 8001

    * Node 2: ./main 127.0.0.1 10002 127.0.0.1 8002

    * Node 3: ./main 127.0.0.1 10003 127.0.0.1 8003

    ./main ip_of_this_node p2p_port_of_this_node ip_of_master_node api_port_of_this_node

    Use Postman to send a transaction: https://www.postman.com

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