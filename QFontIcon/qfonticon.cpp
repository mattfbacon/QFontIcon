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

#include <QDebug>
#include <QFontDatabase>

#include "qfonticon.h"
QFontIcon* QFontIcon::m_instance = Q_NULLPTR;

bool QFontIcon::addFont(QString const& filename) {
	int const id = QFontDatabase::addApplicationFont(filename);
	if (id == -1) {
		qWarning() << QStringLiteral("Cannot load font %0").arg(filename);
		return false;
	}

	instance()->addFamily(QFontDatabase::applicationFontFamilies(id).first());
	return true;
}

QFontIcon* QFontIcon::instance() {
	if (!m_instance) { m_instance = new QFontIcon; }
	return m_instance;
}

QIcon QFontIcon::icon(QChar const& code, QPalette const& palette_reference, QColor const& baseColor, QString const& family) {
	QFontIconEngine* engine = new QFontIconEngine{
		palette_reference,
	};
	engine->setFontFamily(family.isEmpty() ? instance()->families().first() : family);
	engine->setLetter(code);
	engine->setBaseColor(baseColor);
	return QIcon(engine);
}

QStringList const& QFontIcon::families() const {
	return m_families;
}

void QFontIcon::addFamily(QString const& family) {
	m_families.append(family);
}

QFontIcon::QFontIcon(QObject* parent) : QObject(parent) {
	//
}

QFontIcon::~QFontIcon() {
	//
}

//=======================================================================================================

QFontIconEngine::QFontIconEngine(QPalette const& palette_reference) : QIconEngine{}, palette_reference(palette_reference) {
	//
}

QFontIconEngine::~QFontIconEngine() {
	//
}

QPalette::ColorGroup color_group_for_mode(QIcon::Mode const mode) {
	switch (mode) {
		case QIcon::Disabled:
			return QPalette::Disabled;
		case QIcon::Active:
		case QIcon::Selected:
			return QPalette::Active;
		default:
			return QPalette::Normal;
	}
}

void QFontIconEngine::paint(QPainter* const painter, QRect const& rect, QIcon::Mode const mode, QIcon::State const state) {
	Q_UNUSED(state);
	QFont font{
		fontFamily,
	};
	font.setPixelSize(qRound(rect.height() * 0.9));

	QColor const& penColor = palette_reference.color(color_group_for_mode(mode), QPalette::ButtonText);

	painter->save();

	painter->setPen(QPen{
		penColor,
	});
	painter->setFont(font);
	painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, letter);

	painter->restore();
}

QPixmap QFontIconEngine::pixmap(QSize const& size, QIcon::Mode const mode, QIcon::State const state) {
	QPixmap pix{
		size,
	};
	pix.fill(Qt::transparent);

	QPainter painter{
		&pix,
	};
	paint(
		&painter,
		QRect{
			QPoint{
				0,
				0,
			},
			size,
		},
		mode,
		state);
	return pix;
}

void QFontIconEngine::setFontFamily(QString const& family) {
	fontFamily = family;
}

void QFontIconEngine::setLetter(QChar const& letter) {
	this->letter = letter;
}

void QFontIconEngine::setBaseColor(QColor const& baseColor) {
	this->baseColor = baseColor;
}

QIconEngine* QFontIconEngine::clone() const {
	QFontIconEngine* engine = new QFontIconEngine{
		palette_reference,
	};
	engine->setFontFamily(fontFamily);
	engine->setBaseColor(baseColor);
	engine->setLetter(letter);
	return engine;
}
