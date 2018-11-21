#include<linux/init.h>
#include<linux/module.h>
#include <linux/uaccess.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kyle Schultz");


#define BUFFER_SIZE 1024
#define MAJOR_NUM 261

/* Define device_buffer and other global data structures you will need here */
static char* simple_char_dev_name = "simple_character_device";
static int device_open = 0;
static int device_closed = 0;
//static char device_buffer[BUFFER_SIZE]; 
char * device_buffer;


ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */

	if(length > BUFFER_SIZE - *offset)
	{
		printk(KERN_ALERT "Out of buffer bounds!\n");
		length = BUFFER_SIZE - *offset;
	}
	printk("%d",*offset);
	copy_to_user(buffer, &device_buffer[*offset], length);
	printk(KERN_ALERT "Read %d bytes", length);
	*offset += length;
	return length;
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	if(*offset + length >= BUFFER_SIZE)
	{
		printk("Trying to write too much!");
		return -1;
	}
	printk(KERN_ALERT "Wrote %d bytes", (int)length);
	copy_from_user(device_buffer + *offset, buffer, length);
	printk("%d",length);
	*offset += length; 
	printk("%d",*offset);
	return length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	device_open++;
	printk(KERN_ALERT "Device opened %d times\n", device_open);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	device_closed++;
	printk(KERN_ALERT "Device closed %d times\n", device_closed);
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	static loff_t curr_pos;
	if(whence == 0)
	{
		curr_pos = offset;
		if(offset > 1023 || offset < 0)
		{
			printk(KERN_ALERT "Seek set position out of bounds\n");
			return -1;
		}
	}

	else if(whence == 1)
	{
		curr_pos = pfile->f_pos + (int)offset;
		if((pfile->f_pos + (int)offset) > 1023 || (pfile->f_pos + (int)offset) < 0)
		{
			printk(KERN_ALERT "Seek current position out of bounds\n");
			return -1;
		}
	}

	else if(whence == 2)
	{
		if((int)offset < 0 && (int)offset >= -1024)
		{
			curr_pos = BUFFER_SIZE + (int)offset;
		}
		else if((int)offset > 0 && (int)offset <= 1024)
		{
			curr_pos = BUFFER_SIZE - (int)offset;
		}
		if((curr_pos) > 1023 || (curr_pos) < 0 || (int)offset < -1024 || (int)offset > 1024)
		{
			printk(KERN_ALERT "Seek end position out of bounds\n");
			return -1;
		}
	}
	pfile->f_pos = curr_pos;
	printk(KERN_ALERT "Lenght: %d\n", pfile->f_pos);
	return curr_pos;
}

struct file_operations simple_char_driver_file_operations = {

	.owner   = THIS_MODULE,

	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.open    = simple_char_driver_open,
	.write   = simple_char_driver_write,
	.llseek  = simple_char_driver_seek,
	.release = simple_char_driver_close,
	.read    = simple_char_driver_read
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	printk(KERN_ALERT "inside %s function\n", __FUNCTION__);
	device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
	memset(device_buffer, '\0', sizeof(device_buffer));
	register_chrdev(MAJOR_NUM, "simple_character_device", &simple_char_driver_file_operations);
	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	printk(KERN_ALERT "inside %s function\n", __FUNCTION__);
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(MAJOR_NUM, "simple_character_device");
	memset(device_buffer, '\0', sizeof(device_buffer));
	kfree(device_buffer);
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);