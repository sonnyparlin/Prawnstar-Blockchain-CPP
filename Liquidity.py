import random
import time
import requests
import sys

def get_current_liquidity(btcPrice):
    x = requests.get('https://api.coinstats.app/public/v1/tickers?exchange=binanceus&pair=BTC-USD')
    jsonResponse = x.json()
    liquidity = btcPrice * jsonResponse['tickers'][0]['price']
    return liquidity

def main():
    SUPPLY=2000000000
    ORIGINAL_SUPPLY = SUPPLY
    LIQUIDITY = get_current_liquidity(0.5)
    TOKEN_PRICE = LIQUIDITY / SUPPLY
    NUMBER_OF_TRANSACTIONS = 2000000
    PRICE_RANGE_LOW=20
    PRICE_RANGE_HIGH=10000
    formated_num_transactions = "{:,.0f}".format(NUMBER_OF_TRANSACTIONS)
    seed_value = random.randrange(sys.maxsize)

    if TOKEN_PRICE < 1:
        formatted_price = "{:,.8f}".format(TOKEN_PRICE)
    else:
        formatted_price = "{:,.2f}".format(TOKEN_PRICE)

    my_investment = "{:,.0f}".format(1000 / TOKEN_PRICE)
    print(f"\nToken Price is ${formatted_price}, my investment is $1000 which buys me {my_investment} tokens.")
    my_investment = 1000 / TOKEN_PRICE
    start = time.time()

    print(f"\nExecuting {formated_num_transactions} random transactions...")
    for i in range(NUMBER_OF_TRANSACTIONS):            

        random.seed(seed_value)
        USD_PRICE=random.randint(PRICE_RANGE_LOW,PRICE_RANGE_HIGH)
        NUM_TOKENS = USD_PRICE / TOKEN_PRICE
        iterations = NUMBER_OF_TRANSACTIONS / 2
        iterations += 10000

        if NUM_TOKENS < SUPPLY:
            if i < (iterations):
                SUPPLY -= NUM_TOKENS
                LIQUIDITY += USD_PRICE
            else:
                SUPPLY += NUM_TOKENS
                LIQUIDITY -= USD_PRICE

        TOKEN_PRICE = LIQUIDITY / SUPPLY
    end = time.time()

    execution = "{:,.2f}".format(end-start)
    print(f"\nExecution time was {execution} seconds.")

    formatted_remaining_supply = "{:,.0f}".format(ORIGINAL_SUPPLY-SUPPLY)
    formatted_supply = "{:,.0f}".format(SUPPLY)
    
    if TOKEN_PRICE < 1:
        formatted_price = "{:,.8f}".format(TOKEN_PRICE)
    else:
        formatted_price = "{:,.2f}".format(TOKEN_PRICE)
    formatted_liquidity = "{:,.0f}".format(LIQUIDITY)

    my_investment = "{:,.0f}".format(1000 * TOKEN_PRICE)

    print(f"\nAfter {formated_num_transactions} transactions:")
    print(f'The number of tokens purchased was {formatted_remaining_supply}')
    print(f'The current price of our coin is ${formatted_price}')
    print(f'We have {formatted_supply} tokens left')
    print(f'Market cap: ${formatted_liquidity}\n')
    print(f"My Investment of $1000 is now worth: ${my_investment}")

if __name__ == '__main__':
    main()
