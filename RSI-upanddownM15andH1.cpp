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

// Global variables to track the previous RSI values
double prevRsiM15, prevRsiM15Prev;
double prevRsiH1, prevRsiH1Prev;
int trendDetected = 0; // 0 = No trend, 1 = Uptrend, -1 = Downtrend

// Initialization function
int OnInit()
{
   // Initialize previous RSI values for both timeframes
   prevRsiM15 = iRSI(Symbol(), PERIOD_M15, rsiPeriod, 1);
   prevRsiM15Prev = iRSI(Symbol(), PERIOD_M15, rsiPeriod, 2);

   prevRsiH1 = iRSI(Symbol(), PERIOD_H1, rsiPeriod, 1);
   prevRsiH1Prev = iRSI(Symbol(), PERIOD_H1, rsiPeriod, 2);

   return INIT_SUCCEEDED;
}

// Main function
void OnTick()
{
   // Check for second confirmed upward movement on M15 RSI
   bool m15Upward = IsUpwardMovementRSI(PERIOD_M15, prevRsiM15, prevRsiM15Prev);
   // Check for second confirmed upward movement on H1 RSI
   bool h1Upward = IsUpwardMovementRSI(PERIOD_H1, prevRsiH1, prevRsiH1Prev);
   
   // If both timeframes have the same upward trend
   if (m15Upward && h1Upward)
   {
      trendDetected = 1;  // Upward trend detected on both timeframes
   }
   // Check for second confirmed downward movement on M15 RSI
   bool m15Downward = IsDownwardMovementRSI(PERIOD_M15, prevRsiM15, prevRsiM15Prev);
   // Check for second confirmed downward movement on H1 RSI
   bool h1Downward = IsDownwardMovementRSI(PERIOD_H1, prevRsiH1, prevRsiH1Prev);

   // If both timeframes have the same downward trend
   if (m15Downward && h1Downward)
   {
      trendDetected = -1; // Downward trend detected on both timeframes
   }

   // Update RSI values for both timeframes
   if (m15Upward || m15Downward)
   {
      prevRsiM15Prev = prevRsiM15;
      prevRsiM15 = iRSI(Symbol(), PERIOD_M15, rsiPeriod, 1);  // Update M15 RSI values
   }
   
   if (h1Upward || h1Downward)
   {
      prevRsiH1Prev = prevRsiH1;
      prevRsiH1 = iRSI(Symbol(), PERIOD_H1, rsiPeriod, 1); // Update H1 RSI values
   }

   // Send email notification when trend is detected
   if (trendDetected != 0)
   {
      string trend = trendDetected == 1 ? "Upward" : "Downward";
      string subject = "RSI Trend Detected: " + trend + " Movement on " + Symbol();
      string body = "A " + trend + " movement has been confirmed on both M15 and H1 timeframes for the currency pair " + Symbol();
      SendMail(subject, body);
      trendDetected = 0; // Reset the trend detection after sending email
   }
}

// Function to check upward movement on RSI
bool IsUpwardMovementRSI(int timeframe, double currentRsi, double prevRsi)
{
   return (currentRsi > prevRsi);
}

// Function to check downward movement on RSI
bool IsDownwardMovementRSI(int timeframe, double currentRsi, double prevRsi)
{
   return (currentRsi < prevRsi);
}
