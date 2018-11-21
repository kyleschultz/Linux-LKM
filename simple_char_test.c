#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main()
{
	int offset = 0;
	int whence = 0;
	char choice;

	int file = open("/dev/simple_character_device", O_RDWR); //open for reading and writing

	while(1)
	{
		printf("Options:\nPress r to read from device\nPress w to write to the device\nPress s to seek into the device\nPress e to exit from the device\nPress anything else to keep reading or writing from the device\nEnter command:\n");
		scanf("%c", &choice);

		switch(choice)
		{
			case 'r':
			{
				int user_buffer_size;
				printf("Please enter the number of bytes you want to read\n");
				scanf("%d", &user_buffer_size);
				char * buffer = malloc(user_buffer_size);
				read(file, buffer, user_buffer_size);
				printf("Data read from device:%s\n", buffer);
				memset(buffer, '\0', sizeof(buffer));
				free(buffer);
				while (getchar() != '\n');
				break;
			}
			case 'w':
			{
				int length = 0;
				char * b = malloc(BUFFER_SIZE);
				for(int i = 0; i < 1024; i++)
				{
					b[i] = '\0';
				}
				printf("Enter data you want to write to the device:\n");
				scanf(" %[^\n]s", b);
				while(b[length] != '\0')
				{
					length++;
				}
				write(file, b, length);
				memset(b, '\0', sizeof(b));
				free(b);
				while (getchar() != '\n');
				break;
			}
			case 's':
			{
				printf("Enter an offset value:\n");
				scanf("%d", &offset);
				printf("Enter a value for whence (third parameter):\n");
				scanf("%d", &whence);
				llseek(file, offset, whence);
				while (getchar() != '\n');
                break;
            }
            case 'e':
                return 0;	// exits
            default:
                break;
		}
	}
	close(file);
    return 0;
}