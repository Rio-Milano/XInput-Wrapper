

#include"XInput_Wrapper.h"

int main()
{
	XInput_Wrapper input;
	input.InitializeControllers();

	do
	{
		input.Update();

		if (input.GetController(0)->m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			std::cout << input.GetController(0)->leftAnalogStickY << std::endl;
		}


	} while (1);


}