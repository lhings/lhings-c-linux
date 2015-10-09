# Lhings C library for Linux.

This repository contains the Lhings C library for Linux systems. With this library you can connect 
Linux based devices to Lhings in a very easy way. 

For a quick introduction of Lhings, read the [support pages](http://support.lhings.com).

This readme provides the basics about how to compile and use the library, and also how to port it to other platforms.

## Compiling
The Linux port of the Lhings C library depends only on the standard C library and 
[libcurl](http://curl.haxx.se/libcurl/). The latter can be installed through the package managers
of almost any recent Linux distribution. 

You can compile the
project inmediately after cloning the git repository by executing make at the command line.

For ease of use, project files for both [Netbeans](https://netbeans.org/) and 
[Code::Blocks](http://www.codeblocks.org/) IDE's are provided. 

## Using the library
[Reference documentation of the library](http://lhings.github.io/lhings-c-linux/files.html) is provided using DOxygen. 

A simple complete example on how to use the library can be found in the file `main.c`.

The structure of a program that uses the library is very simple, it has to 
implement the functions `setup()` and `loop()` and, of course, a `main()` function. 

`setup()` is used to configure the 
device and is where all the initialization code should go. Configuring the device
means telling the Lhings library which are its actions, events and status components.

Actions, events and status components are added to the Lhings device using the 
functions `lh_model_add_action()`, `lh_model_add_event()` and `lh_model_add_status_component()`
respectively. This functions populate the struct `LH_Device this_device` adding to it
all the information needed so that the Lhings library is able to generate the 
[descriptor](http://support.lhings.com/The-Device-Descriptor.html) of the device.

The `loop()` function will be periodically executed at a rate which can be
changed calling `lh_set_loop_frequency_hz()` or `lh_set_loop_frequency_secs()`. You
can use this method to implement anything that your device needs to perform or
check periodically.

In addition to this, you can define action functions, which will be executed 
any time the corresponding action is requested from the Lhings mobile or web
apps, or using the [Lhings REST API](http://support.lhings.com/Lhings-API-Documentation.html).
These action functions should have the following signature:
```c
void action_function_example(LH_Dict *function_arguments)
```
A pointer to this functions must be passed to `lh_model_add_action()` so that
the Lhings library knows which function to call when the named action is
requested. 

Likewise, you can use variables in your code to define status components of 
your device, passing a pointer to them in the call to the function 
`lh_model_add_status_component()`. For instance, if your device is a sensor
that measures temperature and you store its value in a `float` variable
called `temp`, you can expose its value as a status component passing a 
pointer to `temp` to the aforementioned function. Then its real time value
could be checked from the mobile and web apps of Lhings and also using the 
Lhings REST API.

Whenever you need to send an event you can use the function `lh_send_event()`.

In order to start your device, you have to call the function
`lh_start_device()` from your `main()` function.

Finally a note on conventions used by the library:

* All library API functions start with `lh_*` . In the same way, all
library defined data types start with `LH_*` . 
* The library makes extensive use of dynamic memory allocation. Some functions
return pointers. By convention, this functions will always return a pointer to a
dynamically allocated memory block. In the case of primitive data types (`int` ,
`float`, `char`, etc) **the memory must be freed with `free()` when it is no longer
needed**. In the case of library defined data types (`LH_Device`, `LH_Action`, `LH_Dict`, `LH_List`, etc.)
always exists a function `lh_free_*` that must be used to properly release the 
memory allocated by those methods. The library itself is checked against memory leaks
using [Valgrind](http://valgrind.org) before each release. 

## Porting to other platforms
The library has been designed from the ground up with portability in mind. The platform dependent functionality
is grouped in the project under the directory `abstraction`, while all the platform independent features are
under the directory `core`. To port the library to other platforms the functions declared in the following
header files need to be implemented:

* `abstraction/http-comm/http_api.h`: provides all the functions the library needs to communicate using HTTP. 
Bear in mind that in order to communicate with Lhings HTTPS is needed.
* `abstraction/udp-comm/udp_api.h`: provides all the functions the library needs to communicate using UDP.
* `abstraction/timing/lhings_time.h`: provides all the functions the library needs to access system clock and timing.
* `abstraction/permanent-storage/storage_api.h`: provides access to the permanent storage of the device. 

These files define an API which the rest of the library uses to access platform dependent features. Implementation
details can be found in the [DOxygen documentation of each header file](http://lhings.github.io/lhings-c-linux/files.html).
