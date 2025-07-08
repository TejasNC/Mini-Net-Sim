#include <bits/stdc++.h>
using namespace std;

class Solution {
    public:
        long long maximumProfit(vector<int>& prices, int k) {

            int n = prices.size();

            // dp[i][j][k] => max profit till day i with j transactions left and state k (0 = hold, 1 = one stock held, 2 = one stock owed (shorted))
            vector<vector<vector<long long>>> dp(n + 1, vector<vector<long long>>(k + 1, vector<long long>(3, 0)));

            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= k; ++j) {

                    dp[i][j][0] = max(dp[i - 1][j][0], dp[i - 1][j][1] + prices[i - 1]); // sell stock on day i and square off held stock
                    dp[i][j][0] = max(dp[i][j][0], dp[i - 1][j][2] - prices[i - 1]); // buy stock on day i and square off shorted stock

                    // Transition for holding one stock
                    dp[i][j][1] = max(dp[i - 1][j][1], dp[i - 1][j - 1][0] - prices[i - 1]);

                    // Transition for shorting a stock
                    dp[i][j][2] = max(dp[i - 1][j][2], dp[i - 1][j - 1][0] + prices[i - 1]);
                }
            }

            // The maximum profit will be the maximum of not holding any stock at the end of the last day
            long long maxProfit = 0;
            for (int j = 0; j <= k; ++j) {
                maxProfit = max(maxProfit, dp[n][j][0]);
            }

            return maxProfit;
        }
    };
