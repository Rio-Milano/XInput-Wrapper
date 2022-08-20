#ifndef XINPUT_WRAPPER_H
#define XINPUT_WRAPPER_H

#include<iostream>
#include<string>

//include xinput headders
#include<Windows.h>
#include<winerror.h>
#include<Xinput.h>
#include<vector>
#include<memory>
#include<chrono>


//link the required libraries
#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")


struct Controller
{
	XINPUT_STATE 
		m_state;

	bool 
		m_connected = false;

	float
		leftAnalogStickX,
		leftAnalogStickY,

		rightAnalogStickX,
		rightAnalogStickY;


};

class XInput_Wrapper
{
	/*
		Referances
		https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
		https://www.geeksforgeeks.org/chrono-in-c/
	*/

public:
	void InitializeControllers()
	{
		controllers.resize(XUSER_MAX_COUNT);
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			controllers[i] = std::make_shared<Controller>();
		}
		CheckControllerConnections(controllers);


	}

	void Update()
	{
		std::chrono::duration<double> elapsed_secconds = B - A;
		if (elapsed_secconds.count() >= CHECK_CONNECTED_CONTROLLER_INTERVALS)
		{
			CheckControllerConnections(controllers);
			A = B;
		}
		B = std::chrono::system_clock::now();

		CheckControllerState(controllers);
	}

	const std::shared_ptr<Controller>& GetController(const int& index) const
	{
		//if(index >= controllers.size())
		//	throw exception
		return controllers[index];
	}



private:

	void CheckControllerConnections(std::vector<std::shared_ptr<Controller>>& controllers)
	{
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			memset(&controllers[i]->m_state, 0, sizeof(XINPUT_STATE));

			if (XInputGetState(i, &controllers[i]->m_state) == ERROR_SUCCESS)
				controllers[i]->m_connected = true;

			else
				controllers[i]->m_connected = false;
		}
	}

	void ApplyDeadZoneToAnalogStick(std::shared_ptr<Controller>& controller)
	{
		////get a referance to the passed controller state
		XINPUT_STATE& state = controller->m_state;

		//initialize some variables to hold the stick position vector
		float LstickPx = 0.0f, LstickPy = 0.0f;

		//assign stick position vector
		LstickPx = static_cast<float>(state.Gamepad.sThumbLX);
		LstickPy = static_cast<float>(state.Gamepad.sThumbLY);

		float
			//calculate the magnitude of the position vector
			LmagnitudeOfStickShift = static_cast<float>(sqrt(pow(static_cast<double>(LstickPx), 2.0) + pow(static_cast<double>(LstickPy), 2.0))),

			//use the magnitude to calculate the normal of the position vector
			LstickShiftNormalX = LstickPx / LmagnitudeOfStickShift,
			LstickShiftNormalY = LstickPy / LmagnitudeOfStickShift;

		//if position vector is not inside the dead zone radius
		if (LmagnitudeOfStickShift > LEFT_ANALOG_STICK_DEADZONE)
		{
			//if position vector is outside the usable stick radius
			if (LmagnitudeOfStickShift > LEFT_MAX_ANALOG_STICK_RADIUS)
				//set the magnitude to the useable stick radius
				LmagnitudeOfStickShift = LmagnitudeOfStickShift;

			//subtract the deadzone from the magnitude
			LmagnitudeOfStickShift -= LEFT_ANALOG_STICK_DEADZONE;

			float
				//calculate the maximu magnitude a stick can have
				LmaxMagnitudeOfAnalogStick = LEFT_MAX_ANALOG_STICK_RADIUS - LEFT_ANALOG_STICK_DEADZONE,
				//calculate the magnitude the stick has bassed on the max magnitude
				LrelativeMagnitude = LmagnitudeOfStickShift / LmaxMagnitudeOfAnalogStick;


			controller->leftAnalogStickX = LstickShiftNormalX * LrelativeMagnitude;
			controller->leftAnalogStickY = LstickShiftNormalY * LrelativeMagnitude;
		}
		else
		{
			controller->leftAnalogStickX = 0.f;
			controller->leftAnalogStickY = 0.f;
		}

		//initialize some variables to hold the stick position vector
		float RstickPx = 0.0f, RstickPy = 0.0f;

		//assign stick position vector
		RstickPx = static_cast<float>(state.Gamepad.sThumbRX);
		RstickPy = static_cast<float>(state.Gamepad.sThumbRY);

		float
			//calculate the magnitude of the position vector
			RmagnitudeOfStickShift = static_cast<float>(sqrt(pow(static_cast<double>(RstickPx), 2.0) + pow(static_cast<double>(RstickPy), 2.0))),

			//use the magnitude to calculate the normal of the position vector
			RstickShiftNormalX = RstickPx / RmagnitudeOfStickShift,
			RstickShiftNormalY = RstickPy / RmagnitudeOfStickShift;

		//if position vector is not inside the dead zone radius
		if (RmagnitudeOfStickShift > RIGHT_ANALOG_STICK_DEADZONE)
		{
			//if position vector is outside the usable stick radius
			if (RmagnitudeOfStickShift > RIGHT_MAX_ANALOG_STICK_RADIUS)
				//set the magnitude to the useable stick radius
				RmagnitudeOfStickShift = RmagnitudeOfStickShift;

			//subtract the deadzone from the magnitude
			RmagnitudeOfStickShift -= RIGHT_ANALOG_STICK_DEADZONE;

			float
				//calculate the maximu magnitude a stick can RIGHT_ANALOG_STICK_DEADZONE
				RmaxMagnitudeOfAnalogStick = RIGHT_MAX_ANALOG_STICK_RADIUS - RIGHT_ANALOG_STICK_DEADZONE,
				//calculate the magnitude the stick has bassed on the max magnitude
				RrelativeMagnitude = RmagnitudeOfStickShift / RmaxMagnitudeOfAnalogStick;


			controller->rightAnalogStickX = RstickShiftNormalX * RrelativeMagnitude;
			controller->rightAnalogStickY = RstickShiftNormalY * RrelativeMagnitude;
		}
		else
		{
			controller->rightAnalogStickX = 0.f;
			controller->rightAnalogStickY = 0.f;
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

				ApplyDeadZoneToAnalogStick(controllers[i]);


				/*if (controllers[i]->m_state.Gamepad.wButtons == XINPUT_GAMEPAD_X)
				{
					std::string preFix = "Controller (" + std::to_string(i) + ") :";
					std::cout << preFix << " X Button was pressed" << std::endl;
				}*/
			}

		}
	}

	std::vector<std::shared_ptr<Controller>> controllers;
	std::chrono::time_point<std::chrono::system_clock> A, B;

	double CHECK_CONNECTED_CONTROLLER_INTERVALS = 3.0;

	float LEFT_ANALOG_STICK_DEADZONE = 100.f;
	float LEFT_MAX_ANALOG_STICK_RADIUS = 32767;
	float RIGHT_ANALOG_STICK_DEADZONE = 100;
	float RIGHT_MAX_ANALOG_STICK_RADIUS = 32767;

};

#endif