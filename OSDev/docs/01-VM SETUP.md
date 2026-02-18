01-VM SETUP

I went with QEMU to install this. I had to look up a youtube video to get the correct commands for this.

https://youtu.be/T68CTiudCVI?si=y1oeQvI0tVsGeHlg

I chose Linux to install QEMU.
Command: sudo apt install qemu-kvm libvirt-clients libvirt-daemon-system virt-manager ovmf swtpm swtpm-tools virtiofsd bridge-utils

THEN

sudo usermod -aG libvirt $USER

This command enables QEMU to function correctly by giving the libvirt the proper commands.

INSTALLING:
For installing, I decided to use Debian. While there are dozens of operating systems, I only need the minimalist approach for this. Also, these are much easier to install than many ISOs. 

https://www.debian.org/distrib/

When downloading the ISO, I find it doesn't execute very well. The command: "qemu-system-x86_64 -m 1024 -cdrom ~/Downloads/debian-*.iso" Allowed it to run (NOTE: It was located in the downloads folder)

VM options:
I went with 1024MB of ram, 1 CPU, and 10GB. This is a minimalist build again. I could do 512 if I really wanted.

Username? mewmew

VM RUNNING:
Because I have a username, I have to log on and enable sudo....
Run the commands:
su -
apt update
apt upgrade
apt install sudo
usermod -aG sudo USERNAME

This will allow me to use my username and sudo commands like I would in Linux.

Moreover:
sudo apt install build-essential git make gcc nasm qemu-system-x86
This installs the compiler, assembler, build tools, git, and qemu.

CONNECTING TO THE VM:

Connecting to the VM from the primary machine is crucial. If I can't connect to it, I can't copypaste, and this has A LOT of functions.
I didn't know about SSH before, but now I do!

SSH:
Inside the VM:
sudo apt install qemu-guest-agent
sudo systemctl enable --now qemu-guest-agent
sudo systemctl start spice-vdagentd

sudo apt install openssh-server
ip a (This will list 3 IPs. It's NOT the loopback, but the SECOND IP)

From the HOST terminal (NOT THE VM!)
ssh VM_USERNAME@VM_IP