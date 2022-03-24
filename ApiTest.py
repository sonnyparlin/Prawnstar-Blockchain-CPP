from platform import node
import requests
import time
import sys
import json

f = open('config.json')
data = json.load(f)
ip = data["master_server_ip"]
f.close()

def post_transaction(session, sender, receiver, amount, type):
    url = f"http://{ip}:8001/transact"
    json = '{"transaction": {"sender": "'+ sender +'","receiver": "'+ receiver +'", "amount": '+ amount +', "type": "'+ type +'"}}'
    r = session.post(url, data = json)
    #if "Received" not in r.text:
    #print(r.text)
    
    #time.sleep(2)

start_time = time.time()

exchange = 'pv1403d478bfc4949c9c68af53bbaf8deb58c4eac'
nodeWallet = 'pv171e654b041c51d978becc5cf65b0e2f48ff7'
alice = 'pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7'
bob = 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42'
node2 = 'pv17bf144b32a8522fece3261b0a9fb93a3e2255b42'

with requests.Session() as s:
    post_transaction(s, exchange, nodeWallet, '1', 'EXCHANGE')
    post_transaction(s, exchange, node2, '4', 'EXCHANGE')
    #post_transaction(s, node2, node2, '3', 'STAKE')
    post_transaction(s, exchange, alice, '20000', 'EXCHANGE')
# post_transaction(exchange, bob, '300', 'EXCHANGE')

    time.sleep(2)
    for x in range(int(sys.argv[1])):
        #print(x)            
        post_transaction(s, alice, bob, '1', 'TRANSFER')
        #time.sleep(0.1)

print("--- %s seconds ---\n\n" % (time.time() - start_time))

print("getting final balances...")
time.sleep(1)

print("Alice's wallet: ")
x = requests.get(f"http://{ip}:8001/wallet/{alice}")
print(x.text)

print("Bob's wallet: ")
x = requests.get(f"http://{ip}:8001/wallet/{bob}")
print(x.text)

print("Node one wallet: ")
x = requests.get(f"http://{ip}:8001/wallet/{nodeWallet}")
print(x.text)

print("Node two wallet: ")
x = requests.get(f"http://{ip}:8001/wallet/{node2}")
print(x.text)
