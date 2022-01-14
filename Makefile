all: 
	/usr/bin/g++ -Wall -std=c++17 -lcryptopp -I /Users/sonnyparlin/Github/pr-blockchain-c/include/ -g /Users/sonnyparlin/Github/pr-blockchain-c/*.cpp -o /Users/sonnyparlin/Github/pr-blockchain-c/main

cryptopp: 
	$(MAKE) -C cryptopp860 all ; $(MAKE) -C cryptopp860 install
