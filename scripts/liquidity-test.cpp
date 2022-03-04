#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    float SUPPLY = 1000000000;
    const float ORIGINAL_SUPPLY = SUPPLY;
    float LIQUIDITY = 20000000;
    float TOKEN_PRICE = LIQUIDITY / SUPPLY;
    const int NUMBER_OF_TRANSACTIONS = 20000000;
    const int PRICE_RANGE_LOW=20;
    const int PRICE_RANGE_HIGH=1000;
    float USD_PRICE=0;
    float NUM_TOKENS=0;
    time_t start, end;

    time(&start);
    for (int i = 0; i < NUMBER_OF_TRANSACTIONS; i++) {
        srand(time(nullptr));      
        USD_PRICE = rand() % PRICE_RANGE_HIGH + PRICE_RANGE_LOW;
        NUM_TOKENS = USD_PRICE / TOKEN_PRICE;
        
        if (NUM_TOKENS < SUPPLY) {
            if (i % 10 != 5 && i % 10 !=9 && i % 10 != 3 && i % 10 != 7) {
                // printf("Buying %f coins at %f per coin for %f\n", NUM_TOKENS, TOKEN_PRICE, USD_PRICE);
                SUPPLY -= NUM_TOKENS;
                LIQUIDITY += USD_PRICE;
            } else {
                // printf("Selling %f coins at %f per coin for %f\n", NUM_TOKENS, TOKEN_PRICE, USD_PRICE);
                SUPPLY += NUM_TOKENS;
                LIQUIDITY -= USD_PRICE;
            }
        }
        TOKEN_PRICE = LIQUIDITY / SUPPLY;
    }
    time(&end);
    
    double time_taken = double(end - start);
    cout << "Execution time : " << fixed
         << setprecision(5) << time_taken;
    cout << " sec " << endl;
    cout << "The number of coins purchased " << ORIGINAL_SUPPLY-SUPPLY << endl;
    cout << "The current price of our coin is " << TOKEN_PRICE << endl;
    cout << "There are " << SUPPLY << " coins left" << endl;
    cout << "The current market cap is " << LIQUIDITY << endl << endl;
    return 0;
}
