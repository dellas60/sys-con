#include "SwitchVirtualGamepadHandler.h"

SwitchVirtualGamepadHandler::SwitchVirtualGamepadHandler(std::unique_ptr<IController> &&controller)
    : m_controllerHandler(std::move(controller))
{
}

SwitchVirtualGamepadHandler::~SwitchVirtualGamepadHandler()
{
}

Result SwitchVirtualGamepadHandler::Initialize()
{
    return 0;
}

void SwitchVirtualGamepadHandler::Exit()
{
}

void SwitchVirtualGamepadHandler::InputThreadLoop(void *handler)
{
    while (static_cast<SwitchVirtualGamepadHandler*>(handler)->m_inputThreadIsRunning)
    {
        static_cast<SwitchVirtualGamepadHandler *>(handler)->UpdateInput();
    }
}

void SwitchVirtualGamepadHandler::OutputThreadLoop(void *handler)
{
    while (static_cast<SwitchVirtualGamepadHandler*>(handler)->m_outputThreadIsRunning)
    {
       static_cast<SwitchVirtualGamepadHandler *>(handler)->UpdateOutput();
    }
}

Result SwitchVirtualGamepadHandler::InitInputThread()
{
    R_TRY(m_inputThread.Initialize(&SwitchVirtualGamepadHandler::InputThreadLoop, this, 0x3B).GetValue());
    m_inputThreadIsRunning = true;
    return m_inputThread.Start().GetValue();
}

void SwitchVirtualGamepadHandler::ExitInputThread()
{
    m_inputThreadIsRunning = false;
    m_inputThread.CancelSynchronization();
    m_inputThread.Join();
}

Result SwitchVirtualGamepadHandler::InitOutputThread()
{
    R_TRY(m_outputThread.Initialize(&SwitchVirtualGamepadHandler::OutputThreadLoop, this, 0x3B).GetValue());
    m_outputThreadIsRunning = true;
    return m_outputThread.Start().GetValue();
}

void SwitchVirtualGamepadHandler::ExitOutputThread()
{
    m_outputThreadIsRunning = false;
    m_outputThread.CancelSynchronization();
    m_outputThread.Join();
}