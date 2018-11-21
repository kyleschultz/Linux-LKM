Kyle Schultz

PA2

Files inlcuded in submission:

simple_char_driver.c - Simple Character device driver that can read, write, and seek. This is a character device driver
that will read from the device character by character, writes to the device character by character and 
operates in blocking mode. Write writes to the file and the file will be concatenated with the new characters 
that are to be inserted if it is within the proper bounds. Read will read an amount of bytes that will be specified by the user.
If the user tries to read too much, only what is left of the buffer will be read. Seek will set the current file position
depending on the whence value. If whence is 0, the currpent position of the file will be set to value of the offset. 
If whence is 1, the current position of the file will be incremented by the value of the offset. 
If whence is 2 the current position of the file will be set to the offset bytes before the end of the file. 


Makefile - Contains obj-m:=simple_char_driver.o. m means module and it is telling the compiler to create
a module named the simple_char_driver.o. To run the makefile
type the command: 'make –C /lib/modules/$(uname -r)/build M=$PWD modules' 
This will create the kernel module (.ko) object will insert in the basic kernel image

simple_char_test.c - Interactive test program to read, write, and seek from the device file. Press 'r' to read from
the device and specify the bytes to read. Press 'w' to write to the file and enter the data to write. Press 's'
to seek to a position in the file and specify an offset and whence value. Press 'e' to quit the test app.
To complie, type: 'gcc -o simple_char_test simple_char_test.c'
To run, type: './simple_char_test'

How to install and test LKM:

Create a device file for the device file in the dev folder using the command: 
'sudo mknod -m 777 /dev/simple_character_device c 261 0'. 
261 is the device major number, if it already taken, change it to a number that is not. 0 is the minor number.
This file will be the device file associated with the simple_char_driver device driver. 'c' is creatin a character driver

Run the Makefile in the modules folder with the command above.

To insert the module in the kernel, in the modules folder type: sudo insmod simple_char_driver.ko

To remove the module from the kernel,in the modules folder type: sudo rmmod simple_char_driver

Compile and run the test program with the instructions specified above.