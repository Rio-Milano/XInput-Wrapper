

#include"XInput_Wrapper.h"

int main()
{
	XInput_Wrapper input;
	
	/*
	add setter functions for wrapper constraints
	add vibration effects
	sound 
	triggers

	*/

	do
	{
		input.Update();

		auto controller = input.GetController(0);

		if (controller->GetIsKeyPressed(XINPUT_GAMEPAD_A) && 
			controller->GetIsKeyPressed(XINPUT_GAMEPAD_B) && 
			controller->GetIsKeyPressed(XINPUT_GAMEPAD_X) && 
			controller->GetIsKeyPressed(XINPUT_GAMEPAD_Y))
		{
			std::cout << "ABXY" << std::endl;
		}


	} while (1);


}