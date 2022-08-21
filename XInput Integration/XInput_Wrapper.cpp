#include "XInput_Wrapper.h"

#include<iostream>
#include<string>
#include<winerror.h>


const bool Controller::GetIsKeyPressed(const WORD& key) const
{
	return m_state.Gamepad.wButtons & key;
}

const bool& Controller::GetIsConnected() const
{
	return m_connected;
}

const DWORD& Controller::GetIndex() const
{
	return m_index;
}

const float& Controller::GetLeftStickX() const
{
	return m_leftAnalogStickX;
}

const float& Controller::GetLeftStickY() const
{
	return m_leftAnalogStickY;
}

const float& Controller::GetRightStickX() const
{
	return m_rightAnalogStickX;
}

const float& Controller::GetLeftTrigger() const
{
	return m_leftTrigger;
}

const float& Controller::GetRightTrigger() const
{
	return m_rightTrigger;
}

const float& Controller::GetRightStickY() const
{
	return m_rightAnalogStickY;
}

void Controller::SetVibrationState(WORD leftMotorSpeed, WORD rightMotorSpeed)
{
	memset(&m_vibrationState, 0, sizeof(XINPUT_VIBRATION));

	if (leftMotorSpeed < m_MIN_RUMBLE_SPEED)
		leftMotorSpeed = m_MIN_RUMBLE_SPEED;
	else if (leftMotorSpeed > m_MAX_RUMBLE_SPEED)
		leftMotorSpeed = m_MAX_RUMBLE_SPEED;

	m_vibrationState.wLeftMotorSpeed = leftMotorSpeed;

	if (rightMotorSpeed < m_MIN_RUMBLE_SPEED)
		rightMotorSpeed = m_MIN_RUMBLE_SPEED;
	else if (rightMotorSpeed > m_MAX_RUMBLE_SPEED)
		rightMotorSpeed = m_MAX_RUMBLE_SPEED;

	m_vibrationState.wRightMotorSpeed = rightMotorSpeed;

	XInputSetState(m_index, &m_vibrationState);
}

XInput_Wrapper::XInput_Wrapper()
{
	InitializeControllers();
}

void XInput_Wrapper::Update()
{
	std::chrono::duration<double> elapsed_secconds = m_B - m_A;
	if (elapsed_secconds.count() >= m_checkForConnectedControllersInIntervalsOfX)
	{
		CheckControllerConnections();
		m_A = m_B;
	}
	m_B = std::chrono::system_clock::now();

	CheckControllerState();
}

void XInput_Wrapper::SetAnalogStickConstraints(float leftDeadZone, const float& leftRadius, float rightDeadZone, const float& rightRadius)
{
	m_leftAnalogStickRadius = leftRadius;
	m_rightAnalogStickRadius = rightRadius;

	if (leftDeadZone < XINPUT_MIN_DEADZONE)
		leftDeadZone = XINPUT_MIN_DEADZONE;
	else if (leftDeadZone > XINPUT_MAX_DEADZONE)
		leftDeadZone = XINPUT_MAX_DEADZONE;


	if (rightDeadZone < XINPUT_MIN_DEADZONE)
		rightDeadZone = XINPUT_MIN_DEADZONE;
	else if (rightDeadZone > XINPUT_MAX_DEADZONE)
		rightDeadZone = XINPUT_MAX_DEADZONE;

	m_leftAnalogStickDeadZone = leftDeadZone;
	m_rightAnalogStickDeadZone = rightDeadZone;
}

void XInput_Wrapper::SetCheckForControllerIntervals(const float& intervals)
{
	m_checkForConnectedControllersInIntervalsOfX = intervals;
}

void XInput_Wrapper::SetTriggerThresholds(BYTE leftThreshold, BYTE rightThreshold)
{
	m_leftTriggerThreshold = leftThreshold;
	m_rightTriggerThreshold = rightThreshold;
}

const std::shared_ptr<Controller>& XInput_Wrapper::GetController(const int& index) const
{
	//if(index >= controllers.size())
	//	throw exception
	return m_controllers[index];
}

void XInput_Wrapper::InitializeControllers()
{
	m_controllers.resize(XUSER_MAX_COUNT);
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_controllers[i] = std::make_shared<Controller>();
		m_controllers[i]->m_index = i;
	}
	CheckControllerConnections();
}

void XInput_Wrapper::CheckControllerConnections()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		memset(&m_controllers[i]->m_state, 0, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &m_controllers[i]->m_state) == ERROR_SUCCESS)
		{
			if (m_controllers[i]->m_connected == false)
				std::cout << "Controller connected to port " << std::to_string(i) << ".\n";

			m_controllers[i]->m_connected = true;

		}

		else
		{
			if (m_controllers[i]->m_connected == true)
				std::cout << "Controller on port " << std::to_string(i) << " disconnected.\n";

			m_controllers[i]->m_connected = false;
		}
	}
}

void XInput_Wrapper::ApplyDeadZoneToAnalogStick(std::shared_ptr<Controller>& controller)
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
	if (LmagnitudeOfStickShift > m_leftAnalogStickDeadZone)
	{
		//if position vector is outside the usable stick radius
		if (LmagnitudeOfStickShift > m_leftAnalogStickRadius)
			//set the magnitude to the useable stick radius
			LmagnitudeOfStickShift = LmagnitudeOfStickShift;

		//subtract the deadzone from the magnitude
		LmagnitudeOfStickShift -= m_leftAnalogStickDeadZone;

		float
			//calculate the maximu magnitude a stick can have
			LmaxMagnitudeOfAnalogStick = m_leftAnalogStickRadius - m_leftAnalogStickDeadZone,
			//calculate the magnitude the stick has bassed on the max magnitude
			LrelativeMagnitude = LmagnitudeOfStickShift / LmaxMagnitudeOfAnalogStick;


		controller->m_leftAnalogStickX = LstickShiftNormalX * LrelativeMagnitude;
		controller->m_leftAnalogStickY = LstickShiftNormalY * LrelativeMagnitude;
	}
	else
	{
		controller->m_leftAnalogStickX = 0.f;
		controller->m_leftAnalogStickY = 0.f;
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
	if (RmagnitudeOfStickShift > m_rightAnalogStickDeadZone)
	{
		//if position vector is outside the usable stick radius
		if (RmagnitudeOfStickShift > m_rightAnalogStickRadius)
			//set the magnitude to the useable stick radius
			RmagnitudeOfStickShift = RmagnitudeOfStickShift;

		//subtract the deadzone from the magnitude
		RmagnitudeOfStickShift -= m_rightAnalogStickDeadZone;

		float
			//calculate the maximu magnitude a stick can RIGHT_ANALOG_STICK_DEADZONE
			RmaxMagnitudeOfAnalogStick = m_rightAnalogStickRadius - m_rightAnalogStickDeadZone,
			//calculate the magnitude the stick has bassed on the max magnitude
			RrelativeMagnitude = RmagnitudeOfStickShift / RmaxMagnitudeOfAnalogStick;


		controller->m_rightAnalogStickX = RstickShiftNormalX * RrelativeMagnitude;
		controller->m_rightAnalogStickY = RstickShiftNormalY * RrelativeMagnitude;
	}
	else
	{
		controller->m_rightAnalogStickX = 0.f;
		controller->m_rightAnalogStickY = 0.f;
	}

}

void XInput_Wrapper::ApplyThresholdToTriggers(std::shared_ptr<Controller>& controller)
{
	float leftTrigger = static_cast<float>(controller->m_state.Gamepad.bLeftTrigger);
	if (leftTrigger > m_leftTriggerThreshold)
	{
		float maxMagnitude = 255.f - m_leftTriggerThreshold;
		float triggerMagnitude = leftTrigger - m_leftTriggerThreshold;

		controller->m_leftTrigger = triggerMagnitude / maxMagnitude;

	}
	else
	{
		controller->m_leftTrigger = 0.f;
	}

	float rightTrigger = static_cast<float>(controller->m_state.Gamepad.bRightTrigger);
	if (rightTrigger > m_rightTriggerThreshold)
	{
		float maxMagnitude = 255.f - m_rightTriggerThreshold;
		float triggerMagnitude = rightTrigger - m_rightTriggerThreshold;

		controller->m_rightTrigger = triggerMagnitude / maxMagnitude;

	}
	else
	{
		controller->m_rightTrigger = 0.f;
	}
}

void XInput_Wrapper::CheckControllerState()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_controllers[i]->m_connected == true)
		{
			//only update slots that are connected for performance
			XInputGetState(i, &m_controllers[i]->m_state);

			ApplyThresholdToTriggers(m_controllers[i]);
			ApplyDeadZoneToAnalogStick(m_controllers[i]);

		}

	}
}