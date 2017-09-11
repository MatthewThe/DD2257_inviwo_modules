/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2017 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_CSVREADER_H
#define IVW_CSVREADER_H

#include <dd2257lab1/dd2257lab1moduledefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/io/datareader.h>
#include <dd2257lab1/utils/dataframe.h>

namespace inviwo {

/**
 * \class CSVReader
 * \ingroup dataio
 *
 * \brief A reader for comma separated value (CSV) files with customizable delimiters.
 */
class IVW_MODULE_DD2257LAB1_API CSVReader : public DataReaderType<DataFrame> { 
public:
    CSVReader();
    CSVReader(const CSVReader&) = default;
    CSVReader(CSVReader&&) = default;
    CSVReader& operator=(const CSVReader&) = default;
    CSVReader& operator=(CSVReader&&) = default;
    virtual CSVReader* clone() const override;
    virtual ~CSVReader() = default;

    void setDelimiters(const std::string &delim);
    void setFirstRowHeader(bool hasHeader);

    virtual std::shared_ptr<DataFrame> readData(const std::string& fileName) override;

private:
    std::string delimiters_;
    bool firstRowHeader_;
};

} // namespace

#endif // IVW_CSVREADER_H

