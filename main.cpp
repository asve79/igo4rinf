#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;

#pragma pack(push,1)
typedef struct t_rinfpoi {
    int idx;
    float x;
    float y;
    int type;
    int speed;
    int dirTYpe;
    int angle;
} rinfpoi;
#pragma pack(pop)

int main(int argc, char *argv[])
{
    QFile infile;
    QFile outfile;
    QString procline;
    QStringList proclst;
    rinfpoi poiitem;
    int i=0;
    QCoreApplication a(argc, argv);

    if (argc < 2){
        cout << "Prease tell conversion file name." << endl;
        return 1;
    }
    infile.setFileName(argv[1]);
    if (!infile.open(QIODevice::ReadOnly)){
        cout << "cant open file " << argv[1] << endl;
        return 1;
    }

    if (argc == 3){
        outfile.setFileName(QString(argv[2]));
    } else {
        outfile.setFileName(QString(argv[1]) + ".rinf");
    }
    if (!outfile.open(QIODevice::WriteOnly)){
        cout << "cant create file " <<  argv[1] << ".rinf" << endl;
        infile.close();
        return 1;
    }

    QDataStream fout(&outfile);
    while (!infile.atEnd()){
        procline = infile.readLine();
        if (procline.indexOf("//",0)==0)
            continue;
        proclst = procline.split(",");
        poiitem.x = proclst.at(0).toFloat();
        poiitem.y = proclst.at(1).toFloat();
        poiitem.type = proclst.at(2).toInt();
        poiitem.speed = proclst.at(3).toInt();
        poiitem.dirTYpe = proclst.at(4).toInt();
        if (poiitem.dirTYpe==2){                    //Если камера смотрит в 2 направления, то оставим сделаем 2 записи
            poiitem.angle = proclst.at(5).toInt();
            poiitem.dirTYpe--;
        }
        if (proclst.at(5).toInt() >= 180)           //Для IGO нужен "реверс" угла просмотра
            poiitem.angle = proclst.at(5).toInt()-180;
        else
            poiitem.angle = proclst.at(5).toInt() + 180;
        fout << poiitem.x << poiitem.y << poiitem.type << poiitem.speed << poiitem.dirTYpe << poiitem.angle;

        i++;
    }

    outfile.close();
    infile.close();

    cout << "Converted " << i << " records";
//    return a.exec();
}
