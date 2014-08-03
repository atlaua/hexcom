# hexcom

This simple program opens a path (most likely a char device), reads from it and prints it to stdout
in byte-wise hexadecimal encoding. If there's no data available, it instead reads hexadecimal bytes
from stdin and writes them to the device.

I created *hexcom* for debugging a device offering a custom binary protocol over a serial interface,
but to test it, it can also be hooked up to a pty, as shown below.

Currently, *hexcom* is only a quick hack, but perhaps I'll improve it in the future (speaking of
which, perhaps I'd be better to write a hexadecimal mode for [picocom][picocom].

[picocom]: https://code.google.com/p/picocom/


## Example

The following listing shows *hexcom* in action. (Sorry for the bad layout, I couldn't think of
anything better.)

```
                [ terminal #1 ]                   |                [ terminal #2 ]
--------------------------------------------------|-------------------------------------------------
                                                  |    $ socat PTY,link=pty,echo=0 STDIO
    $ ./hexcom pty                                |
--------------------------------------------------|-------------------------------------------------
                                                  |      ABC
      41 42 43 0a                                 |
--------------------------------------------------|-------------------------------------------------
      61                                          |
      62                                          |
      63                                          |
                                                  |      abc
```
