import requests


def post_transaction(sender, receiver, amount, type):
    url = 'http://localhost:8000/transact'
    json = '{"transaction": {"sender": "'+ sender +'","receiver": "'+ receiver +'", "amount": '+ amount +', "type": "'+ type +'"}}'
    r = requests.post(url, data = json)
    print(r.text)


post_transaction('pv1403d478bfc4949c9c68af53bbaf8deb58c4eac', 'pv171e654b041c51d978becc5cf65b0e2f48ff7', '5000', 'EXCHANGE')
post_transaction('pv1403d478bfc4949c9c68af53bbaf8deb58c4eac', 'pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7', '500', 'EXCHANGE')
post_transaction('pv1403d478bfc4949c9c68af53bbaf8deb58c4eac','pv171e654b041c51d978becc5cf65b0e2f48ff7', '1000', 'EXCHANGE')
post_transaction('pv171e654b041c51d978becc5cf65b0e2f48ff7','pv171e654b041c51d978becc5cf65b0e2f48ff7', '100', 'STAKE')
post_transaction('pv1403d478bfc4949c9c68af53bbaf8deb58c4eac','pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42', '300', 'EXCHANGE')
post_transaction('pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7', 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42','50', 'TRANSFER')
post_transaction('pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7', 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42','50', 'TRANSFER')
post_transaction('pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7', 'pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42','50', 'TRANSFER')