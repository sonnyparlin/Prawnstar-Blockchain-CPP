from platform import node
import requests


def post_transaction(sender, receiver, amount, type):
    url = 'http://localhost:8001/transact'
    json = '{"transaction": {"sender": "'+ sender +'","receiver": "'+ receiver +'", "amount": '+ amount +', "type": "'+ type +'"}}'
    r = requests.post(url, data = json)
    print(r.text)

exchange = 'pv1403d478bfc4949c9c68af53bbaf8deb58c4eac'
nodeWallet = 'pv171e654b041c51d978becc5cf65b0e2f48ff7'
alice = 'pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7'
bob = 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42'
node2 = 'pv17bf144b32a8522fece3261b0a9fb93a3e2255b42'

post_transaction(exchange, nodeWallet, '500', 'EXCHANGE')
post_transaction(exchange, node2, '500', 'EXCHANGE')
post_transaction(node2, node2, '100', 'STAKE')

post_transaction(exchange, alice, '500', 'EXCHANGE')
post_transaction(exchange, bob, '300', 'EXCHANGE')

for i in range(6):
    post_transaction(alice, bob,'50', 'TRANSFER')

post_transaction(nodeWallet, nodeWallet, '200', 'STAKE')

post_transaction(exchange, alice, '1000', 'EXCHANGE')

for i in range(14):
    post_transaction(alice, bob,'50', 'TRANSFER')

post_transaction(exchange, alice, '5000', 'EXCHANGE')

for i in range(12):
    post_transaction(alice, bob,'50', 'TRANSFER')

for i in range(60):
    post_transaction(alice, bob,'1', 'TRANSFER')


print("Alice's wallet: ")
x = requests.get('http://localhost:8001/wallet/'+alice)
print(x.text)

print("Bob's wallet: ")
x = requests.get('http://localhost:8001/wallet/'+bob)
print(x.text)