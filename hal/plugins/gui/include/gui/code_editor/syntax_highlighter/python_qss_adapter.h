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

#include <QTextCharFormat>
#include <QWidget>

namespace hal
{
    /**
     * @ingroup python
     * @brief QSS Adapter to provide the color configurations used by the PythonSyntaxHighlighter.
     */
    class PythonQssAdapter : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(QColor keywordColor READ keywordColor WRITE setKeywordColor)
        Q_PROPERTY(QColor operator_color READ operator_color WRITE setOperatorColor)
        Q_PROPERTY(QColor braceColor READ braceColor WRITE setBraceColor)
        Q_PROPERTY(QColor defclassColor READ defclassColor WRITE setDefclassColor)
        Q_PROPERTY(QColor selfColor READ selfColor WRITE setSelfColor)
        Q_PROPERTY(QColor numberColor READ numberColor WRITE setNumberColor)
        Q_PROPERTY(QColor singleQuotedStringColor READ singleQuotedStringColor WRITE setSingleQuotedStringColor)
        Q_PROPERTY(QColor doubleQuotedStringColor READ doubleQuotedStringColor WRITE setDoubleQuotedStringColor)
        Q_PROPERTY(QColor commentColor READ commentColor WRITE setCommentColor)

    public:
        /**
         * The Constructor.
         *
         * @param parent - The widget's parent.
         */
        explicit PythonQssAdapter(QWidget* parent = nullptr);

        /**
         * The destructor.
         */
        ~PythonQssAdapter();    // DEBUG CODE, DELETE LATER

        /**
         * Get the singleton instance of the adapter.
         *
         * @return The singleton instance.
         */
        static PythonQssAdapter* instance();

        /**
         * Reloads the Q_PROPERTIES from the stylesheet.
         */
        void repolish();

        /** @name Q_PROPERTY READ Functions
         */
        ///@{
        QColor keywordColor() const;
        QColor operator_color() const;
        QColor braceColor() const;
        QColor defclassColor() const;
        QColor selfColor() const;
        QColor numberColor() const;
        QColor singleQuotedStringColor() const;
        QColor doubleQuotedStringColor() const;
        QColor commentColor() const;
        ///@}
        /** @name Q_PROPERTY WRITE Function*/
        ///@{
        void setKeywordColor(const QColor& color);
        void setOperatorColor(const QColor& color);
        void setBraceColor(const QColor& color);
        void setDefclassColor(const QColor& color);
        void setSelfColor(const QColor& color);
        void setNumberColor(const QColor& color);
        void setSingleQuotedStringColor(const QColor& color);
        void setDoubleQuotedStringColor(const QColor& color);
        void setCommentColor(const QColor& color);
        ///@}

        // LEFT PUBLIC INTENTIONALLY
        QTextCharFormat mKeywordFormat;
        QTextCharFormat mOperatorFormat;
        QTextCharFormat mBraceFormat;
        QTextCharFormat mDefclassFormat;
        QTextCharFormat mSelfFormat;
        QTextCharFormat mNumberFormat;
        QTextCharFormat mSingleQuotedStringFormat;
        QTextCharFormat mDoubleQuotedStringFormat;
        QTextCharFormat mCommentFormat;

    private:
        //QColor mTextColor;
        QColor mKeywordColor;
        QColor mOperatorColor;
        QColor mBraceColor;
        QColor mDefclassColor;
        QColor mSelfColor;
        QColor mNumberColor;
        QColor mSingleQuotedStringColor;
        QColor mDoubleQuotedStringColor;
        QColor mCommentColor;
    };
}
