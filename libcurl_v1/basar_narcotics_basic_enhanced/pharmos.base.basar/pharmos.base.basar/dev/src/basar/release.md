# HowTo: ssuk Release

## Local:
- on develop branch:
    - adapt version:
	  - changelog.txt
	  - libbasar_pkgversion.h
	  - data/version.inc
	  - doxygen/doxyfile doxyfile_local
    - run unittests again (long, debug & release, win & aix)

## TFS:
- force nightly build (or wait ...)

- if everything is okay:
  - PR to master branch, Review & Merge (do not delete develop branch!)

## Local:
  - set tag on master branch (and push tag)
  
## TFS:
- manual build (with tag)
- download artifact locally and check them
  - i.e check version numbers in header files

- if everything okay:
  - run TFS release step
  
## Nexus
- check chocobo and moogle if artifacts have been uploaded
  - optional: download artifact locally and check them (check ivy.xml, etc)
  

# Sharepoint:

optional: add an announcement in sharepoint https://coin-collaboration.phoenix.loc/communities/pgit_developerzone/basar/

or Mailing-List / Group-Leaders

# Citrix

## Windows:

in `\\denu1s300\ph-client-FMO$\marathon`

- 1. on TEST
- 2. on ACC

directory ssuk-$version_vc10, i.e ssuk-01.12.01.01_vc10

only sharedlib, no headers or development files

now wait for app-developers for approval (?!)

- 3. on PROD:

remove basar.config.xml

Omnitracker -> RFC (request for change) for approval in PROD -> via Group-Leader

## AIX:

- 1. on TEST

in `zdev21:$WSS/lib/marathon`:

directory ssuk-$version

only sharedlib, no headers or development files

in `zdev21:$DEVLIB_PATH/marathon`:

directoy ssuk-$version

everything incl. development files

> there should be a script in `proc/installdev.sh`  
# TODO: script needs to be updated to use nexus and $DEVLIB_PATH!

now wait for app-developers for approval (?!)

- 2. on ACC

theoretically: (not used yet)

in `zqas21:$WSS/lib/marathon`:

directory ssuk-$version

only sharedlib, no headers or development files


- 3. on PROD

we do not do anything here, same like win32 PROD.

Omnitracker -> RFC (request for change) for approval in PROD -> via Group-Leader


