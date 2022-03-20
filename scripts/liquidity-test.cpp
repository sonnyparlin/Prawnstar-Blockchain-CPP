#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int main() {
    double SUPPLY = 1000000000;
    const double ORIGINAL_SUPPLY = SUPPLY;
    double LIQUIDITY = 20000;
    double TOKEN_PRICE = LIQUIDITY / SUPPLY;
    const int NUMBER_OF_TRANSACTIONS = 10000;
    const int PRICE_RANGE_LOW=1;
    const int PRICE_RANGE_HIGH=700;
    double USD_PRICE=0;
    double NUM_TOKENS=0;
    time_t start, end;
    std::vector<double> sales;


    time(&start);
    srand(time(nullptr));

    for (int i = 0; i < NUMBER_OF_TRANSACTIONS; i++) {
        USD_PRICE = rand() % PRICE_RANGE_HIGH + PRICE_RANGE_LOW;
        NUM_TOKENS = USD_PRICE / TOKEN_PRICE;

        if (USD_PRICE > PRICE_RANGE_HIGH) {
            USD_PRICE = PRICE_RANGE_HIGH;
        }
        
        sales.push_back(USD_PRICE);

        if (NUM_TOKENS < SUPPLY) {
            if (i % 10 != 5 && i % 10 !=9 && i % 10 != 3 && i % 10 != 7) {
                // printf("Buying %f coins at %f per coin for %f\n", NUM_TOKENS, TOKEN_PRICE, USD_PRICE);
                SUPPLY -= NUM_TOKENS;
                LIQUIDITY += USD_PRICE;
                // cout << "buy" << endl;
            } else {
                // printf("Selling %f coins at %f per coin for %f\n", NUM_TOKENS, TOKEN_PRICE, USD_PRICE);
                SUPPLY += NUM_TOKENS;
                LIQUIDITY -= USD_PRICE;
                // cout << "sell" << endl;
            }
        }
        TOKEN_PRICE = LIQUIDITY / SUPPLY;
    }
    time(&end);

    double time_taken = double(end - start);
    cout << "Execution time : " << fixed
         << setprecision(5) << time_taken;
    cout << " sec \n" << endl;    

    cout << "After " << NUMBER_OF_TRANSACTIONS << " transactions: " << endl;
    cout << "The number of coins purchased " << std::fixed << std::setprecision(0) << ORIGINAL_SUPPLY-SUPPLY << endl;
    
    if (TOKEN_PRICE < 1)
        cout << "The current price of our coin is $" << std::fixed << std::setprecision(10) << TOKEN_PRICE << endl;
    else
        cout << "The current price of our coin is $" << std::fixed << std::setprecision(2) << TOKEN_PRICE << endl;
    
    double average = accumulate(sales.begin(), sales.end(), 0) / static_cast<double>(sales.size());
    cout << "Average purchase USD: $" << std::fixed << std::setprecision(2) << average << endl;
    
    double high = *max_element(sales.begin(), sales.end());
    cout << "Highest purchase USD: $" << std::fixed << std::setprecision(2) << high << endl;

    cout << "There are " << std::fixed << std::setprecision(0) << SUPPLY << " coins left" << endl;
    cout << "The current market cap is $" << std::fixed << std::setprecision(2) << LIQUIDITY << endl << endl;
    return 0;
}
