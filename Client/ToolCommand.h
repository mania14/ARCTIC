#pragma once
#include "../CommonUtil/Singletone.h"

enum ToolCommand
{
	ToolCommand_TransformPositionMode,
	ToolCommand_TransformScaleMode,
	ToolCommand_TransformRotationMode,
};

class ToolCommandManager : public Singletone<ToolCommandManager>
{
public:
	explicit ToolCommandManager();
	virtual ~ToolCommandManager();

public:
	int		 executeCommand(ToolCommand command, void* input, void* output);
};