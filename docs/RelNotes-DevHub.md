# Developer Hub Release Notes

## Afero Developer Hub R1.1.673

Release date: January 28, 2020

- Debian packaging updates:

- - Support for Raspbian and Raspbian Lite "Buster" releases.

- Beetle updates:

- - Many fixes to Bluetooth® low energy connection management.
  - No longer requires running on older Raspbian; we now support “Stretch” OS (version 2017-08-16 or newer) and all "Buster" OS releases.

- Hubby updates:

- - Bug fixes to bring Developer Hub up to date with new Afero Cloud onboarding.
  - Support for Afero Enterprise Mode, enabled by default.

## Afero Developer Hub R1.1.443

Release date: December 18, 2017

- Debian packaging updates:

- - Removed requirement for `supervisord` daemon management utility.
  - Converted startup/shutdown scripts to `systemd` services.
  - Fixed an issue where occasionally daemons wouldn’t restart after running “dpkg-reconfigure”.

- Beetle updates:

- - Many fixes to Bluetooth® low energy connection management.
  - Properly restarts scanning when Linux Kernel 4.4 stops delivering advertisement packets.
  - No longer requires running on older Raspbian; we now support “Stretch” OS (version 2017-08-16 or newer).

- Hubby updates:

- - Bug fixes to bring Developer Hub functionality in sync with soft hub (mobile app hub) functionality.
  - Support for Afero peripheral mode devices.

- The Afero Hub Software (“Beetle” and “Hubby”) can now be operated on two separate hosts for testing purposes. To learn more, read [Running Afero Hub Software on Separate Hosts (.pdf)](../files/TechNote-AHS-on-Separate-Hosts.pdf).