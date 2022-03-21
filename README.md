# Modified CM4-IO-BASE-B RTC Code

Waveshare provides sample code for using the real time clock (RTC) on their CM4-IO-BASE-B board. The code is instructive, but I wanted something more useful. I've taken their example code and have modified it to make it more useful. The original code used a hard-coded date/timeof February 28, 2021 T 23:59. In my modification I now have the code using the system time. The modified code can now either read the time from the RTC to set the system time or read from the system time and write it to the RTC. The actions taken are dependent on the argument passed to the program at the command line. One of two arguments can be passed. PAssing both arguments will cause the program to run as though no argument is passed. When no arguments are passed, the program reads from the RTC and prints the time. 

`setsystem` - reads from the real time clock and writes to the system time. For this call to be successful, the program must be run as root (with `sudo`).

`setrtc` - Reads from the system time and writes to the real time clock.

For the original code, please refer to WaveShare. The original code can be found [here](https://www.waveshare.com/wiki/CM4-IO-BASE-B).