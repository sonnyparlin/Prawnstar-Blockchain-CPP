import requests 

exchange = 'pv1403d478bfc4949c9c68af53bbaf8deb58c4eac'
nodeWallet = 'pv171e654b041c51d978becc5cf65b0e2f48ff7'
alice = 'pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7'
bob = 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42'
node2 = 'pv17bf144b32a8522fece3261b0a9fb93a3e2255b42'

print("Alice's wallet: ")
x = requests.get('http://127.0.0.1:8001/wallet/'+alice)
print(x.text)

print("Bob's wallet: ")
x = requests.get('http://127.0.0.1:8001/wallet/'+bob)
print(x.text)

print("Node one wallet: ")
x = requests.get('http://127.0.0.1:8001/wallet/'+nodeWallet)
print(x.text)

print("Node two wallet: ")
x = requests.get('http://127.0.0.1:8001/wallet/'+node2)
print(x.text)