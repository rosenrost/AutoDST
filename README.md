# AutoDST

A small software package for Atari ST/STE/TT and such to automatically switch to and back from daylight saving time.

The package consists of an AUTO folder program to catch up with time changes at boot time if necessary and an accessory which sleeps in the background until the clock needs to be adjusted.

If you do not plan on running the machine at night, installing the AUTO folder program and not using the accessory is probably sufficient.

## Usage

Download the [latest release](https://github.com/rosenrost/AutoDST/releases/latest) package, choose your preferred language and copy the contents of that directory to your boot drive:

Copy ``AUTODST.ACC`` and ``AUTODST.INI`` to the root folder of the boot drive and edit the variables in ``AUTODST.INI`` as described below. Make sure to set the current DST status correctly!

When the accessory is called from the menu, an alert box will be displayed showing the current date and time of the next time change.

Copy ``AUTODST.PRG`` to the ``AUTO`` folder of the boot drive.

The accessory sleeps for one minute between checks. For the last 120 seconds before a time change, the sleep interval is reduced to just one second to be as accurate as possible. When started, it checks whether there was a time change since the last time the machine was switched off, too.

Startup and error messages are written to ``AUTODST.LOG``. DST status changes are also logged here.

## Configuration

``AUTODST.INI`` can be edited with any text editor. The example file is provided with explanatory comments.<br/>

The following variables are used:

``status`` Current DST status (0=off, 1=on)

``tzdst`` Name of the time zone for standard time (optional, only used in the log messages and the accessory's alert box)

``tzdst`` Name of the time zone for daylight saving time (optional, only used in the log messages and the accessory's alert box)

``from`` Rule for the start of daylight saving time

``to`` Rule for the end of daylight saving time

Also note the following rules:
* Spaces at the beginning of a line will be removed.
* Blank lines are ignored.
* Lines starting with "#" are considered comments and are therefore also ignored.
* It is not possible to add comments at the end of a line.

After reading and evaluating ``AUTODST.INI``, the result is saved in binary format in ``AUTODST.CFG``. This eliminates the need to completely parse the ``INI`` file the next time it is started; instead, the configuration is read directly from the small ``CFG`` file. It is only used when a change has been made to the ``INI`` file (i.e., the ``INI`` file is newer than the ``CFG`` file).

## Rules ##

Each rule consists of three fields separated by spaces: day, month, time (24-hour format).

Day: weekday (Sun, Mon, Tue, Wed, Thu, Fri, Sat) followed by an indicator (1..4=first..fourth, -1..-4=last..fourth to last)<br/>
Month: Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec<br/>
Time: HH:MM

The time value can also be 24:00 and beyond, so you can create rules like "the day following the last Thursday in October" (see example for Egypt below).

### Examples ###

European Summer Time is from the last Sunday in March at 2:00 AM to the last Sunday in October at 3:00 AM:
```
from=Sun-1 Mar 02:00
to=Sun-1 Oct 03:00
```

USA: Second Sunday in March at 2:00 AM to first Sunday in November at 2:00 AM
```
from=Sun+2 Mar 02:00
to=Sun+1 Nov 02:00
```

Egyptian Summer Time in 2024 is from the last Friday in April at 12:00 AM to the day following the last Thursday in October at 12:00 AM.
```
from=Fri-1 Apr 00:00
to=Thu-1 Oct 24:00
```

## Compiling ##

To compile the software, Vincent Riviäre's *m68k-atari-mint* cross-tools and the *libcmini* library are required.

Just type ``make`` to compile for all languages. Or, for example, ``make de_AT`` to compile for Austrian German only.

If the library is not installed in ``/usr/local/lib/libcmini``, the path must be specified when calling ``make``:

```
$ make LIBCMINI_DIR=/your/path/to/libcmini
```
Or for e.g. British English only:
```
$ make LIBCMINI_DIR=/your/path/to/libcmini en_UK
```

## Translations ##

Adding new translations is very easy. For example, to add Italian (Italy) translations:
* Copy ``lang/en_US.h`` to ``lang/it_IT.h``
* Make your translations in ``lang/it_IT.h``
* Run ``make it_IT`` or ``make``

After that, you can find Italian executables in ``bins/it_IT``.

For date and time format specifiers, see *strftime(3P)*.<br/>
Please note that the following specifiers are not supported: ``%c``, ``%j``, ``%j``, ``%U``, ``%W``, ``%w``, ``%X``, ``%x``, ``%Z``.
