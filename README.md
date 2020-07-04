# MHW - Console Plugin - Client Helper
Client file to include to interact with console.

```
#include "mhw_console.h"
```

Depends on your code, you may need to modify the `.h` or `.cpp` to eliminate error.



Console using TCP socket in localhost, with port number of 50382 (you known what it means :D).

```
SOCKET s;
```

In your main, you can establish connection with console with:

```
ConsoleEnable = TCPConnect();
```

It returns a state of whether it successfully connect with console.

Then with your looping part, you could use this:

```
memset(buffer, 0, sizeof(buffer));
recv(s, buffer, 1024, 0);
```

to listen for incoming command.



LognSend: Log (the message into loader's log) and Send (the message to console to display in game).


```
void LognSend(std::string str);
```