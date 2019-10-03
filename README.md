# Ransomware-Detection
Visual C++ project to detect ransomware activity on Windows OS. It consists of a registry monitor, filesystem watcher and a process detector.

# Description
Ransomware is a malware/malicious software program designed to block or disable access to the data your computer until a sum of money is paid.
The project aims at detecting and notifying the user of potential ransomware behavior on the windows system.
I decided on three paramters on which the behavior can be judged after studying the behavior of various ransomware.

* Detecting a particular Process execution.(Example VSSadmin process for deleting backup files)
* Monitoring a Registry key for changes.
* Creating a honeypot folder, and monitoring it for any file changes.(renames and deletes)
