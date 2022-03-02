# Todo
* Set up test library, looking at boost and ctest, https://www.codeguru.com/cplusplus/ten-c-testing-tools-for-developers-to-consider/
* Write tests
* set up nosql for each node, CouchDB looks good. https://docs.couchdb.org/en/stable/setup/cluster.html or possibly just write json to file for each node.
* Find all instances of json::parse() and put them 
in a try/catch block. Anytime these fail, it kills the running node.
* Add front end for users to create wallets
* ~~Modify the handleBlockChain method to only send over blocks that the requesting node is missing.~~
* ~~set new nodes to imediately request the blockchain~~
* Build or find block explorer
* build candlestick chart
* Integrate the liquidity pool script into transactions to determine the price at time of purchase
* Make this documentation suite better
* create reward transactions for forgers
* Add security to the Node API to ensure connections can only come from the current machine