# AutoDST
An accessory to automatically switch to and back from daylight saving time.

## Usage

Choose your preferred language in the ``bins`` directory and copy its contents to your boot drive:

Copy ``AUTODST.ACC`` and ``AUTODST.INI`` to the root folder of your boot drive and edit the variables in ``AUTODST.INI`` as described below. Make sure to set the current DST status correctly!
When calling the accessory from the menu, it shows an alert box with information about the current date, time and DST status and the date and time when the next change will occur.

You can copy ``AUTODST.PRG`` to the ``AUTO`` folder of your boot drive to switch when booting if necessary.

Startup messages and error messages will be written to ``AUTODST.LOG``. DST status changes will be logged here, too.

## Configuration
``AUTODST.INI`` can be edited with any text editor. The sample INI is provided with explanatory comments.
However, the following variables can be set (cursives are optional):

``status`` current DST status (0=off, 1=on)

``tzdst`` time zone name for standard time (optional, is used in log messages and the alert box only)

``tzdst`` time zone name for daylight saving time (optional, is used in log messages and the alert box only)

``from`` rule for the beginning of daylight saving time

``to`` rule for returning to standard time

## Rules ##

Each rule consists of three fields separated by blank characters: day, month, time (24 hour format).

Day: Weekday (sun, mon, tue, wed, thu, fri, sat) followed by an indicator (1..4=first..fourth, -1..-4=last..fourth to last)
Month:  jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
Time: HH:MM

The time value can be 24:00 and beyond, so you can create rules such as "the day following the last Thursday in October" (see example for Egypt below).

### Examples ###

European Summer Time is from the last Sunday in March at 2:00 AM until the last Sunday in October at 3:00 AM:
```
from=Sun-1 Mar 02:00
to=Sun-1 Oct 03:00
```

USA: Second Sunday in March at 2:00 AM until first Sunday in November at 2:00 AM
```
from=Sun+2 Mar 02:00
to=Sun+1 Nov 02:00
```

Egyptian Summer Time in 2024 is from the last Friday in April at 12:00 AM until the day following the last Thursday in October at 12:00 AM.
```
from=Fri-1 Apr 00:00
to=Thu-1 Oct 24:00
```

## Translations ##

Adding new translations is very simple. To add e.g. Italian (Italy) translations:
* Copy ``lang/en_US.h`` to ``lang/it_IT.h``
* Make your translations in ``lang/it_IT.h``
* Add "``it_IT``" to the variable ``LANGUAGES`` in the ``Makefile``
* Run ``make it_IT`` or ``make``

After that you can find Italian executables in ``bins/it_IT``.

For the date and time format specifiers see *strftime(3P)*.<br/>
Please note that the following specifiers are not supported: ``%c``, ``%j``, ``%j``, ``%U``, ``%W``, ``%w``, ``%X``, ``%x``, ``%Z``.
