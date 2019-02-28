# Lab1: Establish Lab Environment

> This is a lab requirement offline backup for offline doing labs,
> with little syntax and typo fixed.
> Original Requirement: https://hackmd.io/vwdiqUbZQ96vSfw_mLyCmw?view

## Objective:

In this lab you can learn:

*	Understand version control system and Makefile project
*	Learn how to use QEMU and GDB to debug Linux 0.11 kernel
*	Learn how to commit to Git system

## 1.	Lab 1-1 Linux 0.11 Development Environment Setup

Open VMware and login fedora using below account.

* **Username**: `osdi`
* **Password**: `osdi2014`

Note: If you want to do the labs at home you can download the VM image here: http://grass8.cs.nctu.edu.tw/OSDI_VM.zip

### 1.1.	Prepare Git environment

### 1.2.	Account registration

Go to: http://grass8.cs.nctu.edu.tw:8888/users/sign_up

Use your student id as register username otherwise, you cannot demo in this lab.
 
![](https://i.imgur.com/EDeM13K.png)

After registration, you will receive a confirmation email in your mail box. If not, please tell TA.

### 1.3.	Check out your lab files from GitLab

```bash
git clone http://grass8.cs.nctu.edu.tw:8888/2018/osdi.git
cd osdi
git checkout -b lab1 origin/lab1
```

Then you will see the linux0.11 source code “linux-0.11” and Linux root file system disk “osdi.img” in your OSDI folder.
![](https://i.imgur.com/SSIU5C3.png)
 
### 1.4.	Find the Makefile bugs

Our Linux 0.11 Makefile have some syntax errors. Please find that and fix them.

### 1.5.	Build the Linux 0.11

```bash
cd linux-0.11
make
```

After make, you will see a bootable file **Image** in the linux-0.11 folder, it contains system bootloader and linux0.11 kernel.

![](https://i.imgur.com/oBAqlnC.png)

Note: if you have modified any file, please ‘make clean’ before next make.

### 1.6.	Compile and install newest QEMU

```bash
git clone git://git.qemu-project.org/qemu.git
cd qemu
git checkout -b v2.6 origin/stable-2.6
git submodule update --init dtc
git submodule update --init pixman
./configure
make && make install
```

Note: Your QEMU emulator version needs larger than 2.5.x
 
![](https://i.imgur.com/PZgJoLO.png)

Reference: https://en.wikibooks.org/wiki/QEMU

### 1.7.	Run the Linux 0.11

After Linux 0.11 make, the system will produce a bootable floppy disk image called “Image” in your Linux 0.11 root folder, then you can just use QEMU emulator to load this image and run Linux 0.11.

```bash
qemu-system-i386 -m 16M -boot a -fda Image -hda ../osdi.img
```

![](https://i.imgur.com/Dy7iMns.png)

Note: osdi.img is an MINIX root file system it contains some executable files, such as shell system, gcc, etc. You can mount the root file system to modify or add the files.

```bash
mkdir rootfs
mount -o loop,offset=1024 osdi.img rootfs/
```

![](https://i.imgur.com/WsoFxOx.png)

## 2.	Lab 1-2 Debug kernel

### 2.1.	Find the kernel bugs

In this lab, you need to use the GDB to find out the bugs and fix them.
![](https://i.imgur.com/HKGuaFm.png)

Hint: Use backtrace command to find the kernel bugs, there are 2 bugs in the lab1 Linux kernel.

### 2.2.	Debug the Linux 0.11 on QEMU

In QEMU environment, you can debug the OS kernel via gdbserver and gdb. Use this
 
to find the kernel and open gdb with Linux 0.11 kernel symbol file.

![](https://i.imgur.com/OQgiRbr.png)

```bash
qemu-system-i386 -m 16M -boot a -fda Image -hda ../osdi.img -s -S -serial stdio
```

Open another console

```bash
cd linux-0.11
gdb 
```

> Please check 'linux-0.11/.gdbinit' to find out how gdb connects to its remote target.

Notes:

*	You can add some breakpoint at OS main function or interrupt service routine than step by step see how the code execute.
*	Sometimes (e.g. system call, context switch, …) you may need to debug in assembly, you can see below references that could help debug easier. 
http://mohit.io/blog/assembly-and-the-art-of-debugging/ https://sourceware.org/gdb/onlinedocs/gdb/Machine-Code.html
*	The kernel/panic.c is correct, you need not change it.

 ![](https://i.imgur.com/9FX4kZ7.png)

*	Useful GDB commands
	* `b`- set breakpoint
  * `c`- continue program ‘list’- list code
	* `backtrace`- show call stack
	* `info r`-show current registers value
	* `Ctrl-C` – stop program
	* GDB command reference: http://www.cmlab.csie.ntu.edu.tw/~daniel/linux/gdb.html

### 2.3.	Print your student id

Modify the Linux 0.11 source code and print your student id before shell startup.
 
![](https://i.imgur.com/73PXQo0.png)

## 3.	Lab 1-3 Submit your lab1 for DEMO

### 3.1.	Commit your source code and image

Create your own repository on Gitlab website.

Step1: Login into http://grass8.cs.nctu.edu.tw:8888 and new project.

![](https://i.imgur.com/WLLM9GK.png)

Step2: Use your student ID as the project name and select visibility level as private.
 
![](https://i.imgur.com/EgsSvXd.png)

Step3: Add remote repository to your local machine.

```bash
cd osdi
git config --global user.name "{Your username}"
git config --global user.email "{Your email}"
git remote add osdi http://grass8.cs.nctu.edu.tw/{Your username}/{Project name}.git
git push osdi lab1:lab1
```

You can see below project status on gitlab web site when complete step3.
 
![](https://i.imgur.com/siks9yO.png)


Use “git remote –v” command will see two remote repositories setting.

![](https://i.imgur.com/QTOjBds.png)

Step4: Commit and push the files to your remote repository

```bash
git add -A
git commit -m "Lab1 demo"
git push osdi lab1:lab1
```

Note: GIT documentation and command reference: https://git-scm.com/doc

Git-基礎-與遠端協同工作: https://git-scm.com/book/zh-tw/v1/Git-%E5%9F%BA%E7%A4%8E-%E8%88%87%E9%81%A0%E7%AB%AF%E5%8D%94%E5%90%8C%E5%B7%A5%E4%BD%9C
 
## 4.	Questions

Q1: QEMU
> ```
> qemu-system-i386 -m 16M -boot a -fda Image -hda ../osdi.img -s -S -serial stdio
> ``` 
> According to the above command:
> Q1.1: What's the difference between -S and -s in the command?
> 
> Q1.2: What are -boot, -fda and -hda used for? If I want to boot with \.\./osdi.img(supposed it's a bootable image) what should I do?

Q2: Git
> Q2.1: Please explain all the flags and options used in below command:
> ```
> git checkout -b lab1 origin/lab1
> ```
> Q2.2 What are the differences among git add, git commit, and git push? What's the timing you will use them?
> 

Q3: Makefile
> Q3.1: What happened when you run the below command? Please explain it according to the Makefile.
> ```
> make clean && make
> ```
> Q3.2: I did edit the include/linux/sched.h file and run `make` command successfully but the Image file remains the same. However, if I edit the init/main.c file and run `make` command. My Image will be recompile. What's the difference between these two operations?

Q4: Anatomy of the linux-0.11
> Q4.1 Please explain how linux-0.11 is made in detail(including what 'as', 'ld', 'objcopy', 'build.sh' do?).
> Q4.2 After making, what does the kernel Image 'Image' look like?

Q5: It seems like `main` is not the entry point of the program. Where is the actual entry point of linux-0.11?
