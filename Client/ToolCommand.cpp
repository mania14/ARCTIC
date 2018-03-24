#include "stdafx.h"
#include "ToolCommand.h"
#include "ToolTransformManager.h"

ToolCommandManager::ToolCommandManager()
{
}

ToolCommandManager::~ToolCommandManager()
{
}

int ToolCommandManager::executeCommand(ToolCommand command, void * input, void * output)
{
	switch (command)
	{
		case ToolCommand_TransformPositionMode:
		{
			ToolTransformManager::This().setPickMode(ToolTransformManager::TOOL_TRANSFORM_POSITION);
			return 0;
		}
		case ToolCommand_TransformScaleMode:
		{
			ToolTransformManager::This().setPickMode(ToolTransformManager::TOOL_TRANSFORM_SCALE);
			return 0;
		}
		case ToolCommand_TransformRotationMode:
		{
			ToolTransformManager::This().setPickMode(ToolTransformManager::TOOL_TRANSFORM_ROTATE);
			return 0;
		}
	}

	return -1;
}
