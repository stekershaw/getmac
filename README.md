About this code
===============

First and foremost, consider not using it...

This code was written a few years ago to solve a problem that is now solved by *ethtool* since version 2.6.36 (late 2010).

The problem it solves is to print the permanent 'burned in' MAC address for a given interface on a Linux host.

**The reason for wanting this?** Network testing where it was desirable to change the MAC address temporarily but always revert back to the proper MAC address.

**How it does this?** ioctl.  

Building
========

It's just *c* code so compile with *gcc*:
```
gcc getmac.c > getmac
```

Usage
=====

If built as above and not installed in your $PATH:
```
./getmac eth0
```

A single argument of an interface name is accepted, with eth0 assumed if none given.

The output is the the MAC (i.e. physical) address of the interface, hex formatted, without separators between hex groups.
