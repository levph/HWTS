// MIT License
//
// Copyright (c) 2019 Ruhr University Bochum, Chair for Embedded Security. All Rights reserved.
// Copyright (c) 2019 Marc Fyrbiak, Sebastian Wallat, Max Hoffmann ("ORIGINAL AUTHORS"). All rights reserved.
// Copyright (c) 2021 Max Planck Institute for Security and Privacy. All Rights reserved.
// Copyright (c) 2021 Jörn Langheinrich, Julian Speith, Nils Albartus, René Walendy, Simon Klix ("ORIGINAL AUTHORS"). All Rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "hal_core/defines.h"
#include "hal_core/netlist/netlist_writer/netlist_writer.h"

#include <QString>
#include <QXmlStreamWriter>
#include <functional>
#include <map>
#include <sstream>

namespace hal
{
    /* forward declaration */
    class Netlist;
    class Net;
    class Gate;

    /**
     * @ingroup netlist_writer
     */
    class NETLIST_API GexfWriter : public NetlistWriter
    {
    public:
        GexfWriter();
        ~GexfWriter() = default;

        /**
         * Write the netlist to a Gefx file at the provided location.
         *
         * @param[in] netlist - The netlist.
         * @param[in] file_path - The output path.
         * @returns True on success, false otherwise.
         */
        Result<std::monostate> write(Netlist* netlist, const std::filesystem::path& file_path) override;

    private:
        Netlist* mNetlist;
        int mEdgeId;
        bool mGuiLoaded;

        void writeMeta(QXmlStreamWriter& xmlOut) const;
        void writeGraph(QXmlStreamWriter& xmlOut);
        void writeNode(QXmlStreamWriter& xmlOut, const Gate* g) const;
        void writeColor(QXmlStreamWriter& xmlOut, const Gate* g) const;
        void writeEdge(QXmlStreamWriter& xmlOut, const Net* n);
        void writeAttribute(QXmlStreamWriter& xmlOut, int id, const QString& title, const QString& type) const;
        void writeNodeAttribute(QXmlStreamWriter& xmlOut, const Gate* g, int inx) const;
        void writeEdgeAttribute(QXmlStreamWriter& xmlOut, const Net* n, int inx, const std::string pin = std::string()) const;
    };
}    // namespace hal
