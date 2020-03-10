# All the tools/libraries NURC uses to make fighting robots!

Includes a PS3 and PS4 library for an esp32. Also initializes all everything needed to ensure motors turn on and off safely when controllers disconnect

Requires call of `safetySetup()` in the setup function and `safetyLoop()` in the loop function for controllers to connect. `safetySetup()` takes a function which will disable your motors when the controller disconnects or the esp32 is delayed for more than one second and a value to indicate which controller you using.

See the examples to see how to use this code.
