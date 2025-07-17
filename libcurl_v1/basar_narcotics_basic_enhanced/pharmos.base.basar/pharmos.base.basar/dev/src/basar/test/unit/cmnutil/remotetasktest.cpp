#include <UnitTest++/UnitTest++.h>


#include "libbasarcmnutil.h"
#include "libbasarcmnutil_remote.h"

#include "libbasardebug.h"

#include <iostream>

//------------------------------------------------------------------------------
extern bool g_RUNSHORT;

//------------------------------------------------------------------------------
SUITE(TestRemoteTask)
{

	basar::ConstString INVALID_ADDRESS = "XXX";

	basar::ConstString DEV_SERVER = "zdev21";

	basar::ConstString USER = "devuser";

	basar::ConstString PASS = "phosix+";

	basar::ConstString BASE_PATH = "/software/ae/marathon/bin/ut_scripts/";

	//define own callback
	class MyRemoteTaskCallBack : public basar::cmnutil::IRemoteTaskCallBack
	{
		public:
			virtual void notify(const basar::VarString& message)
			{
				std::cout << message;
			}
	};

	TEST(empty_remote_endpoint_address_fail)
	{
	
		basar::cmnutil::RemoteEndpoint end;
		end.setRemoteEndpointAddress("");

		basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 
		cred->setIdentity(USER);

		basar::cmnutil::RemoteTaskRef rtask;
		rtask.setCredentials(cred);
		rtask.setEndpoint(end);

		rtask.setTaskCommand("ls -l");

		CHECK_THROW(basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask), basar::cmnutil::RemoteEndpointAddressEmptyException);
	}

	TEST(empty_remote_command_fail)
	{

		basar::cmnutil::RemoteEndpoint end;
		end.setRemoteEndpointAddress(DEV_SERVER);
		//end.setRemoteEndpointPort(22); //default

		basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 
		cred->setIdentity(USER);
		cred->setPassword(PASS);

		basar::cmnutil::RemoteTaskRef rtask;
		rtask.setCredentials(cred);
		rtask.setEndpoint(end);

		rtask.setTaskCommand("");

		CHECK_THROW(basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask), basar::cmnutil::RemoteTaskCommandEmptyException);
	}

	TEST(socket_connection_fail)
	{
		basar::cmnutil::RemoteEndpoint end;
		end.setRemoteEndpointAddress(INVALID_ADDRESS);

		basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 
		cred->setIdentity(USER);

		basar::cmnutil::RemoteTaskRef rtask;
		rtask.setCredentials(cred);
		rtask.setEndpoint(end);

		rtask.setTaskCommand("ls -l");

		CHECK_THROW(basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask), basar::cmnutil::TcpSocketException);
	}

	TEST(remotetask_with_environment)
	{
		if (g_RUNSHORT)
		{
			return;
		}
		
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			//ref class needs factory character
			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 

			cred->setIdentity(USER);
			cred->setPassword(PASS);

			//stays value class
			basar::cmnutil::RemoteEndpoint end;

			end.setRemoteEndpointAddress(DEV_SERVER);
			//end.setRemoteEndpointPort(22); //default

			//see AccessorExecutionListener
			basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask;

			rtask.registerListener(callback);

			rtask.setCredentials(cred);

			rtask.setEndpoint(end);

			basar::VarString cmd(BASE_PATH);
			cmd.append("remote_with_env.sh");

			rtask.setTaskCommand(cmd.c_str());
		
			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);

			if (!res.hasError())
			{
				std::cout << res.getResult() << std::endl;
			}

			CHECK_EQUAL(false, res.hasSignal());
			CHECK_EQUAL(true, res.hasError());

			CHECK_EQUAL(42, res.getExitCode());

		}
	}

	TEST(remotetask_with_background_task)
	{
		if (g_RUNSHORT)
		{
			return;
		}
	
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			//ref class needs factory character
			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 

			cred->setIdentity(USER);
			cred->setPassword(PASS);

			//stays value class
			basar::cmnutil::RemoteEndpoint end;

			end.setRemoteEndpointAddress(DEV_SERVER);
			//end.setRemoteEndpointPort(22); //default

			//see AccessorExecutionListener
			basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask;

			rtask.registerListener(callback);

			rtask.setCredentials(cred);

			rtask.setEndpoint(end);

			basar::VarString cmd(BASE_PATH);
			cmd.append("remote_background_test.sh");

			rtask.setTaskCommand(cmd.c_str());
			
			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);

			if (!res.hasError())
			{
				std::cout << res.getResult() << std::endl;
			}

			CHECK_EQUAL(false, res.hasSignal());
			CHECK_EQUAL(false, res.hasError());

			CHECK_EQUAL(0, res.getExitCode());
		}
	}

	TEST(remotetask_std)
	{
		if (g_RUNSHORT)
		{
			return;
		}
		
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			//ref class needs factory character
			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 

			cred->setIdentity(USER);
			cred->setPassword(PASS);

			//stays value class
			basar::cmnutil::RemoteEndpoint end;

			end.setRemoteEndpointAddress(DEV_SERVER);
			//end.setRemoteEndpointPort(22); //default

			//see AccessorExecutionListener
			basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask;

			rtask.registerListener(callback);

			rtask.setCredentials(cred);

			rtask.setEndpoint(end);

			basar::VarString cmd(BASE_PATH);
			cmd.append("remote_test.sh");

			rtask.setTaskCommand(cmd.c_str());

			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);

			if (!res.hasError())
			{
				std::cout << res.getResult() << std::endl;
			}

			CHECK_EQUAL(false, res.hasSignal());
			CHECK_EQUAL(false, res.hasError());

			CHECK_EQUAL(0, res.getExitCode());

		}
	}

	TEST(remotetask_std_duplicate)
	{
		if (g_RUNSHORT)
		{
			return;
		}
		
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			//ref class needs factory character
			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 

			cred->setIdentity(USER);
			cred->setPassword(PASS);

			//stays value class
			basar::cmnutil::RemoteEndpoint end;

			end.setRemoteEndpointAddress(DEV_SERVER);
			//end.setRemoteEndpointPort(22); //default

			//see AccessorExecutionListener
			basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask;

			rtask.registerListener(callback);

			rtask.setCredentials(cred);

			rtask.setEndpoint(end);

			basar::VarString cmd(BASE_PATH);
			cmd.append("remote_test.sh");

			rtask.setTaskCommand(cmd.c_str());
			
			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);

			if (!res.hasError())
			{
				std::cout << res.getResult() << std::endl;
			}

			CHECK_EQUAL(false, res.hasSignal());
			CHECK_EQUAL(false, res.hasError());

			CHECK_EQUAL(0, res.getExitCode());

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask2;

			rtask2.registerListener(callback);

			rtask2.setCredentials(cred);

			rtask2.setEndpoint(end);

			cmd = BASE_PATH;
			cmd.append("remote_test.sh");

			rtask2.setTaskCommand(cmd.c_str());		

			res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask2);

			CHECK_EQUAL(false, res.hasSignal());
			CHECK_EQUAL(false, res.hasError());

			CHECK_EQUAL(0, res.getExitCode());

		}
	}

//------------------------------------------------------------------------------
	TEST(remotetask_justerrorcode)
	{
		if (g_RUNSHORT)
		{
			return;
		}
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			//ref class needs factory character
			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 

			cred->setIdentity(USER);
			cred->setPassword(PASS);

			//stays value class
			basar::cmnutil::RemoteEndpoint end;

			end.setRemoteEndpointAddress(DEV_SERVER);
			//end.setRemoteEndpointPort(22); //default

			//see AccessorExecutionListener
			basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask;

			rtask.registerListener(callback);

			rtask.setCredentials(cred);

			rtask.setEndpoint(end);

			basar::VarString cmd(BASE_PATH);
			cmd.append("remote_return_42.sh");		

			rtask.setTaskCommand(cmd.c_str());
			
			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);


			CHECK_EQUAL(false, res.hasSignal());
			CHECK_EQUAL(true, res.hasError());

			CHECK_EQUAL(42, res.getExitCode());

			CHECK_EQUAL(true, res.getResult().empty());
		}

	}

//------------------------------------------------------------------------------

	TEST(remotetask_sigterm)
	{
		if (g_RUNSHORT)
		{
			return;
		}
		
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			//ref class needs factory character
			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 

			cred->setIdentity(USER);
			cred->setPassword(PASS);


			//stays value class
			basar::cmnutil::RemoteEndpoint end;

			end.setRemoteEndpointAddress(DEV_SERVER);
			//end.setRemoteEndpointPort(22); //default

			//see AccessorExecutionListener
			basar::cmnutil::IRemoteTaskCallBackPtr callback(new MyRemoteTaskCallBack);

			//analogous to RemoteTaskResult
			basar::cmnutil::RemoteTaskRef rtask;

			rtask.registerListener(callback);

			rtask.setCredentials(cred);

			rtask.setEndpoint(end);

			basar::VarString cmd(BASE_PATH);
			cmd.append("remote_return_sigterm.sh");	

			rtask.setTaskCommand(cmd.c_str());
			
			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask);


			CHECK_EQUAL(true, res.hasSignal());
			CHECK_EQUAL(true, res.hasError());

			CHECK_EQUAL(0, res.getExitCode());

			CHECK_EQUAL("TERM", res.getSignal().c_str());

			CHECK_EQUAL(true, res.getResult().empty());
		}
	}

	TEST(login_fail_wrong_password)
	{
		if (g_RUNSHORT)
		{
			return;
		}
		
		if (basar::debug::isBuildServer())
		{
			return;
		}
		else
		{
			basar::cmnutil::RemoteEndpoint end;
			end.setRemoteEndpointAddress(DEV_SERVER);

			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 
			cred->setIdentity(USER);

			basar::cmnutil::RemoteTaskRef rtask;
			rtask.setCredentials(cred);
			rtask.setEndpoint(end);

			rtask.setTaskCommand("ls -l");

			CHECK_THROW(basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(rtask), basar::cmnutil::SshLoginUsernamePasswordFailedException);
		}
	}

//------------------------------------------------------------------------------
}	// end TESTSUITE