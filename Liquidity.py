import random
import time

def main():
    SUPPLY=200000000
    ORIGINAL_SUPPLY = SUPPLY
    LIQUIDITY = 20000
    TOKEN_PRICE = LIQUIDITY / SUPPLY
    NUMBER_OF_TRANSACTIONS = 10000
    PRICE_RANGE_LOW=20
    PRICE_RANGE_HIGH=500

    start = time.time()
    for i in range(NUMBER_OF_TRANSACTIONS):
        USD_PRICE=random.randint(PRICE_RANGE_LOW,PRICE_RANGE_HIGH)
        NUM_TOKENS = USD_PRICE / TOKEN_PRICE

        if NUM_TOKENS < SUPPLY:
            if i % 10 != 5 and i % 10 != 9:
                SUPPLY -= NUM_TOKENS
                LIQUIDITY += USD_PRICE
            else:
                SUPPLY += NUM_TOKENS
                LIQUIDITY -= USD_PRICE

        TOKEN_PRICE = LIQUIDITY / SUPPLY
    end = time.time()

    print(f'\n\nExecution time was {end-start}')

    formatted_remaining_supply = "{:,.0f}".format(ORIGINAL_SUPPLY-SUPPLY)
    formatted_supply = "{:,.0f}".format(SUPPLY)
    if TOKEN_PRICE < 1:
        formatted_price = "{:,.8f}".format(TOKEN_PRICE)
    else:
        formatted_price = "{:,.2f}".format(TOKEN_PRICE)
    formatted_liquidity = "{:,.0f}".format(LIQUIDITY)

    print(f'\nThe number of tokens purchased {formatted_remaining_supply}')
    print(f'The current price of our coin is ${formatted_price}')
    print(f'We have {formatted_supply} tokens left')
    print(f'Market cap: ${formatted_liquidity}\n\n')

if __name__ == '__main__':
    main()
