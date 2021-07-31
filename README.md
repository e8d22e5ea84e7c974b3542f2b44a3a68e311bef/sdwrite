## sdwrite

sdwrite is an application used for burning Linux ISOs to flash drives or to other types of storage devices. It can only be used on UNIX-like operating systems where everything is a file, and it can only burn Linux live CDs which already have the necessary structure to boot--definitely not any Windows ISO!

It is an extremely minimalist version of dd written in C--using nothing but the standard library, for maximum portability--with the express purpose of correcting the misfortunes of dd when it comes to burning live ISOs: sdwrite will provide an accurate status bar and will sync (fsync) on each buffered write.

In order to use sdwrite, you must know the small number of command-line arguments it takes (which can also be found by issuing -h):

    $ sdwrite [source file] [destination file/device] [optional buffer size] 

That's all! 

In a very unscientific test, sdwrite appears to be only slightly faster than dd when using the same block size. The status bar may also make the process faster psychologically. 

Instructions as seen from the help menu:

```
sdwrite - Copy files, with a speciality in copying files to storage devices.
Note: if you are trying to copy a Windows ISO, this is not the program: use something like WoeUSB.
Note: if you are trying to write at a specific offset of a device, use the program dd.

Usage: sdwrite [source, such as an ISO] [destination] [buffer-size], where buffer-size is optional at a default of 1M or 1024KB.
sdwrite will show the progress of the file copying and it will sync and wait in order to display it accurately.
Source file should be a regular file, and the destination file should be a device. Undefined behavior for the file size of source will happen if source is a special file.```
