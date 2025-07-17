# APM-Supplement – Basar Mail Attachment

## Introduction

This a Basar APM-Supplement for the Mail Attachment features in Basar. Herein you will find everything you need to know about using this feature.

### Goals of the feature were:

- Sending an email with a file attached

### Non-Goals:

- Taking measures that the attachments go through any spam filters
- Providing a mechanism for smtp authentication
- Rework of the existing email class

### Prerequisites:

- A network allowing an unobstructed (non-firewalled) connection to a SMTP server on port 25
- A SMTP server which does not require authentication to send email (open relay)

## Usage

The needed header files are part of the header “libbasarcmnutil.h”.

The Attachment functionality offers two kinds of attachment creation:

- Creating an attachment from an existing file
- Creating an attachment from a buffer in memory

These two kinds of creation are handled by the static methods `createFromFile` and `createFromMemory` respectively. Both return a shared pointer to an attachment object. This shared pointer can then be added to an existing basar mail object by calling the mail objects `addAttachment`-member function and passing said shared pointer.

Note: `createFromMemory` requires a name by which the attachment is identified and an istream from which the data is read.
