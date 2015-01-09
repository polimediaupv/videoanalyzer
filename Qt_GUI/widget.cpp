#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
	errBlank = "ERROR! BLANK SPACE IN ROUTE";
}

Widget::~Widget()
{
    delete ui;
}

void Widget::myFunction(QString cad){
	QProcess *myProcess = new QProcess();
	QDir::setCurrent(dir);
	myProcess->setWorkingDirectory(dir);
	myProcess->startDetached(cad);
	//myProcess->startDetached("videoanalyzer.exe video.mp4 --debugLog");
	myProcess->waitForFinished();
}

void Widget::on_buttonBox_clicked(QAbstractButton *button)
{
	QDialogButtonBox::StandardButton stdButton = ui->buttonBox->standardButton(button);
			if (stdButton==QDialogButtonBox::Ok) {
				   //  EXE CMD
			qDebug() << buildLine();
			myFunction(buildLine());
					}
			else if (stdButton==QDialogButtonBox::Cancel) {
					QApplication::quit();
			}
}

void Widget::exeLine(){
}

void Widget::on_videoFileButton_clicked()
{
	QString *videoFileName = new QString();
	*videoFileName = QFileDialog::getOpenFileName(this,tr("Open Video File to Analyze"), QDir::currentPath(), tr("Video Files (*.mp4 *.m4v)"));
	ui->lineEdit_file_path->setText(*videoFileName);
	delete videoFileName;
}

void Widget::on_thumbDirButton_clicked()
{
	QString *thumbFileDir = new QString();
	*thumbFileDir = QFileDialog::getExistingDirectory(this,tr("Select Thumbs Directory"), QDir::currentPath());
	ui->lineEdit_thumbs_path->setText(*thumbFileDir);
	delete thumbFileDir;
}

void Widget::on_imagesFileButton_clicked()
{
	QString *imagesFileDir = new QString();
	*imagesFileDir = QFileDialog::getExistingDirectory(this,tr("Select Images Directory"), QDir::currentPath());
	ui->lineEdit_images_path->setText(*imagesFileDir);
	delete imagesFileDir;
}


QString Widget::buildLine(){
	dir = ui->lineEdit_file_path->text();
	line = "videoanalyzer ";
	QString temp = ui->lineEdit_images_path->text();
	if(temp.contains(" "))return errBlank;
	line += "--outputDir "+ui->lineEdit_images_path->text()+"/ ";

	//THUMBS
	 // ARE THUMBS CHECKED?
	if(ui->checkBox_genThumbs->isChecked())
	{
		QString thumbs = "--generateThumbs Y ";
		thumbs = thumbs + "--thumbnailWidth "+ui->lineEdit_tw->text()+" ";
		thumbs = thumbs + "--thumbnailFormat "+ui->lineEdit_tf->text()+" ";
		thumbs = thumbs + "--thumbnailPrefix "+ui->lineEdit_tp->text()+" ";
		thumbs += "--thumbnailDir "+ui->lineEdit_thumbs_path->text()+"/ ";
		line += thumbs;
	}
	else line += "--generateThumbs N ";

	// IMAGES
	 // ARE IMAGES CHECKED?
	if(ui->checkBox_genImages->isChecked())
	{
		QString images = "--generateImages Y ";
		images += "--imageFormat "+ui->lineEdit_if->text()+" ";
		images += "--imagePrefix "+ui->lineEdit_ip->text()+" ";
		line += images;
	}
	else line+="--generateImages N ";

	//SEQUENCE
	 //ARE SEQUENCE CHECKED?
	if(ui->checkBox_genSequence->isChecked())
	{
		QString seq = "--generateSequence Y ";
		seq += "--sequenceFile "+ui->lineEdit_df->text()+" ";
		line += seq;
	}
	else line += "--generateSequence N";

	line += "--frameStep "+QString::number(ui->spinBox_step->value())+" ";
	line += "--tresshold "+QString::number(ui->spinBox_th->value())+" ";

	line += "--debugLog Y ";
	line += dir;

return line;
}
