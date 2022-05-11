# Todo
* ~~Set up test google test library~~
* ~~Write tests~~
* ~~keep the blockchain stored locally using a json file.~~
* Set up the option to use mongodb for local 
blockchain backups.
* Find all instances of json::parse() and put them 
in a try/catch block. Anytime these fail, it kills the running node.
* Add front end for users to create wallets
* ~~Modify the handleBlockChain method to only send over blocks that the 
requesting node is missing.~~
* ~~set new nodes to imediately request the blockchain~~
* Build or find block explorer
* Build candlestick chart
* ~~Add forger reward system~~
* Integrate the liquidity pool script into 
transactions to determine the price at time of 
purchase, this includes figuring out how to
create a liquidity pool / AMM
* ~~Create documentation suite using doxygen~~
* create reward transactions for forgers
* Add security to the Node API to ensure 
connections can only come from the current 
machine
* ~~Fix bug: if node goes down, staker info is lost~~
* Make React UI better
* ~~Code the following rules for forging blocks:~~ 
  * ~~Blocks will be created every 5 minutes~~
  * ~~Blocks are created as soon as there are 20 transactions in the transaction
  pool~~
* Set up pending state for transactions, this will
depend on confirmations on the blockchain.
* Code a confirmation system with rules
  * 6 confirmations = confirmed
  * Figure out how to update state
  * Build this functionality into the UI
* Code support for using google authenticator when
creating transactions.
* ~~Fix signature verification bug~~
* Write more tests
* Update React to get rid of security vulnerabilities in the dependancies
* Set up ability for non-node addresses to stake and earn rewards.
  * Fix problem with stake winner's node not being available.