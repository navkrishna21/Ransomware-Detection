# Ransomware-Detection
Visual C++ project to detect ransomware activity on Windows OS. It consists of a registry monitor, filesystem watcher and a process detector.

# Requirements
* Visual Studio 2017
* Visual C++ runtime library 

# Description
Ransomware is a malware/malicious software program designed to block or disable access to the data your computer until a sum of money is paid.
The project aims at detecting and notifying the user of potential ransomware behavior on the windows system.
I decided on three parameters on which the behavior can be judged after studying the behavior of various ransomware.

* Detecting a Process commonly executed by Ransomware.(Example VSSadmin process for deleting backup files)
* Monitoring a commonly affected Windows Registry key for changes.
* Creating a honeypot folder, and monitoring it for any file changes.(renames and deletes)

The project is multithreaded so that all the three features can be run asynchronously.
If the program signals for any 2 out of the 3 parameters, it notifies it as a high risk of Ransomware Detection.  

