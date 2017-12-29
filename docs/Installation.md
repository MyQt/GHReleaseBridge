---
id: Installation
title: Installing GHReleaseBridge to your Project.
sidebar_label: Installation
---

**GHReleaseBridge** is a small header writen in C++ using Qt5 Framework. This   
little header helps you to auto update your package that is deployed in Github Releases  
without any complications and thus makes your code clean.

**Before jumping the gun , make sure you have the [dependencies](#dependencies)**

## Dependencies

* [Qt5 Framework](https://qt.io)
	
	Qt is an amazing **GUI Framework** which is made for the future , it got   
	**stability , performance and the looks!**. I really recommend you all to   
	use Qt for your **GUI** Projects. And the best part is , its **Open Source**.   
	Make sure to install it too.

* [libarchive](https://github.com/libarchive/libarchive)   

	Before using **GHReleaseBridge** make sure you have installed **libarchive** , its   
	kinda of a famous **library** so most of the linux distros have it listed   
	in their **repository**. This is one of dependencies of **QArchive**.

## Installing the latest release from github

Follow the instructions specific for your platform.

### Linux

**Just execute this command on your project folder and everything will be done for you!**   
You must have **curl** to do this , **don't worry** because most of the linux distro's must   
have **installed it already** for you , if not then the script will **warn** you to install it!

```
 $ curl -L "https://git.io/vbNIi" | bash
```

**or** you can also use python.   
This requires **requests** to be installed , so make sure you have installed **requests** for python.

```
 $ python -c "from requests import get;exec(get('https://git.io/vbNI1').content)"
```

**Thats it!**

### Windows

**Just execute this command on your project folder and everything will be done for you!**   
You need to have **python** with **requests** installed on your pc to do this but you can also clone   
the entire repo.

```
 > python -c "from requests import get;exec(get('https://git.io/vbNI1').content)"
```

### MacOSX

**Just execute this command on your project folder and everything will be done for you!**   
You need to have **python** with **requests** installed on your pc to do this but you can also clone  
the entire repo.

```
 $ python -c "from requests import get;exec(get('https://git.io/vbNI1').content)"
```
