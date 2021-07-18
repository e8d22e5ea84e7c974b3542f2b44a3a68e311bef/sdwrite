## sdwrite

sdwrite is an application used for burning Linux ISOs to flash drives or to other types of storage devices. It can only be used on UNIX-like operating systems where everything is a file, and it can only burn Linux live CDs which already have the necessary structure to boot--definitely not any Windows ISO!

It is an extremely minimalist version of dd written in C--using nothing but the standard library, for maximum portability--with the express purpose of correcting the misfortunes of dd when it comes to burning live ISOs: sdwrite will provide an accurate status bar and will sync (fsync) on each buffered write.

In order to use sdwrite, you must know the small number of command-line arguments it takes (which can also be found by issuing -h):

    $ sdwrite [source file] [destination file/device] [optional buffer size] 

That's all! 

In a very unscientific test, sdwrite appears to be only slightly faster than dd when using the same block size. The status bar may also make the process faster psychologically. 
