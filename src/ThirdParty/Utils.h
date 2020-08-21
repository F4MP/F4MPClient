//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_UTILS_H
#define F4MPCLIENT_UTILS_H

#include <Windows.h>
#include <string>
#include <Psapi.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace F4MP
{
    namespace Core
    {
        namespace Util
        {
            inline std::string expand_environment_variables(const std::string & str)
            {
                std::string expandedStr;
                const DWORD neededSize = ExpandEnvironmentStringsA(str.c_str(),
                                                                   nullptr, 0);
                if (neededSize)
                {
                    expandedStr.resize(neededSize);
                    if (0 == ExpandEnvironmentStringsA(str.c_str(),
                                                       &expandedStr[0],
                                                       neededSize))
                    {
                        // pathological case requires a copy
                        expandedStr = str;
                    }
                }
                // RVO here
                return expandedStr;
            }

            inline std::string process_name()
            {
                const auto nSize = MAX_PATH + 1;
                char procName[nSize];
                GetProcessImageFileName(GetCurrentProcess(), procName, nSize);

                return std::string(procName);
            }
        };
    };
};

#endif //F4MPCLIENT_UTILS_H
