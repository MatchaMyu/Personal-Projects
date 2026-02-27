as currently stands, the first build is extremely simple. It communicates with the OS and just....prints a message and that's it. It halds cleanly since we tell it to,
but this isn't very constructive of an OS. An operating system does a lot more than this. Keyboards would be great to start, but I also need to detect if it faults and handles CPU exceptions.
There's also loading our Global Descriptor table and IDT. This version will do that and point to my own GDT and IDT. That way when there's a fault, it'll point to something I've put
into the programming.

So right now, it will run and immediately halt because I've implemented a crash into the system. The system prints out the hex numbers, as intended, although this is hardly readable
to most humans, it's the beginning of the process to tell what code broke.

I have to adjust the compile command for the new features and changes to the .elf when it crashes, so the error can be properly understood when it crashes.

After that, it'll be timer sequences and then keyboard entry. I'm not concerned with commands at the moment, merely that it can tell a key is pressed.
