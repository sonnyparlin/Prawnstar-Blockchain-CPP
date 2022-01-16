mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
flags := -Wall -std=c++17
libs := -lcryptopp
includes := $(PWD)/include/
outfile := main
compiler := /usr/bin/g++

all: 
	$(compiler) $(flags) $(libs) -I $(includes) -g $(PWD)/*.cpp -o $(PWD)/$(outfile)

cryptopp: 
	$(MAKE) -C cryptopp860 all ; $(MAKE) -C cryptopp860 install
