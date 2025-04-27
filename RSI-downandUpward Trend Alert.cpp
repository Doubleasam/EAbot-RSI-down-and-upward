//+------------------------------------------------------------------+
//|                                      RSI upward and downward.mq5 |
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
//|                                RSI Upward and Downward EA with Symbol |
//+------------------------------------------------------------------+
#property strict

input int RSI_Period = 14; // RSI Period

// RSI indicator handles
int rsiHandleM15, rsiHandleH1, rsiHandleH4;

// Buffers to hold RSI values
double rsiBufferM15[], rsiBufferH1[], rsiBufferH4[];

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
   // Create RSI handles
   rsiHandleM15 = iRSI(_Symbol, PERIOD_M15, RSI_Period, PRICE_CLOSE);
   rsiHandleH1  = iRSI(_Symbol, PERIOD_H1,  RSI_Period, PRICE_CLOSE);
   rsiHandleH4  = iRSI(_Symbol, PERIOD_H4,  RSI_Period, PRICE_CLOSE);

   if(rsiHandleM15 == INVALID_HANDLE || rsiHandleH1 == INVALID_HANDLE || rsiHandleH4 == INVALID_HANDLE)
     {
      Print("Error creating RSI indicator handles");
      return INIT_FAILED;
     }

   return INIT_SUCCEEDED;
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   IndicatorRelease(rsiHandleM15);
   IndicatorRelease(rsiHandleH1);
   IndicatorRelease(rsiHandleH4);
  }

//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
   static datetime last_checked = 0;

   // Check only once per new M15 bar
   if (iTime(_Symbol, PERIOD_M15, 0) == last_checked)
      return;

   last_checked = iTime(_Symbol, PERIOD_M15, 0);

   // Refresh and read RSI values
   CopyBuffer(rsiHandleM15, 0, 0, 2, rsiBufferM15);
   CopyBuffer(rsiHandleH1,  0, 0, 2, rsiBufferH1);
   CopyBuffer(rsiHandleH4,  0, 0, 2, rsiBufferH4);

   // Check trends
   bool isDownM15 = (rsiBufferM15[0] < rsiBufferM15[1]);
   bool isUpM15   = (rsiBufferM15[0] > rsiBufferM15[1]);

   bool isDownH1  = (rsiBufferH1[0]  < rsiBufferH1[1]);
   bool isUpH1    = (rsiBufferH1[0]  > rsiBufferH1[1]);

   bool isDownH4  = (rsiBufferH4[0]  < rsiBufferH4[1]);
   bool isUpH4    = (rsiBufferH4[0]  > rsiBufferH4[1]);

   // Prepare message
   string message = "";

   if(isDownM15) message += StringFormat("RSI is trending DOWN on %s M15 timeframe.\n", _Symbol);
   if(isUpM15)   message += StringFormat("RSI is trending UP on %s M15 timeframe.\n", _Symbol);

   if(isDownH1)  message += StringFormat("RSI is trending DOWN on %s H1 timeframe.\n", _Symbol);
   if(isUpH1)    message += StringFormat("RSI is trending UP on %s H1 timeframe.\n", _Symbol);

   if(isDownH4)  message += StringFormat("RSI is trending DOWN on %s H4 timeframe.\n", _Symbol);
   if(isUpH4)    message += StringFormat("RSI is trending UP on %s H4 timeframe.\n", _Symbol);

   // If any trend detected, send email
   if (message != "")
     {
      string subject = StringFormat("RSI Alert on %s", _Symbol);
      SendMail(subject, message);
      Print(message);
     }
  }
//+------------------------------------------------------------------+
