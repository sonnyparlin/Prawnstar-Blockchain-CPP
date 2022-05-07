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

start_time = time.time()
exchange = 'pv1403d478bfc4949c9c68af53bbaf8deb58c4eac'
nodeWallet = 'pv171e654b041c51d978becc5cf65b0e2f48ff7'
alice = 'pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7'
bob = 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42'
node2 = 'pv17bf144b32a8522fece3261b0a9fb93a3e2255b42'

print("\n\n\tInitiating Python test suite for Prawnstar Blockchain\n\n")

i=0
with requests.Session() as s:
    print("Attempting first transaction from exchange to this node's wallet")
    post_transaction(s, exchange, nodeWallet, '1', 'EXCHANGE')
    print("Attempting second transaction from exchange to node2's wallet")
    post_transaction(s, exchange, node2, '4', 'EXCHANGE')
    print("Attempting third transaction from exchange to alice's wallet for 20000 tokens")
    post_transaction(s, exchange, alice, '20000', 'EXCHANGE')
    print("Attempting to stake 3 tokens on Node #2")
    post_transaction(s, node2, node2, '3', 'STAKE')

    print(f"Attempting {sys.argv[1]} transactions on network with at least two nodes running...")
    for x in range(int(sys.argv[1])):
        i += 1
        if i % 100 == 0:
            print(f"{i} transactions...")
        post_transaction(s, alice, bob, '1', 'TRANSFER')
        #time.sleep(0.5)

#print(f"{sys.argv[1]} transactions complete: getting final balances...")
print("--- %s seconds ---\n\n" % (time.time() - start_time))
time.sleep(1)

print("Alice's wallet should be 18905: ")
x = requests.get(f"http://{ip}:8001/wallet/{alice}")
aw = json.loads(x.text)
if float(aw['amount']) == 18905:
    print("TRUE")
else:
    print(f"FALSE, got {aw['amount']} instead")

print("Bob's wallet should be 995: ")
x = requests.get(f"http://{ip}:8001/wallet/{bob}")
bw = json.loads(x.text)
if float(bw['amount']) == 995:
    print("TRUE")
else:
    print(f"FALSE, got {bw['amount']} instead")

x = requests.get(f"http://{ip}:8001/wallet/{nodeWallet}")
n1 = x

x = requests.get(f"http://{ip}:8001/wallet/{node2}")
n2 = x

x1 = json.loads(n1.text)
x2 = json.loads(n2.text)
print("Combined rewards earned by node1 and node2 should be 107.00")
if float(x1['amount']) + float(x2['amount']) == 107:
    print("TRUE")
else:
    print(f"FALSE, got {float(x1['amount']) + float(x2['amount'])} instead")

print("\nPython test suite complete.")
