/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.24 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : BASIC_Performance.c
Purpose     : Performance test program

              It checks the performance of the entire system, outputing one
              result value per second. Higher values are better.
              The program computes the first 1000 prime numbers in a loop.
              The value output is the number of times this could be performed
              in a second.
              Please note that this test does not check the driver performance.
Requirements: WindowManager - ( )
              MemoryDevices - ( )
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

#include <string.h>
#include "GUI.h"

/*******************************************************************
*
*       Static data
*
********************************************************************
*/
static char _aIsPrime[1000];
static int  _NumPrimes;

/*******************************************************************
*
*       Static Code
*
********************************************************************
*/
/*********************************************************************
*
*       _CalcPrimes
*/
static void _CalcPrimes(int NumItems) {
  int i;

  //
  // Mark all as potential prime numbers
  //
  memset(_aIsPrime, 1, NumItems);
  //
  // 2 deserves a special treatment
  //
  for (i = 4; i < NumItems; i += 2) {
    _aIsPrime[i] = 0;        // Cross it out: not a prime
  }
  //
  // Cross out multiples of every prime starting at 3. Crossing out starts at i^2.
  //
  for (i = 3; i * i < NumItems; i++) {
    if (_aIsPrime[i]) {
      int j;

      j = i * i;             // The square of this prime is the first we need to cross out
      do {
        _aIsPrime[j] = 0;    // Cross it out: not a prime
        j += 2 * i;          // Skip even multiples (only 3*, 5*, 7* etc)
      } while (j < NumItems);
    }
  }
  //
  // Count prime numbers
  //
  _NumPrimes = 0;
  for (i = 2; i < NumItems; i++) {
    if (_aIsPrime[i]) {
      _NumPrimes++;
    }
  }
}

/*********************************************************************
*
*       _PrintDec
*/
static void _PrintDec(int v) {
  int Digit;
  int r;

  Digit = 10;
  while (Digit < v) {
    Digit *= 10;
  }
  do {
    Digit /= 10;
    r = v / Digit;
    v -= r * Digit;
    GUI_DispChar(r + '0');
  } while (v | (Digit > 1));
}

/*********************************************************************
*
*       _PrintResult
*/
static void _PrintResult(int Cnt) {
  if (_NumPrimes != 168) {
    GUI_DispString("Error");
  } else {
    GUI_DispString("Loops/sec: ");
    _PrintDec(Cnt);
  }
  GUI_DispString("\n");
}

/*******************************************************************
*
*       Public Code
*
********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
  int Cnt;
  int TestTime;

  GUI_Init();
  while(1) {
    Cnt = 0;
    TestTime = GUI_GetTime() + 1000;
    while ((TestTime - GUI_GetTime()) >= 0) {
      _CalcPrimes(sizeof(_aIsPrime));
      Cnt++;
    }
    _PrintResult(Cnt);
  }
}

/*************************** End of file ****************************/
