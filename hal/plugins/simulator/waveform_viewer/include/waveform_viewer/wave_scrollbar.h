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

#include <QScrollBar>

namespace hal {
    class WaveTransform;

    class WaveScrollbar : public QScrollBar
    {
        const WaveTransform* mTransform;
        double mVleft;
        double mVmaxScroll;
        int mVieportWidth;
        int mLastValue;
        bool mHandleSliderChange;
        using QScrollBar::value;
        using QScrollBar::setValue;
        using QScrollBar::setMaximum;
        void setVleftIntern(double v);
        static int toUInt(double v);
    protected:
        void sliderChange(SliderChange change) override;
    public:
        WaveScrollbar(const WaveTransform* trans, QWidget* parent = nullptr);
        void adjust(quint64 visibleWidth);
        double vLeft() const;
        void setVleft(double v);
        double tLeftF() const;
        quint64 tLeftI() const;
        void updateScale(double deltaScale, double tEvent, quint64 visibleWidth);
        int xPosI(double t) const;
        double xPosF(double t) const;
        double tPosF(int x) const;
        quint64 tPosI(int x) const;
        int viewportWidth() const { return mVieportWidth; }
    };
}
