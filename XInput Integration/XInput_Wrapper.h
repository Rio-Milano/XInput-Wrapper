#ifndef XINPUT_WRAPPER_H
#define XINPUT_WRAPPER_H

#include<Windows.h>
#include<Xinput.h>
#include<vector>
#include<memory>
#include<chrono>

/*
https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
https://www.geeksforgeeks.org/chrono-in-c/

*/
class Controller
{
	friend class XInput_Wrapper;
public:
	const bool GetIsKeyPressed(const WORD& key) const;
	const bool& GetIsConnected()const;
	const DWORD& GetIndex()const;
	const float& GetLeftStickX()const;
	const float& GetLeftStickY()const;
	const float& GetRightStickX()const;
	const float& GetRightStickY()const;
	const float& GetLeftTrigger()const;
	const float& GetRightTrigger()const;
	const WORD& GetLeftMotorSpeed()const;
	const WORD& GetRightMotorSpeed()const;

	void SetVibrationState(WORD leftMotorSpeed, WORD rightMotorSpeed);


private:

	XINPUT_STATE m_state{0};
	XINPUT_VIBRATION m_vibrationState{0};
	bool m_connected{ false };
	DWORD m_index{ 0 };
	
	float m_leftAnalogStickX{0.f};
	float m_leftAnalogStickY{0.f};
	float m_rightAnalogStickX{0.f};
	float m_rightAnalogStickY{0.f};
	
	float m_leftTrigger{ 0.f };
	float m_rightTrigger{ 0.f };

	//rumble frequency constraints
	const WORD m_MIN_RUMBLE_SPEED{ 0 };
	const WORD m_MAX_RUMBLE_SPEED{ 65535 };

};


class XInput_Wrapper
{

public:
	XInput_Wrapper();
	void Update();
	void SetAnalogStickConstraints(float leftDeadZone, const float& leftRadius, float rightDeadZone, const float& rightRadius);
	void SetCheckForControllerIntervals(const float& intervals);
	void SetTriggerThresholds(BYTE leftThreshold, BYTE rightThreshold);

	const std::shared_ptr<Controller>& GetController(const int& index) const;

private:
	void InitializeControllers();
	void CheckControllerConnections();
	void ApplyDeadZoneToAnalogStick(std::shared_ptr<Controller>& controller);
	void ApplyThresholdToTriggers(std::shared_ptr<Controller>& controller);
	void CheckControllerState();


	std::vector<std::shared_ptr<Controller>> m_controllers{};
	std::chrono::time_point<std::chrono::system_clock> m_A{}, m_B{};
	
	double m_checkForConnectedControllersInIntervalsOfX{ 3.0 };
	
	float m_leftAnalogStickRadius{32767};
	float m_leftAnalogStickDeadZone{ 100.f };
	float m_rightAnalogStickRadius{ 32767 };
	float m_rightAnalogStickDeadZone{ 100 };
	BYTE m_leftTriggerThreshold{ 30 };
	BYTE m_rightTriggerThreshold{ 30 };

	//XInput Constraints
	const float XINPUT_MIN_DEADZONE{ 0.f };
	const float XINPUT_MAX_DEADZONE{ 65534.f };


};


#endif