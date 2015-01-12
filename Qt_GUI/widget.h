#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QAbstractButton>
#include <QFileDialog>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
	void myFunction(QString cad);
	void exeLine();
	void on_buttonBox_clicked(QAbstractButton *button);
	void on_videoFileButton_clicked();

	void on_thumbDirButton_clicked();
	void on_imagesFileButton_clicked();
	QString buildLine();

private:
    Ui::Widget *ui;
	QString dir;
	QString line;
	QString errBlank;
};

#endif // WIDGET_H
