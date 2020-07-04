# MHW - Console Plugin - Client Helper
Client file to include to interact with console.

`ConsoleServer.dll` and `MHW Console Plugin.dll` goes to `Monster Hunter World\nativePC\plugins` folder, they may expire when game updated



## I/O

The console takes what ever you input in MHW:I's chat bar that start with char '/'.

Following the order of `/PLUGINNAME COMMANDS` the plugin broadcast `PLUGINNAME COMMANDS` as string to any plugin that connected with.

The `PLUGINNAM` don't have to be its actual name, its just a identifier as long as your plugin command don't conflict with others.



`/PLUGINNAME help ` is a reserved command for plugin read the description file of the plugin.

It takes any string that start with `help` that can branch to other descriptions.

Help file should located in `Monster Hunter World\nativePC\plugins\ConsoleHelp`  and naming with `PLUGINNAMEHelp.json`



Here is an example plugin help named `autofarmHelp.json` from my [Auto Fertilizer Plugin](https://www.nexusmods.com/monsterhunterworld/mods/3626)

```json
{
   "help":[
      "usage: /autofarm [option]\nOption list:",
      "harvest   : harvest now\nfertilize   : add soft soil now",
      "remain    : display fert remaining times\nreload    : reload the plugin to refresh address and config"
   ],
   "helpcn":[
      "使用方式: /restock [选项]\n选项列表:",
      "harvest   : 立即收获\nfertilize   : 立即添加软土",
      "remain    : 显示剩余肥料\nreload    : 重新加载插件"
   ]
}
```



## Usage

```c++
#include "mhw_console.h"
```

Depends on your code, you may need to modify the `.h` or `.cpp` to eliminate error.



Console using TCP socket in localhost, with port number of 50382 (you known what it means :D).

```c++
SOCKET s;
```

In your main, you can establish connection with console with:

```c++
ConsoleEnable = TCPConnect();
```

It returns a state of whether it successfully connect with console.

Then with your looping part, you could use this:

```c++
memset(buffer, 0, sizeof(buffer));
recv(s, buffer, 1024, 0);
```

to listen for incoming command.



LognSend: Log (the message into loader's log) and Send (the message to console to display in game).


```c++
void LognSend(std::string str);
```



Code reference see in related section's comments