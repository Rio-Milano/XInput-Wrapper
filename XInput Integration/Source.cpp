

#include"XInput_Wrapper.h"
#include<iostream>

int main()
{
	XInput_Wrapper input;

	/*
	sound 

	*/

	do
	{
		input.Update();

		std::shared_ptr<Controller> controller = input.GetController(0);

		if (controller->GetIsConnected())
		{
			if (controller->GetIsKeyPressed(XINPUT_GAMEPAD_A))
			{
				controller->SetVibrationState(controller->GetLeftTrigger() * 65535, controller->GetRightTrigger() * 65535);
			}
			else
				controller->SetVibrationState(0, 0);
		}


	} while (1);


}