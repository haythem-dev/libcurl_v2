# APM-Supplement – Basar Remote Task Execution

## Introduction

This a Basar APM-Supplement for the Remote Task Execution features in Basar. Herein you will find everything you need to know about using this feature.

### Goals of the feature were:

- Executing a task (program) on a remote machine from inside of a basar application
- Capturing the task’s output on stdout
- Capturing the task’s exit status (return code and signal, if terminated by it)
- Offering a callback to display the task’s output on stdout while the task is working

The executing should not use a custom service on the remote machine – therefore standard SSH was used.

### Non-Goals:

- Concurrency – Remote tasks will be started synchronously and the calling application will block while the remote task is running
- Security – even though SSH is used, not all SSH-features were implemented. Login is offered only by username and password and a “known hosts check” is not performed

### Further Development:

- Adding an option of starting tasks concurrently
- Offering a “known hosts check”
- Offering an option of using certificates to sign on to the remote machine (unfortunately, the used library’s documentation is insufficient for using in-memory-certificates)

### Prerequisites:

- A remote task to execute
- A script, which configures the environment for and starts the remote task
- The absolute path (starting at /) to the script

### Environment:

As this feature does not offer a full-fledged pty, there is no user environment (i.e. .profile is not sourced as it is the case upon logging in). If your task  needs a special environment, your script starting your task will have to configure the environment. (Much like the start-scripts already do)

## Usage

The remote task execution feature offers the meta include file “libbasarcmnutil\_remote.h” which in turn includes all necessary files for the remote task functionality.

*Please note*: No all include files from basar’s cmnutil are offered and libbasarcmnutil\_remote.h is also not part of the main include libbasarcmnutil.h.

### Configure your remote task:

In order to remotely execute a given task, one needs to create an object representing the task to execute. This is done by instancing a `basar::cmnutil::RemoteTaskRef` object. This is a ref-type class offering the setter part of the interface of the underlying IRemoteTask type.

Then one has to configure the RemoteTaskRef instance, setting task command (basar::VarString containing the absolute path to the shell command to execture), RemoteEndpoint (see below), credentials (also see below) and optionally a callback. RemoteTaskRef offers setters for each of its configuration parts.

### RemoteEndpont:

This is a value class containing the remote machine’s address (either ip4 or dns-name) and the service port number (this is defaulted to 22).

Use the member function `setRemoteEndpointAddress` to set the remote address

Use the member function `setRemoteEndpointPort` to set the remote port 

Getters are also offered, respectively.

### Credentials:

Storage for credentials is offered as a shared pointer to base, because each credential class knows how to perform authentication with supplied values.

At the time of writing, the class UserPassCredential is the only one offered.

Create an instance by with `basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential());`

Use `setIdentity` to set user name of remote user (normal unix account)

Use `setPassword` to set password of remote user

Getters are not part of the interface.

### Callback:

Setting a callback in the remote task object is optional. If one wishes to do so, inherit from `basar::cmnutil::IRemoteTaskCallBack` and implement the `void notify(const basar::VarString& message)` member function. Create an instance of your callback class and pass it in the `basar::cmnutil::IRemoteTaskCallBackPtr` shared pointer, then use RemoteTaskRef’s `setCredentials`.

### Starting your remote task:

A configured remote task is started by calling the singleton `basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(remoteTask)` and passing it a configured remote task object. This is a synchronous call.

The singleton’s method submitRemoteTask will, when the task is run, return an instance of `basar::cmnutil::RemoteTaskResultRef` representing the task’s results, if any.

`RemoteTaskResultRef` can be queried for the tasks exit status, exit signal and the task’s output to stdout. One can also query if an error occurred (i.e. the ask returned a non-zero value or was terminated by a signal) or if the task was terminated by a singal.

Please note: Even though `hasError()` returns true in case of a termination by signal, your tasks results may be still valid – depending on your task.

Further things your task does for your business case (e.g. updating database entries) cannot be captured by this feature.

### Exceptions:

The remote task feature defines its own kind of basar exception, the `RemoteTaskException`. This in turn is used as a base for further exceptions. Normally it should suffice to catch `RemoteTaskException` – as most derived exceptions deal with library internals. It could be useful to handling the `SshLoginUsernamePasswordFailedException` individually, depending on how username and password is passed to the library.
<!-- markdownlint-disable MD033 -->
<div style="page-break-before: always;">

### Usage Example:

```cpp
#include "libbasarcmnutil.h"
#include "libbasarcmnutil_remote.h"

//define own callback – if needed
class MyRemoteTaskCallBack : public basar::cmnutil::IRemoteTaskCallBack
{
    public:
    virtual void notify(const basar::VarString& message)
    {
        std::cout << message;
    }
};

//create credential class
basar::cmnutil::CredentialBasePtr cred(new
basar::cmnutil::UserPassCredential());

//set username and password
cred->setIdentity("devuser");

cred->setPassword("phosix+");

//create endpoint instance
basar::cmnutil::RemoteEndpoint end;

//set remote address
end.setRemoteEndpointAddress("129.100.150.141");

//end.setRemoteEndpointPort(22); //default

//create callback instance
basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

//create remote task instance
basar::cmnutil::RemoteTaskRef rtask;

//set callback
rtask.registerListener(callback);

//set credentials
rtask.setCredentials(cred);

//set endpoint
rtask.setEndpoint(end);

//set task command
rtask.setTaskCommand("/path/to/test.sh");

//start task
basar::cmnutil::RemoteTaskResultRef res =
basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);

//if result has no error, display task’s results
if (!res.hasError())
{
    std::cout << res.getResult() << std::endl;
}
```
</div>