objs: eccGenerateSignVerifyHash.o eccGenKeyToDisk.o ecGenKeyPair.o jsontest.o signaturetest.o \
		vectormap.o liquidity-test.o hashtest.o maptest.o template.o jsonSerialization.o \
		rsatest.o vector.o sundeep_test.o mongotest.o

mongotest.o: mongotest.cpp
	/usr/bin/g++ -std=c++14 -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 -I/usr/local/include/bsoncxx/third_party/mnmlstc/ mongotest.cpp -c
	/usr/bin/g++ -std=c++14 -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 -I/usr/local/include/bsoncxx/third_party/mnmlstc/ -L/usr/local/lib -lmongocxx -lbsoncxx mongotest.o -o mongotest

sundeep_test.o: sundeep_test.cpp
	/usr/bin/g++ -Wall -std=c++14 -I/usr/local/opt/openssl@3/include sundeep_test.cpp -c 
	/usr/bin/g++ -Wall -std=c++14 -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib sundeep_test.o -lcrypto -o sundeep_test

eccGenerateSignVerifyHash.o: eccGenerateSignVerifyHash.cpp
	/usr/bin/g++ -Wall -std=c++14 -I/usr/local/opt/openssl@3/include eccGenerateSignVerifyHash.cpp -c 
	/usr/bin/g++ -Wall -std=c++14 -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib eccGenerateSignVerifyHash.o -lcrypto -o eccGenerateSignVerifyHash

eccGenKeyToDisk.o: eccGenKeyToDisk.cpp
	/usr/bin/g++ -Wall -std=c++14 -I/usr/local/opt/openssl@3/include eccGenKeyToDisk.cpp -c
	/usr/bin/g++ -Wall -std=c++14 -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lcrypto eccGenKeyToDisk.o -o eccGenKeyToDisk

ecGenKeyPair.o: ecGenKeyPair.cpp
	/usr/bin/g++ ecGenKeyPair.cpp -std=c++17 -c
	/usr/bin/g++ ecGenKeyPair.cpp -std=c++17 -lcryptopp -o ecGenKeyPair

jsontest.o: jsontest.cpp
	/usr/bin/g++ jsontest.cpp -I ../include -std=c++17 -c
	/usr/bin/g++ jsontest.o -I ../include -std=c++17 -o jsontest

signaturetest.o: signaturetest.cpp
	/usr/bin/g++ signaturetest.cpp -std=c++17 -c
	/usr/bin/g++ signaturetest.o -std=c++17 -lcryptopp -o signaturetest

vectormap.o: vectormap.cpp
	/usr/bin/g++ vectormap.cpp -std=c++17 -c
	/usr/bin/g++ vectormap.o -std=c++17 -o vectormap

hashtest.o: hashtest.cpp
	/usr/bin/g++ hashtest.cpp -std=c++17 -c
	/usr/bin/g++ hashtest.o -std=c++17 -lcryptopp -o hashtest

maptest.o: maptest.cpp
	/usr/bin/g++ maptest.cpp -std=c++17 -c
	/usr/bin/g++ maptest.o -std=c++17 -lcryptopp -o maptest

template.o: template.cpp
	/usr/bin/g++ -Wall -std=c++17 template.cpp -c
	/usr/bin/g++ -Wall -std=c++17 template.o -o template

jsonSerialization.o: jsonSerialization.cpp
	/usr/bin/g++ -std=c++17 -O2 -Wall -pedantic -pthread jsonSerialization.cpp -c
	/usr/bin/g++ -std=c++17 -O2 -Wall -pedantic -pthread jsonSerialization.o -o jsonSerialize

rsatest.o: rsatest.cpp
	/usr/bin/g++ rsatest.cpp -std=c++17 -c
	/usr/bin/g++ rsatest.o -std=c++17 -lcryptopp -o rsatest

vector.o: vector.cpp
	/usr/bin/g++ vector.cpp -std=c++17 -c
	/usr/bin/g++ vector.o -std=c++17 -o vector

liquidity-test.o: liquidity-test.cpp
	/usr/bin/g++ liquidity-test.cpp -std=c++17 -c
	/usr/bin/g++ liquidity-test.o -std=c++17 -o liquidity-test

all: objs

clean:
	rm *.o
