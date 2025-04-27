//+------------------------------------------------------------------+
//|                                        RSI only for M15andH1.mq5 |
//|                                  Copyright 2025, MetaQuotes Ltd. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2025, MetaQuotes Ltd."
#property link      "https://www.mql5.com"
#property version   "1.00"
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
// Input parameters
input string emailRecipient = "youremail@example.com"; // Replace with your email
input int rsiPeriod = 14; // RSI period for confirmation
input int rsiBuyLevel = 30; // Buy RSI level (Overbought)
input int rsiSellLevel = 70; // Sell RSI level (Oversold)

// Global variables to track the previous RSI values for each symbol
double prevRsiM15[], prevRsiM15Prev[], prevRsiH1[], prevRsiH1Prev[];
int trendDetected = 0; // 0 = No trend, 1 = Uptrend, -1 = Downtrend

// Initialization function
int OnInit()
{
   // Initialize arrays for RSI values for each symbol
   int totalSymbols = SymbolsTotal(true); // Get total number of symbols in the Market Watch

   ArrayResize(prevRsiM15, totalSymbols);
   ArrayResize(prevRsiM15Prev, totalSymbols);
   ArrayResize(prevRsiH1, totalSymbols);
   ArrayResize(prevRsiH1Prev, totalSymbols);

   // Initialize RSI values for all symbols
   for (int i = 0; i < totalSymbols; i++)
   {
      string symbol = SymbolName(i, true); // Get symbol name from the Market Watch
      prevRsiM15[i] = iRSI(symbol, PERIOD_M15, rsiPeriod, 1);
      prevRsiM15Prev[i] = iRSI(symbol, PERIOD_M15, rsiPeriod, 2);

      prevRsiH1[i] = iRSI(symbol, PERIOD_H1, rsiPeriod, 1);
      prevRsiH1Prev[i] = iRSI(symbol, PERIOD_H1, rsiPeriod, 2);
   }

   return INIT_SUCCEEDED;
}

// Main function
void OnTick()
{
   // Get the total number of symbols in the Market Watch
   int totalSymbols = SymbolsTotal(true);

   // Loop through each symbol in the Market Watch
   for (int i = 0; i < totalSymbols; i++)
   {
      string symbol = SymbolName(i, true); // Get symbol name from the Market Watch

      // Check for second confirmed upward movement on M15 RSI
      bool m15Upward = IsUpwardMovementRSI(symbol, prevRsiM15[i], prevRsiM15Prev[i]);
      // Check for second confirmed upward movement on H1 RSI
      bool h1Upward = IsUpwardMovementRSI(symbol, prevRsiH1[i], prevRsiH1Prev[i]);

      // If both timeframes have the same upward trend
      if (m15Upward && h1Upward)
      {
         trendDetected = 1;  // Upward trend detected on both timeframes
      }
      // Check for second confirmed downward movement on M15 RSI
      bool m15Downward = IsDownwardMovementRSI(symbol, prevRsiM15[i], prevRsiM15Prev[i]);
      // Check for second confirmed downward movement on H1 RSI
      bool h1Downward = IsDownwardMovementRSI(symbol, prevRsiH1[i], prevRsiH1Prev[i]);

      // If both timeframes have the same downward trend
      if (m15Downward && h1Downward)
      {
         trendDetected = -1; // Downward trend detected on both timeframes
      }

      // Update RSI values for both timeframes for the current symbol
      if (m15Upward || m15Downward)
      {
         prevRsiM15Prev[i] = prevRsiM15[i];
         prevRsiM15[i] = iRSI(symbol, PERIOD_M15, rsiPeriod, 1);  // Update M15 RSI values
      }

      if (h1Upward || h1Downward)
      {
         prevRsiH1Prev[i] = prevRsiH1[i];
         prevRsiH1[i] = iRSI(symbol, PERIOD_H1, rsiPeriod, 1); // Update H1 RSI values
      }

      // Send email notification when trend is detected
      if (trendDetected != 0)
      {
         string trend = trendDetected == 1 ? "Upward" : "Downward";
         string subject = "RSI Trend Detected: " + trend + " Movement on " + symbol;
         string body = "A " + trend + " movement has been confirmed on both M15 and H1 timeframes for the currency pair " + symbol;
         SendMail(subject, body);
         trendDetected = 0; // Reset the trend detection after sending email
      }
   }
}

// Function to check upward movement on RSI
bool IsUpwardMovementRSI(string symbol, double currentRsi, double prevRsi)
{
   return (currentRsi > prevRsi);
}

// Function to check downward movement on RSI
bool IsDownwardMovementRSI(string symbol, double currentRsi, double prevRsi)
{
   return (currentRsi < prevRsi);
}
