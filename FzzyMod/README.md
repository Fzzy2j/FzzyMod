General -> Project Defaults -> Character Set = Use Multi-Byte Character Set
C/C++ -> Precompiled Headers -> Precompiled Header = Not Using Precompiled Headers
Linker -> Input > Module Definition File = midimap.def (the .def file you copied to the project folder)

Right click the project -> Build Dependencies -> Build Customizations and then check ".masm". This will allow the .asm file to work correctly.