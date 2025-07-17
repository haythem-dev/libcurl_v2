# APM-Supplement – Basar 1.11. mainbackground changes

## Introduction

This is a Basar APM-Supplement for the changes of Basars mainbackground class starting of version 1.11. Herein you will find everything you need to know about using these features.

### Goals of the feature were:

- Replace the existing image with a new one indicating which type (Inbound/Outbound/Warehouse) an application has. Additionally a neutral background for use at non-Phoenix-companies was added.

### Additional changes:

- none

## Usage

The needed header files are part of the header `“libbasarguiqt.h”`. Alternatively the header `“libbasarguiqt\_mainbackground.h”` could be used.

As before, create an instance of the class `basar::gui::qt::Mainbackground`, then call the init member function. This function got an additional parameter `eProductType`, so pass the proper member of the `ProductTypeEnum` (Inbound, Outbound, Warehouse). In case application does not belong to any of those, an unknown option also exists.

Furthermore, the existing ImageEnum has been extended to include a neutral option for use in non-Phoenix-companies has been added. In case the neutral background is required, pass the `NEUTRAL`-value as optional fouth parameter to the call of init.
