/*
Copyright 2016 University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "libcellml/component.h"
#include "libcellml/logger.h"
#include "libcellml/types.h"

#include <vector>

namespace libcellml {

/**
 * @brief The Logger::LoggerImpl struct.
 *
 * This struct is the private implementation struct for the LoggerImpl class.  Separating
 * the implementation from the definition allows for greater flexibility when
 * distributing the code.
 */
struct Logger::LoggerImpl
{
    std::vector<IssuePtr> mErrors;
};

Logger::Logger()
    : mPimpl(new LoggerImpl())
{
}

Logger::~Logger()
{
    delete mPimpl;
}

void Logger::removeAllErrors()
{
    mPimpl->mErrors.clear();
}

void Logger::addError(const IssuePtr &issue)
{
    mPimpl->mErrors.push_back(issue);
}

size_t Logger::errorCount() const
{
    return mPimpl->mErrors.size();
}

IssuePtr Logger::error(size_t index) const
{
    IssuePtr err = nullptr;
    if (index < mPimpl->mErrors.size()) {
        err = mPimpl->mErrors.at(index);
    }
    return err;
}



// IssuePtr Logger::error(size_t index) const
// {
//     IssuePtr err = nullptr;
//     if (index < mPimpl->mIssues.size()) {
//         err = mPimpl->mErrors.at(index);
//     }
//     return err;
// }


} // namespace libcellml
