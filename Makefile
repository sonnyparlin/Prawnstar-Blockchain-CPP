mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
flags := -Wall -std=c++14
libs := -lcrypto -L/usr/local/opt/openssl@3/lib
includes := $(PWD)/include/ -I /usr/local/include -I /usr/local/opt/openssl@3/include
outfile := main
compiler := /usr/bin/g++

objs: AccountModel.o Message.o SocketCommunication.o Wallet.o utils.o \
		Block.o Node.o SocketConnector.o config.o Blockchain.o NodeApi.o \
		Transaction.o main.o Lot.o ProofOfStake.o TransactionPool.o \
		p2putils.o

AccountModel.o: AccountModel.cpp AccountModel.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/AccountModel.cpp
Node.o: Node.cpp Node.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Node.cpp
Transaction.o: Transaction.cpp Transaction.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Transaction.cpp
p2putils.o: p2putils.cpp p2putils.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/p2putils.cpp
Block.o: Block.cpp Block.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Block.cpp
NodeApi.o: NodeApi.cpp NodeApi.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/NodeApi.cpp
TransactionPool.o: TransactionPool.cpp TransactionPool.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/TransactionPool.cpp
utils.o: utils.cpp utils.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/utils.cpp
Blockchain.o: Blockchain.cpp Blockchain.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Blockchain.cpp
ProofOfStake.o: ProofOfStake.cpp ProofOfStake.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/ProofOfStake.cpp
Wallet.o: Wallet.cpp Wallet.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Wallet.cpp
Lot.o: Lot.cpp Lot.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Lot.cpp
SocketCommunication.o: SocketCommunication.cpp SocketCommunication.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/SocketCommunication.cpp
config.o: config.cpp config.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/config.cpp
Message.o: Message.cpp Message.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/Message.cpp
SocketConnector.o: SocketConnector.cpp SocketConnector.hpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/SocketConnector.cpp
main.o: main.cpp
	$(compiler) $(flags) -I $(includes) -g -c $(PWD)/main.cpp

all: objs
	$(compiler) $(flags) $(libs) -I $(includes) -g $(PWD)/*.o -o $(PWD)/$(outfile)

clean:
	rm *.o

cryptopp: 
	$(MAKE) -C cryptopp860 all ; /usr/bin/sudo $(MAKE) -C cryptopp860 install
