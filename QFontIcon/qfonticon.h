/*
MIT License

Copyright (c) 2017 Sacha Schutz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <QApplication>
#include <QIconEngine>
#include <QObject>
#include <QPainter>
#include <QPalette>
#include <QtCore>

class QFontIcon;
class QFontIconEngine;
#define FIcon QFontIcon::icon

class QFontIconEngine : public QIconEngine {
public:
	explicit QFontIconEngine(QPalette const& palette_reference);
	~QFontIconEngine();
	void paint(QPainter* const painter, QRect const& rect, QIcon::Mode mode, QIcon::State state) Q_DECL_OVERRIDE;
	QPixmap pixmap(QSize const& size, QIcon::Mode mode, QIcon::State state) Q_DECL_OVERRIDE;
	void setFontFamily(QString const& family);
	void setLetter(QChar const& letter);
	void setBaseColor(QColor const& baseColor);
	QIconEngine* clone() const override;
protected:
	QString fontFamily;
	QChar letter;
	QColor baseColor;

	QPalette const& palette_reference;
};

extern QPalette helper_palette;

class QFontIcon : public QObject {
	Q_OBJECT
public:
	static bool addFont(QString const& filename);
	static QFontIcon* instance();
	static QIcon icon(QChar const& code, QPalette const& palette_reference = helper_palette, QColor const& baseColor = QColor(), QString const& family = QString());
	QStringList const& families() const;
protected:
	void addFamily(const QString& family);
	explicit QFontIcon(QObject* parent = 0);
	~QFontIcon();
	static QFontIcon* m_instance;
	QStringList m_families;
};
