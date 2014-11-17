#ifndef ROBOT_ENGINE_SPEED_CHANGE_COMMAND
#define ROBOT_ENGINE_SPEED_CHANGE_COMMAND

#include "ClientCommand.h"

#include "../KheperaRobot.h"

/*
          Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

             +-------------------+----------------------------------------+-------------------+
             |                   |                                        |                   |
             |   COMMAND_ID      |                 ROBOT_ID               |     ENGINE_ID     |
			 |     1 byte        |                  2 bytes               |      1 byte       |
			 +--------------------------------------------------------------------------------+
			 |                                                                                |
			 |                                                                                |
			 |                             NEW_SPEED_VALUE                                    | 
			 |                                64 bytes                                        |
			 |                                                                                |
			 |                                                                                |
			 +--------------------------------------------------------------------------------+
*/

class RobotMotorSpeedChangeCommand : public ClientCommand
{
	public:
		static const int LEFT_MOTOR_ID = 0;
		static const int RIGHT_MOTOR_ID = 1;

		RobotMotorSpeedChangeCommand();
		virtual ~RobotMotorSpeedChangeCommand();

		virtual uint16_t Execute(Symulation* sym, SOCKET sock);
	private:

};

#endif
