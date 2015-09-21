/*
 * Copyright (c) 2015 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _DRIVER_STATION_TIMES_H
#define _DRIVER_STATION_TIMES_H

/*
 * QTimer objects always process a little after the timeout has happened,
 * we use this constant in time-critical functions to ensure that everything
 * happens at about the value that we really want to use.
 * In other words, we tell the timer to timeout a little before the value that
 * we really need.
 */
#define _SAFE_CONSTANT 2

/**
 * Defines all the time intervals of the library in a single file.
 * \note Change these values as needed.
 */
namespace DS_Times
{
const int ElapsedTimeInterval (100);
const int SafetyDisableTimeout (750);
const int TestConnectionInterval (500);
const int ControlPacketInterval (20 - _SAFE_CONSTANT);
}

#endif /* _DRIVER_STATION_TIMES_H */
