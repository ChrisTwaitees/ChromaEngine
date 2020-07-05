#ifndef CHROMA_CONTROLLER_CODES_H
#define CHROMA_CONTROLLER_CODES_H

//#ifndef PS4_CONTROLLER
//#define PS4_CONTROLLER
//#endif

#ifndef XBOX_CONTROLLER
#define XBOX_CONTROLLER
#endif


namespace Chroma
{
	typedef enum class ControllerCode : uint16_t
	{
		// From glfw3.h
		ButtonCross = 0,
		ButtonCircle = 1,
		ButtonSquare = 2,
		ButtonTriangle = 3,
		ButtonL1 = 4,
		ButtonR1 = 5,
		ButtonL2 = 6,
		ButtonR2 = 7,
		ButtonL3 = 8,
		ButtonR3 = 9,
		ButtonDpadUp = 10,
		ButtonDpadRight = 11,
		ButtonDpadDown = 12,
		ButtonDpadLeft = 13,

		AxisLH = ButtonCross,
		AxisLV = ButtonCircle,
		AxisRH = ButtonSquare,
		AxisRV = ButtonTriangle,
		AxisL2 = ButtonL1,
		AxisR2 = ButtonR1

	} Controller;

	inline std::ostream& operator<<(std::ostream& os, ControllerCode controllerCode)
	{
		os << static_cast<int32_t>(controllerCode);
		return os;
	}
}

#ifdef PS4_CONTROLLER
#define CHROMA_CONTROLLER_CROSS_MAPPING 1
#define CHROMA_CONTROLLER_SQUARE_MAPPING 0
#define CHROMA_CONTROLLER_CIRCLE_MAPPING 2
#define CHROMA_CONTROLLER_TRIANGLE_MAPPING 3
#define CHROMA_CONTROLLER_L1_MAPPING 4
#define CHROMA_CONTROLLER_R1_MAPPING 5
#define CHROMA_CONTROLLER_L2_MAPPING 6
#define CHROMA_CONTROLLER_R2_MAPPING 7
#define CHROMA_CONTROLLER_L3_MAPPING 10
#define CHROMA_CONTROLLER_R3_MAPPING 11
#define CHROMA_CONTROLLER_DPADRIGHT_MAPPING 15
#define CHROMA_CONTROLLER_DPADLEFT_MAPPING 17
#define CHROMA_CONTROLLER_DPADUP_MAPPING 14
#define CHROMA_CONTROLLER_DPADDOWN_MAPPING 16
#define CHROMA_CONTROLLER_SHARE_MAPPING 8
#define CHROMA_CONTROLLER_OPTIONS_MAPPING 9
#define CHROMA_CONTROLLER_TOUCHPAD_MAPPING 13
// Axes
#define CHROMA_CONTROLLER_LH_AXIS 0
#define CHROMA_CONTROLLER_LV_AXIS 1
#define CHROMA_CONTROLLER_RH_AXIS 2
#define CHROMA_CONTROLLER_RV_AXIS 5
#define CHROMA_CONTROLLER_L2_AXIS 3
#define CHROMA_CONTROLLER_R2_AXIS 4
#endif

#ifdef XBOX_CONTROLLER
// Buttons
#define CHROMA_CONTROLLER_CROSS_MAPPING 	 ::Chroma::Controller::ButtonCross
#define CHROMA_CONTROLLER_CIRCLE_MAPPING	 ::Chroma::Controller::ButtonCircle
#define CHROMA_CONTROLLER_SQUARE_MAPPING 	 ::Chroma::Controller::ButtonSquare
#define CHROMA_CONTROLLER_TRIANGLE_MAPPING 	 ::Chroma::Controller::ButtonTriangle
#define CHROMA_CONTROLLER_L1_MAPPING 		 ::Chroma::Controller::ButtonL1
#define CHROMA_CONTROLLER_R1_MAPPING 		 ::Chroma::Controller::ButtonR1
#define CHROMA_CONTROLLER_L2_MAPPING 		 ::Chroma::Controller::ButtonL2
#define CHROMA_CONTROLLER_R2_MAPPING 		 ::Chroma::Controller::ButtonR2
#define CHROMA_CONTROLLER_L3_MAPPING 		 ::Chroma::Controller::ButtonL3
#define CHROMA_CONTROLLER_R3_MAPPING 		 ::Chroma::Controller::ButtonR3
#define CHROMA_CONTROLLER_DPADUP_MAPPING	 ::Chroma::Controller::ButtonDpadUp
#define CHROMA_CONTROLLER_DPADRIGHT_MAPPING  ::Chroma::Controller::ButtonDpadRight
#define CHROMA_CONTROLLER_DPADDOWN_MAPPING 	 ::Chroma::Controller::ButtonDpadDown
#define CHROMA_CONTROLLER_DPADLEFT_MAPPING 	 ::Chroma::Controller::ButtonDpadLeft
// Axes	
#define CHROMA_CONTROLLER_LH_AXIS            ::Chroma::Controller::AxisLH
#define CHROMA_CONTROLLER_LV_AXIS            ::Chroma::Controller::AxisLV
#define CHROMA_CONTROLLER_RH_AXIS            ::Chroma::Controller::AxisRH
#define CHROMA_CONTROLLER_RV_AXIS            ::Chroma::Controller::AxisRV
#define CHROMA_CONTROLLER_L2_AXIS            ::Chroma::Controller::AxisL2
#define CHROMA_CONTROLLER_R2_AXIS            ::Chroma::Controller::AxisR2
#endif



#endif