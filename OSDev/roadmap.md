OS development roadmap.
End Goal Definition: "It runs Doom."

To create an old meme, it's not an OS until it can run Doom.

Challenges: I know next to nothing about Assembly, little about C, and honestly I haven't done much of any major programming outside of school assignments which I am super quick to forget because they never feel important.

Timing is also an issue. I don't have a deadline for this. I also don't have the intention to work on this regularly enough that it'll be done in a reasonable length. Estimates put it at 1 year, and that's being generous.

Steps:

X Get a bootable OS.

X Global Descriptor Table

X IDT load Entries (prints proof)

X Prints CPU Errors from table.

X Halts after UD2 Exception and DE Exception

X Displays errors in a new screen including hex code (Currently mostly unreadable to a person, but begins the procedure to map the code to faults)

X PIC Remapping

X IRQ Handling (Timers, display timers)

X Keyboard Recongition

X 1 basic shell command

NEXT:
More Basic SHELL commands.
Memory management.
Advanced SHELL commands. (Changing color of text?)
Graphic management (Basic. 320x200)
Hardware Identifiers (Basic, does not have to USE them, just IDENTIFY them)
DOOM Port. (GOAL. genericdoom)

LONG TERM:
Whatis hardware command (Detects current hardware signatures)
File system
Super Mario Brothers

What is NOT needed:
These would make things too complex.

Advanced GUI is a bit much. Command prompt similar style screen works, so long as it can launch DOOM. (Many OSes are capable of doing this without a GUI)
