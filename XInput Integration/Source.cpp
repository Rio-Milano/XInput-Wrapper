

#include"XInput_Wrapper.h"
#include<iostream>
#include<stdlib.h>
#include<chrono>
int main()
{
	XInput_Wrapper input;

	/*
	sound 

	*/

	std::chrono::system_clock::time_point A, B;

	do
	{
		input.Update();

		system("cls");

		std::shared_ptr<Controller> controller = input.GetController(0);

		if (controller->GetIsConnected())
		{

			if (controller->GetIsKeyPressed(XINPUT_GAMEPAD_A))
			{
				controller->SetVibrationState(static_cast<WORD>(controller->GetLeftTrigger() * 65535.f), static_cast<WORD>(controller->GetRightTrigger() * 65535.f));
			}
			else
				controller->SetVibrationState(0, 0);

			std::cout << "CONTROLLER PORTS" << std::endl;
			std::cout << "Port 0 :" << (input.GetController(0)->GetIsConnected() ? "true" : "false") << std::endl;
			std::cout << "Port 1 :" << (input.GetController(1)->GetIsConnected() ? "true" : "false") << std::endl;
			std::cout << "Port 2 :" << (input.GetController(2)->GetIsConnected() ? "true" : "false") << std::endl;
			std::cout << "Port 3 :" << (input.GetController(3)->GetIsConnected() ? "true" : "false") << std::endl << std::endl;

			std::cout << "ANALOG STICKS" << std::endl;
			std::cout << "Left Stick X :" << controller->GetLeftStickX() << std::endl;
			std::cout << "Left Stick Y :" << controller->GetLeftStickY() << std::endl;			
			std::cout << "Right Stick X :" << controller->GetRightStickX() << std::endl;
			std::cout << "Right Stick Y :" << controller->GetRightStickY() << std::endl << std::endl;

			std::cout << "BUTTONS" << std::endl;
			std::cout << "X :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_X) ? "true" : "false") << std::endl;
			std::cout << "Y :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_Y) ? "true" : "false") << std::endl;
			std::cout << "A :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_A) ? "true" : "false") << std::endl;
			std::cout << "B :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_B) ? "true" : "false") << std::endl;
			std::cout << "D-PAD Up :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_DPAD_UP) ? "true" : "false") << std::endl;
			std::cout << "D-PAD Down :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_DPAD_DOWN) ? "true" : "false") << std::endl;
			std::cout << "D-PAD Left :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_DPAD_LEFT) ? "true" : "false") << std::endl;
			std::cout << "D-PAD Right :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_DPAD_RIGHT) ? "true" : "false") << std::endl;
			std::cout << "Left Analog Thumstick :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_LEFT_THUMB) ? "true" : "false") << std::endl;
			std::cout << "Right Analog Thumstick :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_RIGHT_THUMB) ? "true" : "false") << std::endl;
			std::cout << "Left Shoulder Button :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_LEFT_SHOULDER) ? "true" : "false") << std::endl;
			std::cout << "Right Shoulder Button :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) ? "true" : "false") << std::endl;
			std::cout << "Back :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_BACK) ? "true" : "false") << std::endl;
			std::cout << "Start :" << (controller->GetIsKeyPressed(XINPUT_GAMEPAD_START) ? "true" : "false") << std::endl << std::endl;


			std::cout << "TRIGGERS" << std::endl;
			std::cout << "Left Trigger :" << controller->GetLeftTrigger() << std::endl;
			std::cout << "Right Trigger :" << controller->GetRightTrigger() << std::endl << std::endl;

			std::cout << "VIBRARION" << std::endl;
			std::cout << "Left Vibration :" << controller->GetLeftMotorSpeed() << std::endl;
			std::cout << "Right Vibration :" << controller->GetRightMotorSpeed() << std::endl;


		}
		std::chrono::duration<double> elapsed = B - A;
		while (elapsed.count() < 0.3)
		{
			B = std::chrono::system_clock::now();
			elapsed = B - A;
		}
		A = B;

	} while (1);


}