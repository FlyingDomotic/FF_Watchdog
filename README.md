# FF_Watchdog
Implements an hardware watchdog using an ATtiny85

## What's for?

This code resets an external controller when not receiving trigger pulses for 20s (30s after this MCU boot or after external MCU reset)

## Installation

Clone repository somewhere on your disk.
```
cd <where_you_want_to_install_it>
git clone https://github.com/FlyingDomotic/FF_Watchdog.git FF_Watchdog
```

## Update

Go to code folder and pull new version:
```
cd <where_you_installed_FF_Waatchdog>
git pull
```

Note: if you did any changes to files and `git pull` command doesn't work for you anymore, you could stash all local changes using:
```
git stash
```
or
```
git checkout <modified file>
```

## ATtiny85 hardware connections

```
           +-----+
    N/C 1 -+ O   +- 8 VCC (3 to 5V)
    N/C 2 -+     +- 7 N/C
    N/C 3 -+     +- 6 Trigger (from external MCU)
    GND 4 -+     +- 5 Reset (to external MCU through a 470 Ohms resistor)
           +-----+
```

## Connections when programming ATtiny with an Arduino

```
    Arduino - ATtiny
      D10   -   1
      D11   -   5
      D12   -   6
      D12   -   7
      GND   -   4
       5V   -   8
```
