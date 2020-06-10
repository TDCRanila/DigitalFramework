#pragma once

namespace DCore
{
    class ApplicationInstance
    {
    public:
        virtual ~ApplicationInstance();

        void RunApplication();

    protected:
        ApplicationInstance();

        virtual void PreApplicationLoad();
        virtual void ApplicationLoad();

    private:
        void InitApplication();
        void UpdateApplication();
        void TerminateApplication();

    };

} // End of namespace ~ DCore
