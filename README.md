# System-Programming
This repository contains Linux OS programming fundamentals. Also it contains how to implement System Call  and FUSE on Linux.
  - In System Call fork() and exit() system calls are modified.
  - In FUSE a read-only directory created which tidies HTML files of a website using Libtidy.

## System Call Development
   ### Requirements For System Call Project 
       - Install Virtual Box (highly recommended in system call development process because of safety)
       - Download Lubuntu 14.04.5 LTS from http://cdimage.ubuntu.com/lubuntu/releases/14.04.5/release/
       - create your virtual machine and DON'T UPDATE it.
       - Install required packages with using this command : 
         - sudo apt-get update
         - sudo apt-get install manpages-dev g++ patch strace ltrace linux-headers-$(uname -r) linux-source kernel-package fakeroot libncurses5-dev libfuse-dev
         
   ### Adding a System Call
       - move linux source archive file to desktop
         - cd Desktop
         - sudo mv /usr/src/linux-source-3.13.0/linux-source- 3.13.0.tar.bz2 linux-source-3.13.0.tar.bz2
       - and extract it
         - tar -xjvf linux-source-3.13.0.tar.bz2
       - enter linux source folder  cd linux-source-3.13.0
       
   ### Write Your System Call 
       - You can find the step by step project explanation in [THIS](SystemCall/STEP_BY_STEP_REPORT.pdf) report. 
    
   ### Compile and Install New Kernel 
       - Use make defconfig command to generate kernel configuration 
       - make-kpkg cleans the previous kernel compilations
       - Compile Command (It may take 30 minutes) : 
         - fakeroot make-kpkg --initrd --append-to-version=-custom kernel_image kernel_headers
       - You can find the kernel image and kernel header outputs in the desktop.
       - Install the new kernel : 
         - sudo dpkg -i linux-image-3.13...
         - sudo dpkg -i linux-headers-3.13...
         - sudo update-grub
         - sudo reboot( You can find the new kernel from advanced options )
       - Now you can test it.
      
   ### Uninstall Compiled Kernel
       - sudo dpkg -r linux-image-3.13...custom
       - sudo dpkg -r linux-headers-3.13...custom
  
## FUSE Development
   ### ----------------- INFORMATION -----------------------
   -  This is a Website Read Only File System for users.
   -  Simply it does not allows write operations in it.
   -  Main purpose is giving tidied HTML files to users.
   -  In read operation code creates tidied output of HTML.
   ### ----------------- REQUIREMENTS ----------------------
   -  First we need to install libtidy 
   -  git clone git@github.com:htacg/tidy-html5.git
   -  cd {your-tidy-html5-directory}/build/cmake
   -  cmake ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
   -  make
   -  sudo make install
   ### -------------------- USAGE---------------------------
   -  Create a directory ( for example  Website ) 
   -  In Website directory create html directory 
   -  The other directories in website does not make sense.
   -  Move your .html files into html directory
   -  Go to out of your website.  
   -  Create a directory named test
   -  Create a configuration file config.txt to make configuration
   -  For a true configuration change the config_file_path to your code directory
   -  Then compile using : gcc -I/usr/include/tidy myFuseSystem.c -o myFuse -Wall -ansi -W -std=c99 -g -ggdb -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -lfuse -ltidy
   -  Execute : ./myFuse -d website test
   -  Open another terminal and go to test/html directory
   -  Read the Html files using cat try.html 
   -  Output is tidied.
   -  You can find all explanations and requirements in [THIS](FUSE/fuseSystem.pdf) report. 
