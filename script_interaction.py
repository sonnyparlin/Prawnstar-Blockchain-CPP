import requests


def post_transaction1():
    url = 'http://localhost:8000/transact'
    package = {"transaction":{"sender": "00000xxxxxxxxxxxxxxxxxxxxxx", "receiver": "2342342342234234234234wwerwer234","amount": 500, "type": "EXCHANGE"}}
    request = requests.post(url, json=package)
    print(request.json())

def post_transaction2():
    url = 'http://localhost:8000/transact'
    package = {"transaction":{"sender": "2342342342234234234234wwerwer234", "receiver": "81f298c9ab974271874cb1ddab83e066","amount": 50,"type": "TRANSFER"}}
    request = requests.post(url, json=package)
    print(request.json())


if __name__ == '__main__':
    #forger: genesis
    post_transaction1()
    post_transaction2()
    
