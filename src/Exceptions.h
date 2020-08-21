//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_EXCEPTIONS_H
#define F4MPCLIENT_EXCEPTIONS_H


#include <Windows.h>
#include <stdexcept>

namespace F4MP {

    namespace Core {

        namespace Exceptions {
            class RuntimeException : public std::runtime_error
            {
            public:
                explicit RuntimeException(const std::string &msg) :
                        std::runtime_error(msg.c_str())
                { }

                explicit RuntimeException(const char *msg) :
                        std::runtime_error(msg)
                { }
            };

            class DetourException : public RuntimeException
            {
            public:
                explicit DetourException(const std::string &msg) :
                        RuntimeException(msg.c_str())
                { }

                explicit DetourException(const char *msg) :
                        RuntimeException(msg)
                { }
            };

            class GenericWinAPIException : public std::runtime_error
            {
                DWORD last_error_;

            public:
                explicit GenericWinAPIException(const std::string &msg) :
                        std::runtime_error(msg.c_str()), last_error_(GetLastError())
                { }

                explicit GenericWinAPIException(const char *msg) :
                        std::runtime_error(msg), last_error_(GetLastError())
                { }

                virtual DWORD get_last_error() const
                {
                    return last_error_;
                }
            };

            class ModuleNotFoundException : public GenericWinAPIException
            {
            public:
                explicit ModuleNotFoundException(const std::string &msg) :
                        GenericWinAPIException(msg)
                { }

                explicit ModuleNotFoundException(const char *msg) :
                        GenericWinAPIException(msg)
                { }
            };

            class ProcAddressNotFoundException : public GenericWinAPIException
            {
            public:
                explicit ProcAddressNotFoundException(const std::string &msg) :
                        GenericWinAPIException(msg)
                { }

                explicit ProcAddressNotFoundException(const char *msg) :
                        GenericWinAPIException(msg)
                { }
            };

            class COMInterfaceException : public RuntimeException
            {
                HRESULT hresult_;

            public:
                explicit COMInterfaceException(const std::string &msg, HRESULT result) :
                        RuntimeException(msg), hresult_(result)
                { }

                explicit COMInterfaceException(const char *msg, HRESULT result) :
                        RuntimeException(msg), hresult_(result)
                { }

                HRESULT hresult() const
                {
                    return hresult_;
                }
            };

            class DXAPIException : public COMInterfaceException
            {
            public:
                explicit DXAPIException(const std::string &msg, HRESULT result) :
                        COMInterfaceException(msg, result)
                { }

                explicit DXAPIException(const char *msg, HRESULT result) :
                        COMInterfaceException(msg, result)
                { }
            };

            class ARCException : public COMInterfaceException
            {
            public:
                explicit ARCException(const std::string &msg, HRESULT result) :
                        COMInterfaceException(msg, result)
                { }

                explicit ARCException(const char *msg, HRESULT result) :
                        COMInterfaceException(msg, result)
                { }
            };
        };

    };

};

#endif //F4MPCLIENT_EXCEPTIONS_H
