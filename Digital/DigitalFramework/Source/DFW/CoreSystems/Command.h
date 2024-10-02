#pragma once

namespace DFW
{
    class Command
    {
    public:
        Command() = default;
        virtual ~Command() = default;

        virtual void Execute() = 0;
    };

} // End of namespace ~ DFW.
