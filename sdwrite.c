/* GNU License */
/* GPL */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define MEGABYTE 1024*1024
#define DEFAULT_BUFFER_SIZE 1024*1024*2 // 2M

/* Dangerous devices to write to. */
const char *dangers[] = {
	"/dev/sda",
	"/dev/vda",
	"/dev/hda",
	"/dev/mmcblk",
	"/dev/fd0"
};

/* Generate a choice. */
int choice(const char *prompt)
{
	while (1)
	{
		printf("%s (y/n): ", prompt);
		char c = getchar();
		getchar();

		if (c == 'y' || c == 'Y')
		{
			return 1;
		}
		else if (c == 'n' || c == 'N')
		{
			return 0;
		}
		else
		{
			puts("What? Y/y or N/n\n");
			continue;
		}
	}
}


/* Help menu */
void helpm()
{
	const char *help =
	"sdwrite - Copy files, with a speciality in copying files to storage devices.\n"
	"Note: if you are trying to copy a Windows ISO, this is not the program: use something like WoeUSB.\n"
	"Note: if you are trying to write at a specific offset of a device, use the program dd.\n"
	"Usage: sdwrite [source, such as an ISO] [destination] [buffer-size], where buffer-size is optional at a default of 1M or 1024KB.\n"
	"sdwrite will show the progress of the file copying and it will sync and wait in order to display it accurately.\n"
	"Source file should be a regular file, and the destination file should be a device. Undefined behavior for the file size of source will happen if source is a special file.\n";

	printf("%s\n", help);
}


/* Main program. */
int main(int argc, char **argv, char **envp)
{
	const char *source = argv[1];
	const char *destination = argv[2];
	const int buffersize = (argv[3] == NULL) ? DEFAULT_BUFFER_SIZE : atoi(argv[3]) * 1024;
	/*
				^^^^^^^^^^^^^^^
			       	If argv[3] or the buffer size does not exist, set it to the default!	
	*/

	if (!source)
	{
		fprintf(stderr, "Error: you must provide a source file. Do --help or -h to see the help menu.\n");
		return 1;
	}

	/* If they supplied the help argument or if the destination does not exist, show them the help menu. */
	if (!strcmp(source, "-h") || !strcmp(source, "--help") || !destination)
	{
		helpm();
		return 1;
	}

	FILE *fsource = fopen(source, "rb");
	FILE *fdestination = fopen(destination, "wb");

	if (!fsource)
	{
		fprintf(stderr, "Error opening source file: ");
		perror(source);
		return 2;
	}

	if (!fdestination)
	{
		fprintf(stderr, "Error opening destination file/device: ");
		perror(destination);
		return 3;
	}

	/* If the buffersize is zero or above 256 megabytes. */
	if (!buffersize || buffersize > MEGABYTE*256)
	{
		fprintf(stderr, "Error: A buffer size of %d does not make sense!\n", buffersize);
		return 4;
	}

	printf("Reading from: %s\n", source);
	printf("Writing to: %s\n", destination);
	printf("By a buffer size of: %dKB (1M or 1024 KB is the default).\n", buffersize/1024);

	if (!choice("Are you sure you want to continue?"))
		return 0;

	/* Cycle through each dangerous path and check if we are writing to it. */
	for (int i = 0; i < sizeof(dangers)/sizeof(char*); i++)
	{
		if (!strcmp(dangers[i], destination))
		{
			if (!choice("WARNING!!! You are writing to your main storage device! Continue? THIS WILL OVERWRITE DATA."))
				return 0;
			
			break; 
		}
	}

	fseek(fsource, 0, SEEK_END);

	const int fsize = ftell(fsource);
	const int fdestinationfd = fileno(fdestination);

	fseek(fsource, 0, SEEK_SET);

	printf("Writing %dMB...\n", fsize / (1024*1024));

	size_t fwrote = 0;
	char buffer[buffersize];

	/* While EOF not reached, read into buffer with buffer size, and then write the bytes read from fread into the destination. */
	while (!feof(fsource))
	{
		size_t bytes = fread(buffer, sizeof(char), buffersize, fsource);

		if (ferror(fsource))
		{
			perror("FATAL: Error while reading from ISO/File");
			return 5;
		}

		if (fwrite(buffer, sizeof(char), bytes, fdestination) != bytes || ferror(fdestination) != 0)
		{
			perror("FATAL: Error while writing to device");
			return 6;
		}
		
		fwrote = fwrote + bytes;
		fsync(fdestinationfd);

		double percent_complete = (double) fwrote/fsize*100;

		/* The carriage return with the flushing of stdout will overwrite the previous line. */
		printf("%.2lf KB / %.2lf KB complete. (%.2lf%).\r", (double) fwrote/1024, (double) fsize/1024, percent_complete);
		fflush(stdout);
	}

	printf("\nDone writing to %s.\n", destination); 
	printf("Data synced. If the USB or device isn't flashing rapidly anymore, it's probably safe to remove or use.\n");

	/* Clean up. */
	fclose(fsource);
	fclose(fdestination);


	return 0;
}
