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

#include <QFrame>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QTimer>

namespace hal {

    class GraphContext;

    class AbstractBusyIndicator : public QFrame
    {
        Q_OBJECT
    public:
        AbstractBusyIndicator(QWidget* parent = nullptr) : QFrame(parent) {;}
        virtual void setValue(int percent) = 0;
        virtual void setText(const QString& txt) = 0;
    };

    class ProgressBar : public AbstractBusyIndicator
    {
        Q_OBJECT

        QProgressBar* mProgressBar;
        QLabel*       mLabel;
        QPushButton*  mButAbort;

    public:
        ProgressBar(GraphContext* context, QWidget* parent = nullptr);
        void setValue(int percent) override;
        void setText(const QString& txt) override;
    };

    class BusyAnimation : public QWidget
    {
        Q_OBJECT
        QImage mImage;
        double mAngle;
    public Q_SLOTS:
        void handleTimeout();
    protected:
        void paintEvent(QPaintEvent* event) override;
    public:
        BusyAnimation(QWidget* parent = nullptr);
    };

    class BusyIndicator : public AbstractBusyIndicator
    {
        BusyAnimation* mAnimation;
        QTimer* mTimer;
        QLabel* mLabel;
        QProgressBar* mProgressBar;
    public:
        BusyIndicator(QWidget* parent = nullptr);
        ~BusyIndicator();
        void setValue(int percent) override;
        void setText(const QString& txt) override;
    };
}

