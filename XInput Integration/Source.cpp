

#include<iostream>
#include<string>

//include xinput headders
#include<Windows.h>
#include<winerror.h>
#include<Xinput.h>
#include<vector>
#include<memory>

//link the required libraries
#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")

struct Controller
{
	XINPUT_STATE m_state;
	bool m_connected = false;
	
};

void InitializeControllers(std::vector<std::shared_ptr<Controller>>& controllers)
{
	controllers.resize(XUSER_MAX_COUNT);
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		controllers[i] = std::make_shared<Controller>();
	}
}

void CheckControllerConnections(std::vector<std::shared_ptr<Controller>>& controllers)
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		memset(&controllers[i]->m_state, 0, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &controllers[i]->m_state) == ERROR_SUCCESS)
		{
			std::cout << "Controller Connected on port :" + std::to_string(i) << std::endl;
			controllers[i]->m_connected = true;
		}
		else
		{
			//controller is not connected
			controllers[i]->m_connected = false;
		}
	}
}

void CheckControllerState(std::vector<std::shared_ptr<Controller>>& controllers)
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (controllers[i]->m_connected == true)
		{
			//only update slots that are connected for performance
			XInputGetState(i, &controllers[i]->m_state);
			if (controllers[i]->m_state.Gamepad.wButtons == XINPUT_GAMEPAD_X)
			{
				std::string preFix = "Controller (" + std::to_string(i) + ") :";
				std::cout << preFix << " X Button was pressed" << std::endl;
			}
		}

	}
}


int main()
{
	//https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
	std::vector<std::shared_ptr<Controller>> controllers;

	InitializeControllers(controllers);

	//do this every few secconds
	CheckControllerConnections(controllers);
	do
	{
		CheckControllerState(controllers);


	} while (1);

}