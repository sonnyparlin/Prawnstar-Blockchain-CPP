from platform import node
import requests


def post_transaction(sender, receiver, amount, type):
    url = 'http://localhost:8000/transact'
    json = '{"transaction": {"sender": "'+ sender +'","receiver": "'+ receiver +'", "amount": '+ amount +', "type": "'+ type +'"}}'
    r = requests.post(url, data = json)
    print(r.text)

exchange = 'pv1403d478bfc4949c9c68af53bbaf8deb58c4eac'
nodeWallet = 'pv171e654b041c51d978becc5cf65b0e2f48ff7'
alice = 'pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7'
bob = 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42'

post_transaction(exchange, alice, '500', 'EXCHANGE')

for i in range(1):
    post_transaction(alice, bob,'50', 'TRANSFER')

print("Alice's wallet: ")
x = requests.get('http://localhost:8000/wallet/'+alice)
print(x.text)

print("Bob's wallet: ")
x = requests.get('http://localhost:8000/wallet/'+bob)
print(x.text)
